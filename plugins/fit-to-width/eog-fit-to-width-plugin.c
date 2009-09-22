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
#include <eog/eog-scroll-view.h>
#include <eog/eog-image.h>

#include "eog-fit-to-width-plugin.h"


#define WINDOW_DATA_KEY "EogFitToWidthWindowData"

EOG_PLUGIN_REGISTER_TYPE(EogFitToWidthPlugin, eog_fit_to_width_plugin)

typedef struct
{
	GtkActionGroup *ui_action_group;
	guint           ui_menuitem_id;
} WindowData;

static void
fit_to_width_cb (GtkAction *action,
	  EogWindow *window)
{
	GtkWidget     *view;
	EogImage      *image;
	gint           image_width;
	gint           image_height;
	gint           view_width;
	double         zoom;

	g_return_if_fail (EOG_IS_WINDOW (window));

	view = eog_window_get_view (window);
	image = eog_window_get_image (window);

	g_return_if_fail (EOG_IS_SCROLL_VIEW (view));
	g_return_if_fail (EOG_IS_IMAGE (image));

	eog_image_get_size (image, &image_width, &image_height);
	view_width = view->allocation.width;

	// HACK: It's necessary subtract the width size (15) of vscrollbar
	//       to scrollview for obtain the display area.
	zoom = (double) (view_width - 15) / image_width;

	eog_scroll_view_set_zoom (EOG_SCROLL_VIEW (view), zoom);
}

static const gchar * const ui_definition =
	"<ui><menubar name=\"MainMenu\">"
	"<menu action=\"View\">"
	"<menuitem action=\"EogPluginFitToWidth\"/>"
	"</menu></menubar></ui>";

static const GtkActionEntry action_entries[] =
{
	{ "EogPluginFitToWidth",
	  GTK_STOCK_ZOOM_FIT,
	  N_("Fit to width"),
	  NULL,
	  N_("Zoom to fit image width"),
	  G_CALLBACK (fit_to_width_cb) }
};

static void
free_window_data (WindowData *data)
{
	g_return_if_fail (data != NULL);

	g_object_unref (data->ui_action_group);

	g_free (data);
}


static void
eog_fit_to_width_plugin_init (EogFitToWidthPlugin *plugin)
{
}

static void
impl_activate (EogPlugin *plugin,
	       EogWindow *window)
{
	GtkUIManager *manager;
	WindowData *data;

	manager = eog_window_get_ui_manager (window);
	data = g_new (WindowData, 1);

	data->ui_action_group = gtk_action_group_new ("EogFitToWidthPluginActions");

	gtk_action_group_set_translation_domain (data->ui_action_group,
						 GETTEXT_PACKAGE);

	gtk_action_group_add_actions (data->ui_action_group,
				      action_entries,
				      G_N_ELEMENTS (action_entries),
				      window);

	gtk_ui_manager_insert_action_group (manager,
					    data->ui_action_group,
					    -1);

	data->ui_menuitem_id = gtk_ui_manager_add_ui_from_string (manager,
								  ui_definition,
								  -1, NULL);

	g_object_set_data_full (G_OBJECT (window),
				WINDOW_DATA_KEY,
				data,
				(GDestroyNotify) free_window_data);
}

static void
impl_deactivate	(EogPlugin *plugin,
		 EogWindow *window)
{
	GtkUIManager *manager;
	WindowData *data;

	manager = eog_window_get_ui_manager (window);

	data = (WindowData *) g_object_get_data (G_OBJECT (window),
						 WINDOW_DATA_KEY);
	g_return_if_fail (data != NULL);

	gtk_ui_manager_remove_ui (manager,
				  data->ui_menuitem_id);

	gtk_ui_manager_remove_action_group (manager,
					    data->ui_action_group);

	g_object_set_data (G_OBJECT (window),
			   WINDOW_DATA_KEY,
			   NULL);
}

static void
eog_fit_to_width_plugin_class_init (EogFitToWidthPluginClass *klass)
{
	EogPluginClass *plugin_class = EOG_PLUGIN_CLASS (klass);

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;
}
