#include <stdio.h>
#include <gst/gst.h>
#include <gtk/gtk.h>


typedef struct _CustomData {
  GstElement *pipeline;
  GstElement *source;
  GstElement *demuxer;
  GstElement *decoder;
  GstElement *convert;
  GstElement *sink;
} CustomData;

void init_player();

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
	GtkWidget *button = (GtkWidget *)data;
	const gchar * button_label = gtk_button_get_label(button);
	g_print ("Button %s clicked\n", button_label);
	if(strcmp(button_label, "Button1") == 0)
	{
		g_message("Button1");
	}
	else
	{
		g_message("Button2");
	}
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *button_box;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add (GTK_CONTAINER (window), button_box);

  button = gtk_button_new_with_label ("Button1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), button);
  // g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  gtk_container_add (GTK_CONTAINER (button_box), button);
  
  button = gtk_button_new_with_label ("Button2");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), button);
  // g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  gtk_container_add (GTK_CONTAINER (button_box), button);

  gtk_widget_show_all (window);
}

static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
      gchar  *debug;
      GError *error;

      gst_message_parse_error (msg, &error, &debug);
      g_free (debug);

      g_printerr ("Error: %s\n", error->message);
      g_error_free (error);

      g_main_loop_quit (loop);
      break;
    }
    default:
      break;
  }

  return TRUE;
}

static void on_pad_added (GstElement *element, GstPad *pad, gpointer data)
{
	GstPad *sinkpad;
	GstElement *convert = (GstElement *) data;
	
	GstCaps * caps = gst_pad_query_caps (pad, NULL);
	GstStructure * str = gst_caps_get_structure (caps, 0);

	/* We can now link this pad with the vorbis-decoder sink pad */
	g_print ("Dynamic pad created, linking demuxer/decoder, %s\n", gst_structure_get_name (str));

	sinkpad = gst_element_get_static_pad (convert, "sink");

	gst_pad_link (pad, sinkpad);

	gst_object_unref (sinkpad);
}

void init_player()
{
	CustomData data;
	GstBus *bus;
	guint bus_watch_id;
	
	/* Initialize GStreamer */
	gst_init (NULL, NULL);
	
	/* Create gstreamer elements */
	data.pipeline = gst_pipeline_new ("audio-player");
	data.source   = gst_element_factory_make("filesrc", "file-source");
	// data.demuxer  = gst_element_factory_make("decodebin", "ogg-demuxer");
	data.decoder  = gst_element_factory_make("decodebin", "vorbis-decoder");
	data.convert  = gst_element_factory_make("audioconvert", "converter");
	data.sink     = gst_element_factory_make("autoaudiosink", "audio-output");
	if (!data.pipeline || !data.source || !data.decoder  || !data.sink) 
	{
		g_printerr ("One element could not be created. Exiting.\n");
		return -1;
	}
	/* we set the input filename to the source element */
	// g_object_set (G_OBJECT (data.source), "location", argv[1], NULL);
	
	/* we add a message handler */
	bus = gst_pipeline_get_bus(GST_PIPELINE (data.pipeline));
	bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
	gst_object_unref(bus);
	
	/* we add all elements into the pipeline */
	/* file-source | ogg-demuxer | vorbis-decoder | converter | alsa-output */
	gst_bin_add_many(GST_BIN (data.pipeline), data.source, data.decoder, data.convert, data.sink, NULL);

	/* we link the elements together */
	/* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> alsa-output */
	gst_element_link (data.source, data.decoder);
	gst_element_link_many (data.convert, data.sink, NULL);
	g_signal_connect (data.decoder, "pad-added", G_CALLBACK (on_pad_added), data.convert);
	
	/* Set the pipeline to "playing" state*/
	
	// g_print ("Now playing: %s\n", argv[1]);
	// gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
	
	lpb_ctrl_init(8080);	
}


int main(int argc, char *argv[])
{
	printf("Start demo player.\n");	
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	return 0;
}
