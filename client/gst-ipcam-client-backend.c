/**
 * \file:   gst-ipcam-client-backend.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 * \author: Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com>
 *
 * \date 8-26-2009
 */
#include <string.h>

#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>
#include <gstreamer-0.10/gst/gststructure.h>
#include <gstreamer-0.10/gst/gstutils.h>

#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

#include "gst-ipcam-client-backend.h"
#include "gst-ipcam-client-interface.h"
#include "gst-ipcam-client-callbacks.h"

static gpointer window;
static gchar *videoType;
static gchar *audioType;
static GstElement *pipeline, *rtspsrc,
		*v_depay_jpg, *v_decoder_jpg, *v_filter_jpg, *videosink,
		*v_depay_h264, *v_decoder_h264, *v_filter_h264,
		*v_depay_mp4, *v_decoder_mp4, *v_filter_mp4,
		*a_depay_aac, *a_decoder_aac, *a_filter_aac, *a_resample_aac, *a_sink_aac,
		*a_depay_g726, *a_decoder_g726, *a_filter_g726, *a_resample_g726, *a_sink_g726,
		*a_depay_g711, *a_decoder_g711, *a_filter_g711, *a_resample_g711, *a_sink_g711;
static GstElement *video_decoder = NULL;
static GstElement *audio_decoder = NULL;
static gint prewState;
static gint curtState;

//static gboolean gst_ipcam_client_create_video_H264_branch(GstElement * pipeline);
//static gboolean gst_ipcam_client_create_video_MPEG4_branch(GstElement * pipeline);
static gboolean gst_ipcam_client_backend_create_video_branch(GstElement * pipeline);
//static gboolean gst_ipcam_client_create_audio_AAC_branch(GstElement * pipeline);
static gboolean gst_ipcam_client_backend_create_audio_branch(GstElement * pipeline);

static void gst_ipcam_client_backend_decoder_on_pad_add(GstElement * decoder, GstPad * pad, gboolean last, gpointer next_element );

/**
 * This function will try to find the best suitable video sink used for current window system.
 *
 * @return GstElement* a video sink that can be worked for current window system
 */
static GstElement * gst_ipcam_client_backend_find_best_video_sink();

/**
 * Compare ranks of 2 plugin features to sort the list of features.
 * Copied from GstAutoVideoSink source (gst-plugins-good-0.10.15)
 *
 * @param f1 GstPluginFeature* the first feature
 * @param f2 GstPluginFeature* the 2nd feature
 *
 * @return gint 0 if they are equal, -1 if f1 is less than f2 and 1 if f1 is greater
 */
static gint gst_ipcam_client_backend_video_sink_compare_ranks(GstPluginFeature * f1, GstPluginFeature * f2);

/**
 * Check if we should list given feature in to search list
 * Copied from GstAutoVideoSink source (gst-plugins-good-0.10.15)
 *
 * @param feature GstPluginFeature* feature to check
 * @param data gpointer userdata to callback function - unused
 *
 * @return gboolean TRUE if should/ FALSE if not
 */
static gboolean gst_ipcam_client_backend_video_sink_factory_filter(GstPluginFeature * feature, gpointer data);

/**
 * Create a sink with given factory
 *
 * @param factory GstElementFactory the factory to create element
 *
 * @return GstElement* an element created with given factory
 */
static GstElement * gst_ipcam_client_backend_create_sink(GstElementFactory * factory);

/**
 * handle messages from pipeline bus.
 *
 * @param bus GstBus* the bus that contains the messages
 * @param msg GstMessage* current bus message
 * @param data gpointer addition user data
 *
 * @return gboolean will comment on this later
 */
static gboolean gst_ipcam_client_backend_bus_watch(GstBus * bus, GstMessage * msg, gpointer data);

/**
 * Parse and print gstreamer message in the case that it's info/warning or error message.
 *
 * @param message GstMessage* the message that we need to parse and print
 *
 * @return Nothing
 */
static void gst_ipcam_client_backend_print_gst_message(GstMessage * message);

/**
 * init for using gstreamer
 * This function is used whenever the main function is created
 *
 * @param argc int *
 * @param argv[] char **
 *
 * @return nothing
 */
void
gst_ipcam_client_backend_init(int *argc,
		char **argv[]) {
	gst_init(argc, argv);
	pipeline = NULL;
}

/**
 * create the pipeline to get the data from the rtsp server
 *
 * @param pipeline_description const gchar * the pipeline description
 *
 * @return nothing
 */
/*
void
gst_ipcam_client_backend_create_pipeline(const gchar *url) {
	//GstElement * connector;
	GstElement *videosink;
	g_message("SETUP request is sending...");
	g_debug("the pipeline is :%s", url);

	//pipeline = gst_parse_launch(pipeline_description, NULL);
        pipeline = gst_element_factory_make ("playbin", "client");
        g_object_set (G_OBJECT (pipeline), "uri", url, NULL);
	g_message("SETUP request sent.");

	videosink = gst_ipcam_client_backend_find_best_video_sink();
        //videosink = gst_element_factory_make("xvimagesink", "videosink");
	//connector = gst_bin_get_by_name(GST_BIN(pipeline), "connector");
        //playbin = gst_bin_get_by_name(GST_BIN(pipeline), "playbin0");
	//gst_bin_add(GST_BIN(pipeline), videosink);
	//gst_element_link(connector, videosink);
        g_object_set(G_OBJECT(videosink), "force-aspect-ratio", TRUE, NULL);
        g_object_set(G_OBJECT(pipeline), "video-sink", videosink, NULL);

	// set the bus message handling function
	{
		GstBus * bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
                gst_bus_add_watch(bus, gst_ipcam_client_backend_bus_watch, url);
		gst_object_unref(bus);
	}

	if (GST_IS_X_OVERLAY(videosink)) {
		gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(videosink), GPOINTER_TO_INT(window));
	}
}*/

/**
 * Set the pointer window
 *
 * @param window_ gpointer the pointer to show video region in the main window
 *
 * @return nothing
 */
void
gst_ipcam_client_backend_set_window(gpointer window_) {
	window = window_;
}

/**
 * set the pipeline from the state GST_STATE_NULL to the state GST_STATE_PLAYING
 *
 * @param nothing
 *
 * @return Result of the state change
 */
gint
gst_ipcam_client_backend_play() {
        prewState = GST_STOP_STATE;
        curtState = GST_PLAYING_STATE;
        GstStateChangeReturn stateReturn;

	stateReturn = gst_element_set_state(pipeline, GST_STATE_PLAYING);
	g_message("Setting to Play.....Done");
        
	return stateReturn;
}


//static GList *
void
get_stream_info_objects_for_type ()//(GstElement *play)//, const gchar *typestr)
{
        GValueArray *info_arr = NULL;
        GstElement *source;
        GstElementFactory *factory;
        //GList *ret = NULL;
        guint ii;

        if (pipeline == NULL)
                return ;
        g_object_get(G_OBJECT(pipeline), "stream-info-value-array", &info_arr, NULL);
        g_object_get(G_OBJECT(pipeline), "source", &source, NULL);

        g_message("source name %s", gst_element_get_name(source));
        factory = gst_element_get_factory(source);
        g_message("factory description %s", gst_element_factory_get_description(factory));
        if (info_arr == NULL)
                return ;

        g_message (" get info %d",info_arr->n_values);
        for (ii = 0; ii < info_arr->n_values; ++ii)
        {
                GObject *info_obj;
                GValue *val;

                val = g_value_array_get_nth(info_arr, ii);
                info_obj = g_value_get_object(val);
                if (info_obj)
                {
                        //GParamSpec *pspec;
                        //GEnumValue *value;
                        GstCaps *caps;
                        GstPad * pad;
                        gint type = -1;
                        gchar *codec;

                        g_object_get(info_obj, "type", &type, NULL);
                        g_object_get(info_obj, "object", &pad, NULL);
                        g_object_get(info_obj, "encoding-name", &codec, NULL);
                        g_message("codec %s",codec);
                        g_message("type %d",type);
                        GstStructure *ss=NULL;

                        caps = gst_pad_get_negotiated_caps(pad);

                        codec = gst_caps_to_string (caps);

                        g_message ("codec: %s",codec);
                        ss = gst_caps_get_structure(caps, 0);
                        if (ss)
                        {
                            gint fps_n, fps_d, width, height;


                            gst_structure_get_fraction(ss, "framerate", &fps_n, &fps_d);
                            gst_structure_get_int(ss, "width", &width);
                            gst_structure_get_int(ss, "height", &height);
                            //codec = gst_structure_get_string(ss, "video-codec");

                            g_message ("frame rate %d/%d",fps_n,fps_d);
                            //g_message ("codec: %s",codec);
                        }
                        /*pspec = g_object_class_find_property(
                                                G_OBJECT_GET_CLASS (info_obj), "type");
                        value = g_enum_get_value(
                                                G_PARAM_SPEC_ENUM (pspec)->enum_class, type);
                        if (value)
                        {
                                if (g_ascii_strcasecmp (value->value_nick, typestr) == 0 ||
                                        g_ascii_strcasecmp (value->value_name, typestr) == 0)
                                {
                                        ret = g_list_prepend (ret, g_object_ref (info_obj));
                                }
                        }*/
                }
        } 
        g_value_array_free (info_arr);
        //return g_list_reverse (ret);
}

/**
 * set the pipeline from the state GST_STATE_PLAYING to the state GST_STATE_PAUSED
 *
 * @param nothing
 *
 * @return Result of the state change
 */
gint
gst_ipcam_client_backend_pause() {
        prewState = GST_PLAYING_STATE;
        curtState = GST_PAUSE_STATE;
        
	GstStateChangeReturn stateReturn;

	stateReturn = gst_element_set_state(pipeline, GST_STATE_PAUSED);
	g_message("Setting to Pause.....Done");

	return stateReturn;
}

/**
 * set the pipeline from the state GST_STATE_PLAYING to the state GST_STATE_NULL
 *
 * @param nothing
 *
 * @return Result of the state change
 */
gint
gst_ipcam_client_backend_stop() {
        prewState = GST_PLAYING_STATE;
        curtState = GST_STOP_STATE;
        
	gtk_window_resize(GTK_WINDOW(mainWindow), 420, 50);
	GstStateChangeReturn stateReturn;

	stateReturn = gst_element_set_state(pipeline, GST_STATE_NULL);
	g_message("Setting to Stop.....Done");

        videoType = NULL;
        audioType = NULL;
	return stateReturn;
}

/**
 * set the pipeline from the state GST_STATE_PAUSED to the state GST_STATE_PLAYING
 *
 * @param nothing
 *
 * @return Result of the state change
 */
gint
gst_ipcam_client_backend_resume() {
        prewState = GST_PAUSE_STATE;
        curtState = GST_PLAYING_STATE;
        
	GstStateChangeReturn stateReturn;

	stateReturn = gst_element_set_state(pipeline, GST_STATE_PLAYING);
	g_message("Setting to Resume.....Done");

	return stateReturn;
}

/**
 * free memories for pipeline and videosink pointer before exit the application
 *
 * @param nothing
 *
 * @return nothing
 */
void
gst_ipcam_client_backend_deinit() {
	if (pipeline != NULL) {
		gst_object_unref(pipeline);
		pipeline = NULL;
	}
}

static GstElement * gst_ipcam_client_backend_find_best_video_sink() {
	GList *list, *item;
	GstElement *choice = NULL;
//	GstMessage *message = NULL;
	GSList *errors = NULL;
	GstBus *bus = gst_bus_new();

	GstElement * sink = gst_element_factory_make("fakesink", "fake-video-sink");

	list = gst_registry_feature_filter(gst_registry_get_default(),
			(GstPluginFeatureFilter) gst_ipcam_client_backend_video_sink_factory_filter, FALSE, sink);
	list = g_list_sort(list, (GCompareFunc) gst_ipcam_client_backend_video_sink_compare_ranks);

	for (item = list; item != NULL; item = item->next) {
		GstElementFactory *f = GST_ELEMENT_FACTORY(item->data);
		GstElement *el;

		if ((el = gst_ipcam_client_backend_create_sink(f))) {
			GstStateChangeReturn ret;

			gst_element_set_bus(el, bus);
			ret = gst_element_set_state(el, GST_STATE_READY);
			if (ret == GST_STATE_CHANGE_SUCCESS) {
				choice = el;
				g_debug("We are using '%s'", GST_PLUGIN_FEATURE (f)->name);
				break;
			}

			/* collect all error messages */
/*
			while ((message = gst_bus_pop_filtered(bus, GST_MESSAGE_ERROR))) {
				g_warning("error message %" GST_PTR_FORMAT, message);
				errors = g_slist_append(errors, message);
			}
*/

			gst_element_set_state(el, GST_STATE_NULL);
			gst_object_unref(el);
		}
	}

	if (!choice) {
		if (errors) {
			/* FIXME: we forward the first error for now; but later on it might make
			 * sense to actually analyse them */
			gst_message_ref(GST_MESSAGE(errors->data));
			//g_warning("reposting message %s", errors->data);
			gst_ipcam_client_backend_print_gst_message(errors->data);
			gst_message_unref(GST_MESSAGE(errors->data));
		} else {
			/* send warning message to application and use a fakesink */
			g_warning("Failed to find a usable video sink");
			choice = sink;//gst_element_factory_make("fakesink", "fake-video-sink");
			if (g_object_class_find_property(G_OBJECT_GET_CLASS(choice), "sync"))
				g_object_set(choice, "sync", TRUE, NULL);
			gst_element_set_state(choice, GST_STATE_READY);
		}
	}
	gst_object_unref(bus);
	gst_plugin_feature_list_free(list);
	g_slist_foreach(errors, (GFunc) gst_mini_object_unref, NULL);
	g_slist_free(errors);

	return choice;
}

static gint gst_ipcam_client_backend_video_sink_compare_ranks(GstPluginFeature * f1, GstPluginFeature * f2) {
	gint diff;

	diff = gst_plugin_feature_get_rank(f2) - gst_plugin_feature_get_rank(f1);
	if (diff != 0)
		return diff;
	return strcmp(gst_plugin_feature_get_name(f2),
			gst_plugin_feature_get_name(f1));
}

static gboolean gst_ipcam_client_backend_video_sink_factory_filter(GstPluginFeature * feature, gpointer data) {
	guint rank;
	const gchar *klass;

	/* we only care about element factories */
	if (!GST_IS_ELEMENT_FACTORY(feature))
		return FALSE;

	/* video sinks */
	klass = gst_element_factory_get_klass(GST_ELEMENT_FACTORY(feature));
	if (!(strstr(klass, "Sink") && strstr(klass, "Video")))
		return FALSE;

	/* only select elements with autoplugging rank */
	rank = gst_plugin_feature_get_rank(feature);
	if (rank < GST_RANK_MARGINAL)
		return FALSE;

	return TRUE;
}

static GstElement * gst_ipcam_client_backend_create_sink(GstElementFactory* factory) {
	GstElement * sink;
	sink = gst_element_factory_create(factory, "video-sink");
	return sink;
}

static gboolean gst_ipcam_client_backend_bus_watch(GstBus* bus, GstMessage* msg, gpointer data) {
        switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
            {
		gchar * debug;
		GError * error;
                gst_message_parse_error(msg, &error, &debug);

                if ((prewState == GST_PAUSE_STATE) && (curtState == GST_PLAYING_STATE))
                {
                    gst_ipcam_client_backend_stop();
                    pipeline = NULL;
                    gst_ipcam_client_backend_create_pipeline((gchar *)data);
                    gst_ipcam_client_backend_play();

                    g_message("PLAY request sent.");

                    /*Resize the mainwindow to show the video got from server*/
                    gtk_window_resize(GTK_WINDOW(mainWindow), 420, 400);
                }
                else
                {
                    g_message("PLAY request could not be sent.");
                
                    GtkWidget *dialog;
                    dialog = gtk_message_dialog_new(NULL,
                                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    GTK_MESSAGE_ERROR,
                                                    GTK_BUTTONS_CLOSE,
                                                    "The connection is failed. Please try again");

                    gtk_dialog_run (GTK_DIALOG (dialog));
                    gtk_widget_destroy (dialog);

                    gst_ipcam_client_on_btn_Disconnect_clicked(NULL, NULL);
                    g_free(debug);

                    g_warning("Pipeline error: %s", error->message);

                    g_error_free(error);
                
                }
            }
            break;
            case GST_MESSAGE_STATE_CHANGED:
            {
                GstState oldState, newState, pending;
                gst_message_parse_state_changed(msg, &oldState, &newState, &pending);
                switch (newState) {
                    case GST_STATE_PLAYING:
                        if (videosink != NULL)
                            gst_ipcam_client_read_video_props(videosink);
                        gst_ipcam_client_set_status_text("Playing");
                        gst_ipcam_client_set_status_Video_Type(videoType);
                        gst_ipcam_client_set_status_Audio_Type(audioType);
                        break;
                    case GST_STATE_PAUSED:
                        gst_ipcam_client_set_status_text("Paused");
                        break;
                    case GST_STATE_NULL:
                        gst_ipcam_client_set_status_text("Stopped");
                        break;
                    case GST_STATE_READY:
                        break;
                    default:
                        g_message("Unknow state : %d!", newState);
                        gst_ipcam_client_set_status_text("Stopped");
                        break;
                }
                break;
            }
                break;
            default:
		break;
        }
	return TRUE;
}

static void gst_ipcam_client_backend_print_gst_message(GstMessage* message) {
	switch (GST_MESSAGE_TYPE(message)) {
		case GST_MESSAGE_ERROR:
		case GST_MESSAGE_INFO:
		case GST_MESSAGE_WARNING:
		{
			gchar * debug;
			GError * error;
			gst_message_parse_error(message, &error, &debug);

			if (debug != NULL) {
				g_free(debug);
			}

			if (error != NULL) {
				g_warning("Backend message (%s): %s", GST_MESSAGE_TYPE_NAME(message), error->message);
				g_error_free(error);
			}
		}
			break;
		default:
			break;
	}
}

/**
 * Handle the event when the signal pad-added is called
 *
 * @param element GstElement *
 * @param pad GstPad *
 *
 * @return nothing
 */
void gst_ipcam_client_on_pad_added (GstElement *element, GstPad *pad)
{
    g_debug ("Signal: pad-added");
    GstCaps *caps;
    GstStructure *str;
    caps = gst_pad_get_caps (pad);
    g_assert (caps != NULL);
    str = gst_caps_get_structure (caps, 0);
    g_assert (str != NULL);

	g_warning("The structure I got: %s", gst_structure_to_string(str));

    const gchar *c = gst_structure_get_string(str, "media");
    gchar *stream_type = g_strdup(gst_structure_get_string(str, "encoding-name"));
    
    g_message("Stream type : %s\n", stream_type);
	{
		/* preprocess audio type */
		if (g_strcmp0(stream_type, "MP4V-ES") == 0) {
			g_free(stream_type);
			stream_type = g_strdup("MPEG4");
		} else if (g_strcmp0(stream_type, "MP4A-LATM") == 0) {
			g_free(stream_type);
			stream_type = g_strdup("AAC");
		} else if (g_strcmp0(stream_type, "G726-16") == 0) {
			g_free(stream_type);
			stream_type = g_strdup("G726");
		} else if (g_strcmp0(stream_type, "PCMA") == 0) {
			g_free(stream_type);
			stream_type = g_strdup("G711");
		}
	}
    g_message("Pad name %s\n", c);
	if (g_strrstr(c, "video") != NULL) {
		// got video stream
		if (videoType != NULL) {
			g_free(videoType);
		}
		videoType = g_strdup_printf("Video type: %s", stream_type);
		GstPad * video_decoder_sink_pad = gst_element_get_static_pad(video_decoder, "sink");
		if (GST_PAD_IS_LINKED(video_decoder_sink_pad)) {
			gst_object_unref(video_decoder_sink_pad);
			return;
		}

		gst_pad_link(pad, video_decoder_sink_pad);
		gst_caps_unref(caps);

		if (GST_IS_X_OVERLAY(videosink)) {
			gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(videosink), GPOINTER_TO_INT(window));
		}
	} else if (g_strrstr(c, "audio") != NULL) {
		// got audio stream
		if (audioType != NULL) {
			g_free(audioType);
		}
		audioType = g_strdup_printf("/Audio type: %s", stream_type);

		GstPad * audio_decoder_sink_pad = gst_element_get_static_pad(audio_decoder, "sink");
		if (GST_PAD_IS_LINKED(audio_decoder_sink_pad)) {
			gst_object_unref(audio_decoder_sink_pad);
			return;
		}

		gst_pad_link(pad, audio_decoder_sink_pad);
		gst_caps_unref(caps);
	}
}

/**
 * create the pipeline to get the data from the rtsp server
 *
 * @param url const gchar * the url of server
 *
 * @return nothing
 */
void
gst_ipcam_client_backend_create_pipeline(const gchar *url)
{
    GstBus *bus;

    /* create elements */
    pipeline = gst_pipeline_new ("pipeline");
    /* watch for messages on the pipeline's bus (note that this will only
    * work like this when a GLib main loop is running) */
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, gst_ipcam_client_backend_bus_watch, url);
    gst_object_unref (bus);
    rtspsrc = gst_element_factory_make ("rtspsrc", "rtspsource");

    g_object_set (G_OBJECT (rtspsrc), "location", url, NULL);
    //g_object_set (G_OBJECT (rtspsrc), "debug", TRUE, NULL);
    g_signal_connect (rtspsrc, "pad-added", G_CALLBACK (gst_ipcam_client_on_pad_added), NULL);

	gst_bin_add_many (GST_BIN (pipeline), rtspsrc, NULL);
	gst_ipcam_client_backend_create_video_branch(pipeline);
	gst_ipcam_client_backend_create_audio_branch(pipeline);
}

/**
 * Read the video properties
 *
 * @param caps GstPad *
 *
 * @return nothing
 */
void
gst_ipcam_client_read_video_props (GstElement *videosink)
{
    gint fps_n, fps_d;
    gint width, height;
    GstStructure *str = NULL;
    GstPad *pad;
    GstCaps *caps;

    pad = gst_element_get_static_pad(videosink, "sink");
    if (pad == NULL)
    {
        return;
    }
    caps = gst_pad_get_negotiated_caps (pad);

    g_return_if_fail (gst_caps_is_fixed (caps));
    str = gst_caps_get_structure (caps, 0);

    gst_structure_get_fraction(str, "framerate", &fps_n, &fps_d);
    gst_structure_get_int(str, "width", &width);
    gst_structure_get_int(str, "height", &height);

    g_message ("frame rate %d/%d", fps_n, fps_d);
    g_message ("The video size of this set of capabilities is %dx%d",
    width, height);
}

static gboolean gst_ipcam_client_backend_create_video_branch(GstElement* pipeline) {
	//GstElement * video_decoder;
	GstElement * video_queue;
	GstElement * video_converter;
	//GstElement * videosink;
	g_return_val_if_fail(pipeline != NULL, FALSE);
	video_decoder = gst_element_factory_make("decodebin", "video_decoder");
	g_assert(video_decoder);
	video_queue = gst_element_factory_make("queue", "video_queue");
	g_assert(video_queue);
	video_converter = gst_element_factory_make("ffmpegcolorspace", "video_converter");
	g_assert(video_converter);
	videosink = gst_ipcam_client_backend_find_best_video_sink();
	g_assert(videosink);
	g_object_set(G_OBJECT(videosink), "force-aspect-ratio", TRUE, NULL);

	gst_bin_add_many(GST_BIN(pipeline), video_decoder, video_queue, video_converter, videosink, NULL);
	gst_element_link_many(video_queue, video_converter, videosink, NULL);
	g_signal_connect(video_decoder, "new-decoded-pad", G_CALLBACK(gst_ipcam_client_backend_decoder_on_pad_add), video_queue);

	return TRUE;
}

static void gst_ipcam_client_backend_decoder_on_pad_add(GstElement* decoder, GstPad* pad, gboolean last, gpointer data) {
	g_assert(decoder);
	GstElement * next_element = GST_ELEMENT(data);

	GstPad * next_sink;

	g_debug("******** new decoded pad **********");

	next_sink = gst_element_get_static_pad(next_element, "sink");
	if (GST_PAD_IS_LINKED(next_sink)) {
		gst_object_unref(next_sink);
		return;
	}

	gst_pad_link(pad, next_sink);
}

static gboolean gst_ipcam_client_backend_create_audio_branch(GstElement* pipeline) {
	GstElement * audio_queue;
	GstElement * audio_converter;
	GstElement * audio_sink;
	
	g_return_val_if_fail(pipeline != NULL, FALSE);
	audio_decoder = gst_element_factory_make("decodebin", "audio_decoder");
	g_assert(audio_decoder);
	audio_queue = gst_element_factory_make("queue", "audio_queue");
	g_assert(audio_queue);
	audio_converter = gst_element_factory_make("audioconvert", "audio_converter");
	g_assert(audio_converter);
	audio_sink = gst_element_factory_make("autoaudiosink", "audio_sink");
	g_assert(audio_sink);

	gst_bin_add_many(GST_BIN(pipeline), audio_decoder, audio_queue, audio_converter, audio_sink, NULL);
	gst_element_link_many(audio_converter, audio_queue, audio_sink, NULL);
	g_signal_connect(audio_decoder, "new-decoded-pad", G_CALLBACK(gst_ipcam_client_backend_decoder_on_pad_add), audio_converter);

	return TRUE;
}