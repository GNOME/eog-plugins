#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "eog-postr-plugin.h"

#include <gmodule.h>
#include <glib/gi18n-lib.h>

#include <eog/eog-debug.h>
#include <eog/eog-thumb-view.h>
#include <eog/eog-image.h>

#define WINDOW_DATA_KEY "EogPostrWindowData"
#define MENU_PATH "/MainMenu/ToolsMenu/ToolsOps_2"

EOG_PLUGIN_REGISTER_TYPE(EogPostrPlugin, eog_postr_plugin)

typedef struct
{
	GtkActionGroup *ui_action_group;
	guint ui_id;
} WindowData;

static void
postr_cb (GtkAction	*action,
	  EogWindow *window)
{
	GtkWidget *thumbview = eog_window_get_thumb_view (window);
	GList *images, *i;
	gchar *cmd = g_strdup ("postr ");

	images = eog_thumb_view_get_selected_images (EOG_THUMB_VIEW (thumbview));

	for (i = g_list_first (images); i; i = i->next) {
		EogImage *image = (EogImage *) i->data;
		GFile *imgfile;
		gchar *imgpath;

		imgfile = eog_image_get_file (image);
		imgpath = g_file_get_path (imgfile);

		if (G_LIKELY (imgpath != NULL))
			cmd = g_strconcat (cmd, "\"", imgpath, "\"", " ", NULL);

		g_free (imgpath);
		g_object_unref (imgfile);
	}

	g_spawn_command_line_async (cmd, NULL);
}

static const GtkActionEntry action_entries[] =
{
	{ "RunPostr",
	  "postr",
	  N_("Upload to Flickr"),
	  NULL,
	  N_("Upload your pictures to Flickr"),
	  G_CALLBACK (postr_cb) }
};

static void
free_window_data (WindowData *data)
{
	g_return_if_fail (data != NULL);

	eog_debug (DEBUG_PLUGINS);

	g_object_unref (data->ui_action_group);

	g_free (data);
}

static void
eog_postr_plugin_init (EogPostrPlugin *plugin)
{
	eog_debug_message (DEBUG_PLUGINS, "EogPostrPlugin initializing");
}

static void
eog_postr_plugin_finalize (GObject *object)
{
	eog_debug_message (DEBUG_PLUGINS, "EogPostrPlugin finalizing");

	G_OBJECT_CLASS (eog_postr_plugin_parent_class)->finalize (object);
}

static void
impl_activate (EogPlugin *plugin,
	       EogWindow *window)
{
	GtkUIManager *manager;
	WindowData *data;

	eog_debug (DEBUG_PLUGINS);

	data = g_new (WindowData, 1);
	manager = eog_window_get_ui_manager (window);

	data->ui_action_group = gtk_action_group_new ("EogPostrPluginActions");

	gtk_action_group_set_translation_domain (data->ui_action_group,
						 GETTEXT_PACKAGE);
	gtk_action_group_add_actions (data->ui_action_group,
				      action_entries,
				      G_N_ELEMENTS (action_entries),
				      window);

	gtk_ui_manager_insert_action_group (manager,
					    data->ui_action_group,
					    -1);

	data->ui_id = gtk_ui_manager_new_merge_id (manager);

	g_object_set_data_full (G_OBJECT (window),
				WINDOW_DATA_KEY,
				data,
				(GDestroyNotify) free_window_data);

	gtk_ui_manager_add_ui (manager,
			       data->ui_id,
			       MENU_PATH,
			       "RunPostr",
			       "RunPostr",
			       GTK_UI_MANAGER_MENUITEM,
			       FALSE);
}

static void
impl_deactivate	(EogPlugin *plugin,
		 EogWindow *window)
{
	GtkUIManager *manager;
	WindowData *data;

	eog_debug (DEBUG_PLUGINS);

	manager = eog_window_get_ui_manager (window);

	data = (WindowData *) g_object_get_data (G_OBJECT (window),
						 WINDOW_DATA_KEY);
	g_return_if_fail (data != NULL);

	gtk_ui_manager_remove_ui (manager,
				  data->ui_id);

	gtk_ui_manager_remove_action_group (manager,
					    data->ui_action_group);

	g_object_set_data (G_OBJECT (window),
			   WINDOW_DATA_KEY,
			   NULL);
}

static void
impl_update_ui (EogPlugin *plugin,
		EogWindow *window)
{
	WindowData *data;

	eog_debug (DEBUG_PLUGINS);

	data = (WindowData *) g_object_get_data (G_OBJECT (window),
						 WINDOW_DATA_KEY);
}

static void
eog_postr_plugin_class_init (EogPostrPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	EogPluginClass *plugin_class = EOG_PLUGIN_CLASS (klass);

	object_class->finalize = eog_postr_plugin_finalize;

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;
	plugin_class->update_ui = impl_update_ui;
}
