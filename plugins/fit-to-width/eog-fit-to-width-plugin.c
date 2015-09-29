/* Fit-to-width -- Fit zoom to the image width
 *
 * Copyright (C) 2009 The Free Software Foundation
 *
 * Author: Javier Sánchez  <jsanchez@deskblue.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>
#include <eog/eog-application.h>
#include <eog/eog-scroll-view.h>
#include <eog/eog-image.h>
#include <eog/eog-window.h>
#include <eog/eog-window-activatable.h>

#include <libpeas/peas.h>

#include "eog-fit-to-width-plugin.h"

#define EOG_FIT_TO_WIDTH_PLUGIN_MENU_ID "EogPluginFitToWidth"
#define EOG_FIT_TO_WIDTH_PLUGIN_ACTION "zoom-fit-width"


static void
eog_window_activatable_iface_init (EogWindowActivatableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (EogFitToWidthPlugin, eog_fit_to_width_plugin,
		PEAS_TYPE_EXTENSION_BASE, 0,
		G_IMPLEMENT_INTERFACE_DYNAMIC (EOG_TYPE_WINDOW_ACTIVATABLE,
					eog_window_activatable_iface_init))

enum {
	PROP_0,
	PROP_WINDOW
};

static void
fit_to_width_cb (GSimpleAction *simple,
		 GVariant      *parameter,
		 gpointer       user_data)
{
	EogWindow     *window;
	GtkWidget     *view;
	EogImage      *image;
	gint           image_width;
	gint           image_height;
	gint           view_width;
	double         zoom;
	GtkAllocation  allocation;


	g_return_if_fail (EOG_IS_WINDOW (user_data));

	window = EOG_WINDOW (user_data);

	view = eog_window_get_view (window);
	image = eog_window_get_image (window);

	g_return_if_fail (EOG_IS_SCROLL_VIEW (view));
	g_return_if_fail (EOG_IS_IMAGE (image));

	eog_image_get_size (image, &image_width, &image_height);
	gtk_widget_get_allocation (view, &allocation);
	view_width = allocation.width;

	// HACK: It's necessary subtract the width size (15) of vscrollbar
	//       to scrollview for obtain the display area.
	zoom = (double) (view_width - 15) / image_width;

	eog_scroll_view_set_zoom (EOG_SCROLL_VIEW (view), zoom);
}

static void
eog_fit_to_width_plugin_init (EogFitToWidthPlugin *plugin)
{
}

static void
impl_activate (EogWindowActivatable *activatable)
{
	const gchar * const accel_keys[] = { "W", NULL };
	EogFitToWidthPlugin *plugin = EOG_FIT_TO_WIDTH_PLUGIN (activatable);
	GMenu *model, *menu;
	GMenuItem *item;
	GSimpleAction *action;
	GAction *ref_action;

	model= eog_window_get_gear_menu_section (plugin->window,
						 "plugins-section");

	g_return_if_fail (G_IS_MENU (model));

	/* Setup and inject action */
	action = g_simple_action_new (EOG_FIT_TO_WIDTH_PLUGIN_ACTION, NULL);
	g_signal_connect(action, "activate",
			 G_CALLBACK (fit_to_width_cb), plugin->window);
	g_action_map_add_action (G_ACTION_MAP (plugin->window),
				 G_ACTION (action));

	/* Bind to the zoom-normal action's enabled property to only enable
	 * fit-to-width zooming if zooming is generally enabled */
	ref_action = g_action_map_lookup_action (G_ACTION_MAP (plugin->window),
						 "zoom-normal");
	if (ref_action)
		g_object_bind_property (ref_action, "enabled",
					action, "enabled",
					G_BINDING_DEFAULT | G_BINDING_SYNC_CREATE);
	g_object_unref (action);

	/* Append entry to the window's gear menu */
	menu = g_menu_new ();
	g_menu_append (menu, _("Fit to width"),
		       "win." EOG_FIT_TO_WIDTH_PLUGIN_ACTION);

	item = g_menu_item_new_section (NULL, G_MENU_MODEL (menu));
	g_menu_item_set_attribute (item, "id",
				   "s", EOG_FIT_TO_WIDTH_PLUGIN_MENU_ID);
	g_menu_item_set_attribute (item, G_MENU_ATTRIBUTE_ICON,
				   "s", "zoom-fit-best-symbolic");
	g_menu_append_item (model, item);
	g_object_unref (item);

	g_object_unref (menu);

	/* Define accelerator keys */
	gtk_application_set_accels_for_action (GTK_APPLICATION (EOG_APP),
					       "win." EOG_FIT_TO_WIDTH_PLUGIN_ACTION,
					       accel_keys);
}

static void
impl_deactivate	(EogWindowActivatable *activatable)
{
	const gchar * const empty_accels[1] = { NULL };
	EogFitToWidthPlugin *plugin = EOG_FIT_TO_WIDTH_PLUGIN (activatable);
	GMenu *menu;
	GMenuModel *model;
	gint i;

	menu = eog_window_get_gear_menu_section (plugin->window,
						 "plugins-section");

	g_return_if_fail (G_IS_MENU (menu));

	/* Remove menu entry */
	model = G_MENU_MODEL (menu);
	for (i = 0; i < g_menu_model_get_n_items (model); i++) {
		gchar *id;
		if (g_menu_model_get_item_attribute (model, i, "id", "s", &id)) {
			const gboolean found =
				(g_strcmp0 (id, EOG_FIT_TO_WIDTH_PLUGIN_MENU_ID) == 0);
			g_free (id);

			if (found) {
				g_menu_remove (menu, i);
				break;
			}
		}
	}

	/* Unset accelerator */
	gtk_application_set_accels_for_action(GTK_APPLICATION (EOG_APP),
					      "win." EOG_FIT_TO_WIDTH_PLUGIN_ACTION,
					      empty_accels);

	/* Finally remove action */
	g_action_map_remove_action (G_ACTION_MAP (plugin->window),
				    EOG_FIT_TO_WIDTH_PLUGIN_ACTION);
}

static void
eog_fit_to_width_plugin_dispose (GObject *object)
{
	EogFitToWidthPlugin *plugin = EOG_FIT_TO_WIDTH_PLUGIN (object);

	if (plugin->window != NULL) {
		g_object_unref (plugin->window);
		plugin->window = NULL;
	}

	G_OBJECT_CLASS (eog_fit_to_width_plugin_parent_class)->dispose (object);
}

static void
eog_fit_to_width_plugin_get_property (GObject    *object,
				      guint       prop_id,
				      GValue     *value,
				      GParamSpec *pspec)
{
	EogFitToWidthPlugin *plugin = EOG_FIT_TO_WIDTH_PLUGIN (object);

	switch (prop_id)
	{
	case PROP_WINDOW:
		g_value_set_object (value, plugin->window);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
eog_fit_to_width_plugin_set_property (GObject      *object,
				      guint         prop_id,
				      const GValue *value,
				      GParamSpec   *pspec)
{
	EogFitToWidthPlugin *plugin = EOG_FIT_TO_WIDTH_PLUGIN (object);

	switch (prop_id)
	{
	case PROP_WINDOW:
		plugin->window = EOG_WINDOW (g_value_dup_object (value));
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
eog_fit_to_width_plugin_class_init (EogFitToWidthPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = eog_fit_to_width_plugin_dispose;
	object_class->set_property = eog_fit_to_width_plugin_set_property;
	object_class->get_property = eog_fit_to_width_plugin_get_property;

	g_object_class_override_property (object_class, PROP_WINDOW, "window");
}

static void
eog_fit_to_width_plugin_class_finalize (EogFitToWidthPluginClass *klass)
{
	/* Dummy needed for G_DEFINE_DYNAMIC_TYPE_EXTENDED */
}

static void
eog_window_activatable_iface_init (EogWindowActivatableInterface *iface)
{
	iface->activate = impl_activate;
	iface->deactivate = impl_deactivate;
}

G_MODULE_EXPORT void
peas_register_types (PeasObjectModule *module)
{
	eog_fit_to_width_plugin_register_type (G_TYPE_MODULE (module));
	peas_object_module_register_extension_type (module,
						    EOG_TYPE_WINDOW_ACTIVATABLE,
						    EOG_TYPE_FIT_TO_WIDTH_PLUGIN);
}
