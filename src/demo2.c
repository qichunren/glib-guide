#include <stdio.h>
#include <glib.h>
#include <gst/gst.h>


// DEMO: gst-launch-1.0 filesrc location=/home/qichunren/Music/ATB-9PM.mp3 ! decodebin ! audioconvert ! autoaudiosink

static GstElement *g_audioconvert_element;
static GstElement *g_playbin_element;
static const gchar *g_demo_mp3_file = "/home/qichunren/Music/ATB-9PM.mp3";


static void decodebin_new_decoded_pad_cb(GstElement *decodebin, 
    GstPad *pad, gpointer user_data)
{
    GstCaps *caps;
    GstStructure *structure;
    const gchar *mimetype;
    gboolean cancel = FALSE;
    GstPad *sink_pad;
    caps = gst_pad_query_caps(pad, NULL);
    
    /* we get "ANY" caps for text/plain files etc. */
    if(gst_caps_is_empty(caps) || gst_caps_is_any(caps))
    {
        g_warning("Decoded pad with no caps or any caps. "
            "This file is boring.");
        cancel = TRUE;
    }
    else
    {
        /* Is this pad audio? */
        structure = gst_caps_get_structure(caps, 0);
        mimetype = gst_structure_get_name(structure);
        if(g_str_has_prefix(mimetype, "audio/x-raw"))
        {
            g_debug("Got decoded audio pad of type %s", mimetype);
            sink_pad = gst_element_get_static_pad(g_audioconvert_element, "sink");
            gst_pad_link(pad, sink_pad);
            gst_object_unref(sink_pad);
        }
        else if(g_str_has_prefix(mimetype, "video/"))
        {
            g_debug("Got decoded video pad of type %s", mimetype);
        }
        else
        {
            g_debug("Got decoded pad of non-audio type %s", mimetype);
        }
    }

    gst_caps_unref(caps);
    /* If this is non-audio, cancel the operation.
     * This seems to cause some deadlocks with video files, so only do it
     * when we get no/any caps.
     */
}

int main(int argc, char *argv[])
{
	GstElement *main_pipeline;
	GstElement *filesrc_element;
	GstElement *decodebin_element;

	GstBus *bus;
	GstMessage *msg;

	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	
	
	main_pipeline = gst_pipeline_new("DemoPipeline");
	filesrc_element = gst_element_factory_make("filesrc", NULL);
	decodebin_element = gst_element_factory_make("decodebin", NULL);
	g_audioconvert_element = gst_element_factory_make("audioconvert", NULL);
	g_playbin_element = gst_element_factory_make("autoaudiosink", NULL);
	
	g_object_set(filesrc_element, "location", g_demo_mp3_file, NULL);
	
	
	gst_bin_add_many(GST_BIN(main_pipeline), filesrc_element, decodebin_element, g_audioconvert_element, g_playbin_element , NULL);
	gst_element_link_many(filesrc_element, decodebin_element, NULL);
	
	g_signal_connect(decodebin_element, "pad-added", G_CALLBACK(decodebin_new_decoded_pad_cb), NULL);
	gst_element_link_many(g_audioconvert_element, g_playbin_element, NULL);

	g_message("Playing mp3 file:%s", g_demo_mp3_file);
	GMainLoop * g_lpcon_main_loop = g_main_loop_new(NULL, FALSE);
	
	
	gst_element_set_state(main_pipeline, GST_STATE_NULL);
    gst_element_set_state(main_pipeline, GST_STATE_PLAYING);
	
	g_main_loop_run(g_lpcon_main_loop);
	return 0;
}
