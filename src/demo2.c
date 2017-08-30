#include <stdio.h>
#include <glib.h>
#include <gst/gst.h>


// DEMO: gst-launch-1.0 filesrc location=/home/qichunren/Music/ATB-9PM.mp3 ! decodebin ! audioconvert ! autoaudiosink

static GstElement *g_main_pipeline;
static GstElement *g_audioconvert_element;
static GstElement *g_playbin_element;
static const gchar *g_demo_mp3_file = "/home/qichunren/Music/ATB-9PM.mp3";

static gboolean timeout_cb_resume (gpointer user_data)
{
	GstElement * main_pipeline = (GstElement *)user_data;
	
	guint64 time_nanoseconds = 150 * GST_SECOND;
	if (!gst_element_seek (main_pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, time_nanoseconds, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE)) 
	{
		g_message("Seek failedx!");
	}
	
	// gst_element_set_state(main_pipeline, GST_STATE_NULL);
    gst_element_set_state(main_pipeline, GST_STATE_PLAYING);
	
	
	return FALSE;
}

static void resume_play()
{
	g_message("Play from 30 seconds position");
	// Delay 2 seconds to resume play.
	g_timeout_add_seconds(2, timeout_cb_resume, g_main_pipeline);
}

static gboolean timeout_cb_pause(gpointer user_data)
{
	GstElement * main_pipeline = (GstElement *)user_data;
    gint64 now;
    now = g_get_monotonic_time();
	g_message("Now %ld, PAUSE.", now);
	
	
	gint64 pos, len;

	if (gst_element_query_position (main_pipeline, GST_FORMAT_TIME, &pos) && gst_element_query_duration (main_pipeline, GST_FORMAT_TIME, &len)) {
		g_message("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT, GST_TIME_ARGS (pos), GST_TIME_ARGS (len));
	}
	
	gst_element_set_state(g_main_pipeline, GST_STATE_PAUSED);
	
	resume_play();
	
    return FALSE;
}


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
	GstElement *filesrc_element;
	GstElement *decodebin_element;

	GstBus *bus;
	GstMessage *msg;

	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	
	
	g_main_pipeline = gst_pipeline_new("DemoPipeline");
	filesrc_element = gst_element_factory_make("filesrc", NULL);
	decodebin_element = gst_element_factory_make("decodebin", NULL);
	g_audioconvert_element = gst_element_factory_make("audioconvert", NULL);
	g_playbin_element = gst_element_factory_make("autoaudiosink", NULL);
	
	g_object_set(filesrc_element, "location", g_demo_mp3_file, NULL);
	
	
	gst_bin_add_many(GST_BIN(g_main_pipeline), filesrc_element, decodebin_element, g_audioconvert_element, g_playbin_element , NULL);
	gst_element_link_many(filesrc_element, decodebin_element, NULL);
	
	g_signal_connect(decodebin_element, "pad-added", G_CALLBACK(decodebin_new_decoded_pad_cb), NULL);
	gst_element_link_many(g_audioconvert_element, g_playbin_element, NULL);

	g_message("Playing mp3 file:%s", g_demo_mp3_file);
	GMainLoop * g_lpcon_main_loop = g_main_loop_new(NULL, FALSE);
	
	
	gst_element_set_state(g_main_pipeline, GST_STATE_NULL);
    gst_element_set_state(g_main_pipeline, GST_STATE_PLAYING);
    
    g_timeout_add_seconds(5, timeout_cb_pause, g_main_pipeline);
	
	g_main_loop_run(g_lpcon_main_loop);
	return 0;
}
