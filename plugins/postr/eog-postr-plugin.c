#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "eog-postr-plugin.h"

#include <gmodule.h>
#include <glib/gi18n-lib.h>

#include <eog/eog-debug.h>
#include <eog/eog-thumb-view.h>
#include <eog/eog-image.h>
#include <eog/eog-window-activatable.h>

#define EOG_POSTR_PLUGIN_MENU_ID "EogPluginPostr"
#define EOG_POSTR_PLUGIN_ACTION "upload-with-postr"

enum {
	PROP_O,
	PROP_WINDOW
};

static void
eog_window_activatable_iface_init (EogWindowActivatableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (EogPostrPlugin, eog_postr_plugin,
		PEAS_TYPE_EXTENSION_BASE, 0,
		G_IMPLEMENT_INTERFACE_DYNAMIC(EOG_TYPE_WINDOW_ACTIVATABLE,
					eog_window_activatable_iface_init))

static void
postr_cb (GSimpleAction *simple,
	  GVariant      *parameter,
	  gpointer       user_data)
{
	EogWindow *window;
	GtkWidget *thumbview;
	GList *images, *i;
	gchar *cmd = g_strdup ("postr ");

	eog_debug(DEBUG_PLUGINS);

	g_return_if_fail (EOG_IS_WINDOW (user_data));

	window = EOG_WINDOW (user_data);
	thumbview = eog_window_get_thumb_view (window);
	images = eog_thumb_view_get_selected_images (EOG_THUMB_VIEW (thumbview));

	for (i = g_list_first (images); i; i = i->next) {
		EogImage *image = (EogImage *) i->data;
		GFile *imgfile;
		gchar *imgpath;
		gchar *oldcmd = cmd;

		imgfile = eog_image_get_file (image);
		imgpath = g_file_get_path (imgfile);

		if (G_LIKELY (imgpath != NULL)) {
			cmd = g_strconcat (oldcmd, "\"", imgpath, "\"", " ", NULL);
			g_free (oldcmd);
		}

		g_free (imgpath);
		g_object_unref (imgfile);
	}

	g_spawn_command_line_async (cmd, NULL);

	g_list_free_full (images, g_object_unref);
	g_free (cmd);
}

static void
eog_postr_plugin_update_action_state (EogPostrPlugin *plugin)
{
	GAction *action;
	EogThumbView *thumbview;
	gboolean enable = FALSE;

	thumbview = EOG_THUMB_VIEW (eog_window_get_thumb_view (plugin->window));

	if (G_LIKELY (thumbview))
	{
		enable = (eog_thumb_view_get_n_selected (thumbview) != 0);
	}

	action = g_action_map_lookup_action (G_ACTION_MAP (plugin->window),
					     EOG_POSTR_PLUGIN_ACTION);
	g_simple_action_set_enabled (G_SIMPLE_ACTION (action), enable);
}

static void
_selection_changed_cb (EogThumbView *thumbview, gpointer user_data)
{
	EogPostrPlugin *plugin = EOG_POSTR_PLUGIN (user_data);

	if (G_LIKELY (plugin))
		eog_postr_plugin_update_action_state (plugin);
}

static void
eog_postr_plugin_init (EogPostrPlugin *plugin)
{
	eog_debug_message (DEBUG_PLUGINS, "EogPostrPlugin initializing");
}


static void
eog_postr_plugin_dispose (GObject *object)
{
	EogPostrPlugin *plugin = EOG_POSTR_PLUGIN (object);

	eog_debug_message (DEBUG_PLUGINS, "EogPostrPlugin disposing");

	if (plugin->window != NULL) {
		g_object_unref (plugin->window);
		plugin->window = NULL;
	}

	G_OBJECT_CLASS (eog_postr_plugin_parent_class)->dispose (object);
}

static void
impl_activate (EogWindowActivatable *activatable)
{
	EogPostrPlugin *plugin = EOG_POSTR_PLUGIN (activatable);
	GMenu *model, *menu;
	GMenuItem *item;
	GSimpleAction *action;

	eog_debug (DEBUG_PLUGINS);

	g_return_if_fail (plugin->window != NULL);

	model= eog_window_get_gear_menu_section (plugin->window,
						 "plugins-section");

	g_return_if_fail (G_IS_MENU (model));

	/* Setup and inject action */
	action = g_simple_action_new (EOG_POSTR_PLUGIN_ACTION, NULL);
	g_signal_connect(action, "activate",
			 G_CALLBACK (postr_cb), plugin->window);
	g_action_map_add_action (G_ACTION_MAP (plugin->window),
				 G_ACTION (action));
	g_object_unref (action);

	g_signal_connect (G_OBJECT (eog_window_get_thumb_view (plugin->window)),
			  "selection-changed",
			  G_CALLBACK (_selection_changed_cb),
			  plugin);
	eog_postr_plugin_update_action_state (plugin);

	/* Append entry to the window's gear menu */
	menu = g_menu_new ();
	g_menu_append (menu, _("Upload to Flickr"),
		       "win." EOG_POSTR_PLUGIN_ACTION);

	item = g_menu_item_new_section (NULL, G_MENU_MODEL (menu));
	g_menu_item_set_attribute (item, "id",
				   "s", EOG_POSTR_PLUGIN_MENU_ID);
	g_menu_item_set_attribute (item, G_MENU_ATTRIBUTE_ICON,
				   "s", "postr");
	g_menu_append_item (model, item);
	g_object_unref (item);

	g_object_unref (menu);

}

static void
impl_deactivate	(EogWindowActivatable *activatable)
{
	EogPostrPlugin *plugin = EOG_POSTR_PLUGIN (activatable);
	GMenu *menu;
	GMenuModel *model;
	gint i;

	eog_debug (DEBUG_PLUGINS);

	menu = eog_window_get_gear_menu_section (plugin->window,
						 "plugins-section");

	g_return_if_fail (G_IS_MENU (menu));

	/* Remove menu entry */
	model = G_MENU_MODEL (menu);
	for (i = 0; i < g_menu_model_get_n_items (model); i++) {
		gchar *id;
		if (g_menu_model_get_item_attribute (model, i, "id", "s", &id)) {
			const gboolean found =
				(g_strcmp0 (id, EOG_POSTR_PLUGIN_MENU_ID) == 0);
			g_free (id);

			if (found) {
				g_menu_remove (menu, i);
				break;
			}
		}
	}

	g_signal_handlers_disconnect_by_func (eog_window_get_thumb_view (plugin->window),
					      _selection_changed_cb, plugin);


	/* Finally remove action */
	g_action_map_remove_action (G_ACTION_MAP (plugin->window),
				    EOG_POSTR_PLUGIN_ACTION);
}

static void
eog_postr_plugin_get_property (GObject    *object,
			       guint       prop_id,
			       GValue     *value,
			       GParamSpec *pspec)
{
	EogPostrPlugin *plugin = EOG_POSTR_PLUGIN (object);

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
eog_postr_plugin_set_property (GObject      *object,
			       guint         prop_id,
			       const GValue *value,
			       GParamSpec   *pspec)
{
	EogPostrPlugin *plugin = EOG_POSTR_PLUGIN (object);

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
eog_postr_plugin_class_init (EogPostrPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = eog_postr_plugin_dispose;
	object_class->set_property = eog_postr_plugin_set_property;
	object_class->get_property = eog_postr_plugin_get_property;

	g_object_class_override_property (object_class, PROP_WINDOW, "window");
}

static void
eog_postr_plugin_class_finalize (EogPostrPluginClass *klass)
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
	eog_postr_plugin_register_type (G_TYPE_MODULE (module));
	peas_object_module_register_extension_type (module,
						    EOG_TYPE_WINDOW_ACTIVATABLE,
						    EOG_TYPE_POSTR_PLUGIN);
}
