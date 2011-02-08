/* Exif-display Plugin - Configuration Interface
 *
 * Copyright (C) 2009-2011 The Free Software Foundation
 *
 * Author: Felix Riemann  <friemann@gnome.org>
 * Based on code by Emmanuel Touzery  <emmanuel.touzery@free.fr>
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
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>

#include <gconf/gconf-client.h>

#include <glib/gi18n-lib.h>
#include <eog/eog-debug.h>

#include <libpeas-gtk/peas-gtk-configurable.h>

#include "eog-exif-display-plugin-setup.h"

#define EOG_EXIF_DISPLAY_CONF_UI_DISPLAY_CHANNELS_HISTOGRAM "/apps/eog/plugins/exif_display/display_channels_histogram"
#define EOG_EXIF_DISPLAY_CONF_UI_DISPLAY_RGB_HISTOGRAM "/apps/eog/plugins/exif_display/display_rgb_histogram"
#define EOG_EXIF_DISPLAY_CONF_UI_DISPLAY_EXIF_STATUSBAR "/apps/eog/plugins/exif_display/display_exif_in_statusbar"

/* copy-pasted from eog-preferences-dialog.c */
#define GCONF_OBJECT_KEY	"GCONF_KEY"

/* copy-pasted from eog-preferences-dialog.c */
#define TOGGLE_INVERT_VALUE	"TOGGLE_INVERT_VALUE"

static GConfClient *gconf_client = NULL;

#define GTKBUILDER_CONFIG_FILE EOG_PLUGINDIR"/exif-display/exif-display-config.ui"

static void
peas_gtk_configurable_iface_init (PeasGtkConfigurableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (EogExifDisplayPluginSetup,
		eog_exif_display_plugin_setup, PEAS_TYPE_EXTENSION_BASE, 0,
		G_IMPLEMENT_INTERFACE_DYNAMIC(PEAS_GTK_TYPE_CONFIGURABLE,
					peas_gtk_configurable_iface_init))


static void
eog_exif_display_plugin_setup_init (EogExifDisplayPluginSetup *setup)
{
	setup->vbox = NULL;
}

/* copy-pasted from eog-preferences-dialog.c */
static void
pd_check_toggle_cb (GtkWidget *widget, gpointer data)
{
	char *key = NULL;
	gboolean invert = FALSE;
	gboolean value;

	key = g_object_get_data (G_OBJECT (widget), GCONF_OBJECT_KEY);
	invert = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (widget), TOGGLE_INVERT_VALUE));

	value = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget));

	if (key == NULL) return;

	gconf_client_set_bool (GCONF_CLIENT (data),
			       key,
			       (invert) ? !value : value,
			       NULL);
}

static void
close_config_window_cb(GtkWidget *widget, gpointer _data)
{
	GtkWidget *data = GTK_WIDGET (_data);

	gtk_widget_destroy (GTK_WIDGET (gtk_widget_get_toplevel (data)));
}

static void
connect_checkbox_to_gconf_setting (GtkToggleButton *checkbox, char *gconf_key)
{
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox),
				      gconf_client_get_bool (gconf_client,
							     gconf_key,
							     NULL));

	g_object_set_data (G_OBJECT (checkbox),
			   GCONF_OBJECT_KEY,
			   gconf_key);

	g_signal_connect (G_OBJECT (checkbox),
			  "toggled",
			  G_CALLBACK (pd_check_toggle_cb),
			  gconf_client);
}

static GtkWidget *
impl_create_config_widget (PeasGtkConfigurable *configurable)
{
	EogExifDisplayPluginSetup *setup;
	GtkBuilder *config_builder;
	GError *error = NULL;
	GtkWidget *display_channels_histogram_widget, *display_rgb_histogram_widget;
	GtkWidget *close_button, *display_camera_settings_in_statusbar;
	GtkWidget *result;

	setup = EOG_EXIF_DISPLAY_PLUGIN_SETUP (configurable);

	config_builder = gtk_builder_new ();
	gtk_builder_set_translation_domain (config_builder, GETTEXT_PACKAGE);
	if (!gtk_builder_add_from_file (config_builder, GTKBUILDER_CONFIG_FILE, &error))
	{
		g_warning ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}
	result = GTK_WIDGET (gtk_builder_get_object (config_builder, "vbox1"));
	gtk_widget_unparent (result);
	display_channels_histogram_widget = GTK_WIDGET (
			gtk_builder_get_object (config_builder, "display_per_channel_histogram"));
	display_rgb_histogram_widget = GTK_WIDGET (
			gtk_builder_get_object (config_builder, "display_rgb_histogram"));
	display_camera_settings_in_statusbar = GTK_WIDGET (
			gtk_builder_get_object (config_builder, "display_camerasettings_statusbar"));

	connect_checkbox_to_gconf_setting (GTK_TOGGLE_BUTTON (display_channels_histogram_widget),
			EOG_EXIF_DISPLAY_CONF_UI_DISPLAY_CHANNELS_HISTOGRAM);
	connect_checkbox_to_gconf_setting (GTK_TOGGLE_BUTTON (display_rgb_histogram_widget),
			EOG_EXIF_DISPLAY_CONF_UI_DISPLAY_RGB_HISTOGRAM);
	connect_checkbox_to_gconf_setting (GTK_TOGGLE_BUTTON (display_camera_settings_in_statusbar),
			EOG_EXIF_DISPLAY_CONF_UI_DISPLAY_EXIF_STATUSBAR);
	return result;
}


static void
eog_exif_display_plugin_setup_dispose (GObject *object)
{
	EogExifDisplayPluginSetup *setup = EOG_EXIF_DISPLAY_PLUGIN_SETUP (object);

	eog_debug_message (DEBUG_PLUGINS,
			   "EogExifDisplayPluginSetup disposing");

	if (setup->vbox != NULL) {
		g_object_unref (setup->vbox);
		setup->vbox = NULL;
	}

	G_OBJECT_CLASS (eog_exif_display_plugin_setup_parent_class)->dispose (object);
}

static void
eog_exif_display_plugin_setup_class_init (EogExifDisplayPluginSetupClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	gconf_client = gconf_client_get_default ();

	object_class->dispose = eog_exif_display_plugin_setup_dispose;
}

static void
peas_gtk_configurable_iface_init (PeasGtkConfigurableInterface *iface)
{
	iface->create_configure_widget = impl_create_config_widget;
}

static void
eog_exif_display_plugin_setup_class_finalize (EogExifDisplayPluginSetupClass *klass)
{
	/* Dummy needed for G_DEFINE_DYNAMIC_TYPE_EXTENDED */
}

void
eog_exif_display_plugin_setup_register_types (PeasObjectModule *module)
{
	eog_exif_display_plugin_setup_register_type (G_TYPE_MODULE (module));
	peas_object_module_register_extension_type (module,
						    PEAS_GTK_TYPE_CONFIGURABLE,
						    EOG_TYPE_EXIF_DISPLAY_PLUGIN_SETUP);
}
