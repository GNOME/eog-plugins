#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "eog-champlain-plugin.h"

#include <gmodule.h>
#include <glib/gi18n-lib.h>

#include <eog/eog-debug.h>
#include <eog/eog-thumb-view.h>
#include <eog/eog-image.h>
#include <eog/eog-window.h>
#include <eog/eog-sidebar.h>

#include <math.h>
#include <string.h>
#include <champlain/champlain.h>
#include <champlain-gtk/champlain-gtk.h>
#include <clutter-cairo/clutter-cairo.h>
#include <clutter-gtk/gtk-clutter-embed.h>
#include <libexif/exif-data.h>

#define WINDOW_DATA_KEY "EogChamplainWindowData"

EOG_PLUGIN_REGISTER_TYPE (EogChamplainPlugin, eog_champlain_plugin)

#define PADDING 4
#define FACTOR 2.0

typedef struct {
	/* Handlers ids */
	guint selection_changed_id;
	guint thumbnail_changed_id;

	GtkWidget *viewport;
	ClutterActor *map, *marker, *layer;
} WindowData;

static void
free_window_data (WindowData *data)
{
	g_return_if_fail (data != NULL);

	eog_debug (DEBUG_PLUGINS);

	g_free (data);
}

static void
eog_champlain_plugin_init (EogChamplainPlugin *plugin)
{
	eog_debug_message (DEBUG_PLUGINS, "EogChamplainPlugin initializing");
	gtk_clutter_init (NULL, NULL);
}

static void
eog_champlain_plugin_finalize (GObject *object)
{
	eog_debug_message (DEBUG_PLUGINS, "EogChamplainPlugin finalizing");

	G_OBJECT_CLASS (eog_champlain_plugin_parent_class)->finalize (object);
}

static ClutterActor *
create_champlain_marker(EogImage *image)
{
	ClutterActor *actor, *thumb, *marker;
	ClutterColor color = { 0x99, 0x99, 0x99, 0xff };
	ClutterColor darker_color;
	GdkPixbuf* thumbnail = eog_image_get_thumbnail (image);

	marker = champlain_marker_new ();
	thumb = clutter_texture_new ();
	guint width, height, point;

	if (thumbnail) {
		gtk_clutter_texture_set_from_pixbuf (CLUTTER_TEXTURE (thumb),
						     thumbnail);
		clutter_actor_set_scale (thumb, 1 / FACTOR, 1 / FACTOR);
		width = clutter_actor_get_width (thumb) / FACTOR;
		height = clutter_actor_get_height (thumb) / FACTOR - PADDING;
	}
	else
		width = height = 10;

	point = width / 3;

	actor = clutter_cairo_new (width, height + point);
	cairo_t * cr = clutter_cairo_create (CLUTTER_CAIRO (actor));

	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width (cr, 1);

	cairo_line_to (cr, width - point, height);
	cairo_line_to (cr, width / 2, height + point);
	cairo_line_to (cr, point, height);
	cairo_close_path (cr);

	cairo_set_source_rgba (cr,
			       color.red / 255.0,
			       color.green / 255.0,
			       color.blue / 255.0,
			       color.alpha / 255.0);
	cairo_fill (cr);
	clutter_color_darken (&color, &darker_color);
	cairo_set_source_rgba (cr,
			       darker_color.red / 255.0,
			       darker_color.green / 255.0,
			       darker_color.blue / 255.0,
			       darker_color.alpha / 255.0);
	cairo_stroke (cr);
	cairo_destroy (cr);

	clutter_container_add (CLUTTER_CONTAINER (marker), actor, NULL);
	clutter_container_add (CLUTTER_CONTAINER (marker), thumb, NULL);

	clutter_actor_set_anchor_point (marker, width / 2, height + point);

	if (thumbnail)
		g_object_unref (thumbnail);

	return marker;
}

static gboolean
get_coordinates (EogImage *image, gdouble *latitude, gdouble *longitude)
{
	ExifData *exif_data;
	gchar buffer[32];
	gdouble lon, lat;
	gfloat hour, min, sec;

	exif_data = (ExifData *)eog_image_get_exif_info (image);

	if (exif_data) {

		eog_exif_util_get_value (exif_data,
					 EXIF_TAG_GPS_LONGITUDE,
					 buffer,
					 32);
		if (strlen(buffer) < 5) {
			exif_data_unref (exif_data);
			return FALSE;
		}

		sscanf(buffer, "%f, %f, %f", &hour, &min, &sec);
		lon = hour;
		lon += min / 60.0;
		lon += sec / 3600.0;

		eog_exif_util_get_value (exif_data,
					 EXIF_TAG_GPS_LONGITUDE_REF,
					 buffer,
					 32);
		if (strcmp(buffer, "W") == 0)
			lon *= -1;

		eog_exif_util_get_value (exif_data,
					 EXIF_TAG_GPS_LATITUDE,
					 buffer,
					 32);
		if (strlen(buffer) < 5) {
			exif_data_unref (exif_data);
			return FALSE;
		}

		sscanf(buffer, "%f, %f, %f", &hour, &min, &sec);
		lat = hour;
		lat += min / 60.0;
		lat += sec / 3600.0;

		eog_exif_util_get_value (exif_data,
					 EXIF_TAG_GPS_LATITUDE_REF,
					 buffer,
					 32);
		if (strcmp(buffer, "S") == 0)
			lat *= -1;

		*longitude = lon;
		*latitude = lat;

		exif_data_unref (exif_data);
		return TRUE;
	}
	return FALSE;
}

static void
create_marker (EogImage *image, WindowData *data)
{
	data->marker = NULL;
	if (!image)
		return;

	if (!eog_image_has_data (image, EOG_IMAGE_DATA_EXIF) &&
	    !eog_image_load (image, EOG_IMAGE_DATA_EXIF, NULL, NULL))
		return;

	gdouble lon, lat;
	if (get_coordinates (image, &lat, &lon)) {
		data->marker = create_champlain_marker (image);

		clutter_actor_show (data->marker);
		champlain_marker_set_position (CHAMPLAIN_MARKER (data->marker),
					       lat,
					       lon);
		clutter_container_add (CLUTTER_CONTAINER (data->layer),
				       data->marker,
				       NULL);
	}
}

static void
thumbnail_changed_cb (EogImage* image, WindowData* data)
{
	gdouble lon, lat;

	if (eog_image_get_thumbnail(image)) {
		create_marker (image, data);
		if (data->marker) {
			g_object_get (data->marker,
				      "latitude", &lat,
				      "longitude", &lon,
				      NULL);
			g_object_set (G_OBJECT(data->map),
				      "zoom-level",
				      15,
				      NULL);
			champlain_view_center_on (CHAMPLAIN_VIEW (data->map),
						  lat,
						  lon);
		} else {
			g_object_set (G_OBJECT(data->map),
				      "zoom-level",
				      3,
				      NULL);
		}
		g_signal_handler_disconnect (image,
					     data->thumbnail_changed_id);
	}
}

static void
selection_changed_cb (EogThumbView *view, WindowData *data)
{
	EogImage *image;

	if (!eog_thumb_view_get_n_selected (view))
		return;

	image = eog_thumb_view_get_first_selected_image (view);

	g_return_if_fail (image != NULL);

	if (data->marker)
		clutter_container_remove (CLUTTER_CONTAINER (data->layer),
					  data->marker,
					  NULL);

	data->thumbnail_changed_id = g_signal_connect (G_OBJECT (image),
						       "thumbnail-changed",
						       G_CALLBACK (thumbnail_changed_cb),
						       data);

	/* Call the callback because images that are already in the
	 * thumbview don't emit thumbnail_changed
	 */
	thumbnail_changed_cb (image, data);

	g_object_unref (image);
}

static void
zoom_in (GtkWidget *widget, ChamplainView *view)
{
	champlain_view_zoom_in(view);
}

static void
zoom_out (GtkWidget *widget, ChamplainView *view)
{
	champlain_view_zoom_out(view);
}

static void
impl_activate (EogPlugin *plugin, EogWindow *window)
{
	GtkWidget *sidebar, *thumbview, *vbox, *bbox, *button, *viewport;
	WindowData *data;

	eog_debug (DEBUG_PLUGINS);

	data = g_new0(WindowData, 1);
	g_object_set_data_full (G_OBJECT (window),
				WINDOW_DATA_KEY,
				data,
				(GDestroyNotify) free_window_data);


	viewport = gtk_viewport_new (NULL, NULL);
	gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport),
				      GTK_SHADOW_ETCHED_IN);

	data->map = champlain_view_new (CHAMPLAIN_VIEW_MODE_KINETIC);
	g_object_set (G_OBJECT (data->map), "zoom-level", 3, NULL);
	gtk_container_add (GTK_CONTAINER (viewport),
			   champlain_view_embed_new (CHAMPLAIN_VIEW (data->map)));

	vbox = gtk_vbox_new(FALSE, 0);
	bbox =	gtk_toolbar_new ();
	button = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_STOCK_ZOOM_IN));
	g_signal_connect (button,
			  "clicked",
			  G_CALLBACK (zoom_in),
			  data->map);
	gtk_container_add (GTK_CONTAINER (bbox), button);

	button = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_STOCK_ZOOM_OUT));
	g_signal_connect (button,
			  "clicked",
			  G_CALLBACK (zoom_out),
			  data->map);
	gtk_container_add (GTK_CONTAINER (bbox), button);

	data->layer = champlain_layer_new();
	champlain_view_add_layer (CHAMPLAIN_VIEW (data->map), data->layer);

	sidebar = eog_window_get_sidebar (window);
	data->viewport = vbox;
	gtk_box_pack_start (GTK_BOX (vbox), bbox, FALSE, FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox), viewport);
	eog_sidebar_add_page (EOG_SIDEBAR (sidebar), _("Map"), vbox);
	gtk_widget_show_all (vbox);

	thumbview = eog_window_get_thumb_view (window);
	data->selection_changed_id = g_signal_connect (G_OBJECT (thumbview),
						       "selection-changed",
						       G_CALLBACK (selection_changed_cb),
						       data);

	/* Call the callback because if the plugin is activated after
	 *  the image is loaded, selection_changed isn't emited
	 */
	selection_changed_cb (EOG_THUMB_VIEW (thumbview), data);
}

static void
impl_deactivate	(EogPlugin *plugin,
		 EogWindow *window)
{
	WindowData *data;
	GtkWidget *sidebar, *thumbview;

	eog_debug (DEBUG_PLUGINS);

	data = g_object_get_data (G_OBJECT (window), WINDOW_DATA_KEY);
	g_return_if_fail (data != NULL);

	sidebar = eog_window_get_sidebar (window);
	eog_sidebar_remove_page(EOG_SIDEBAR (sidebar), data->viewport);

	thumbview = eog_window_get_thumb_view (window);
	g_signal_handler_disconnect (thumbview, data->selection_changed_id);

	g_object_set_data (G_OBJECT (window), WINDOW_DATA_KEY, NULL);
}

static void
eog_champlain_plugin_class_init (EogChamplainPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	EogPluginClass *plugin_class = EOG_PLUGIN_CLASS (klass);

	object_class->finalize = eog_champlain_plugin_finalize;

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;
}
