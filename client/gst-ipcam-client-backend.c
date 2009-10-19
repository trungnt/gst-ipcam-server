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

#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

#include "gst-ipcam-client-backend.h"
#include "gst-ipcam-client-interface.h"
#include "gst-ipcam-client-callbacks.h"

static gpointer window;
static gchar *video_type;
static gchar *audio_type;
static GstElement *pipeline, *rtspsrc, *videosink;;
static GstElement *video_decoder = NULL;
static GstElement *audio_decoder = NULL;
static GstElement * video_tee, * audio_tee;
static GstElement * audio_branch;
static GstPad * audio_pad;
static gint prewState;
static gint curtState;

/**
 * Get the string representation for GstStreamStatusType
 *
 * @param type GstStreamStatusType the type we want to get string form
 *
 * @return string form of given value
 */
static const gchar * gst_ipcam_client_backend_stream_status_get_name(GstStreamStatusType type);

/**
 * Create the video branch for the pipeline
 *
 * @param pipeline GstElement* the pipeline to insert elements to
 *
 * @return gboolean TRUE if everything is ok, FALSE if something wrong (can create an element, ... )
 */
static gboolean gst_ipcam_client_backend_create_video_branch(GstElement * pipeline);

/**
 * Create the audio branch for the pipeline
 *
 * @param pipeline GstElement* the pipeline to insert elements to
 *
 * @return gboolean TRUE if everything is ok, FALSE if something wrong (can create an element, ... )
 */
static gboolean gst_ipcam_client_backend_create_audio_branch(GstElement * pipeline);

/**
 * callback when the video_decoder/audio_decoder create new src pad.
 *
 * @param decoder GstElement* the decoder that emit the signal
 * @param pad GstPad* newly added pad by the decoder
 * @param last gboolean unused
 * @param next_element gpointer pointer to the next element to link the decoder to
 *
 * @return None
 */
static void gst_ipcam_client_backend_decoder_on_pad_add(GstElement * decoder, GstPad * pad, gboolean last, gpointer next_element);

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

	video_type = NULL;
	audio_type = NULL;
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
				g_debug("We are using '%s'", GST_PLUGIN_FEATURE(f)->name);
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
			gst_ipcam_client_backend_print_gst_message(errors->data);
			gst_message_unref(GST_MESSAGE(errors->data));
		} else {
			/* send warning message to application and use a fakesink */
			g_warning("Failed to find a usable video sink");
			choice = sink; //gst_element_factory_make("fakesink", "fake-video-sink");
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
			if ((prewState == GST_PAUSE_STATE) && (curtState == GST_PLAYING_STATE)) {
				gst_ipcam_client_backend_stop();
				pipeline = NULL;
				gst_ipcam_client_backend_create_pipeline((gchar *) data);
				gst_ipcam_client_backend_play();

				g_message("PLAY request sent.");

				/*Resize the mainwindow to show the video got from server*/
				gtk_window_resize(GTK_WINDOW(mainWindow), 420, 400);
			} else {
				g_message("PLAY request could not be sent.");

				GtkWidget *dialog;
				dialog = gtk_message_dialog_new(NULL,
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_CLOSE,
						"The connection is failed. Please try again");

				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);

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
					gst_ipcam_client_set_status_Video_Type(video_type);
					gst_ipcam_client_set_status_Audio_Type(audio_type);
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
			gst_ipcam_client_backend_print_gst_message(msg);
			break;
	}
	return TRUE;
}

static void gst_ipcam_client_backend_print_gst_message(GstMessage* message) {
	switch (GST_MESSAGE_TYPE(message)) {
		case GST_MESSAGE_ERROR:
		{
			gchar * debug;
			GError * error;
			gst_message_parse_error(message, &error, &debug);

			if (debug != NULL) {
				g_debug("Error Debug: %s", debug);
				g_free(debug);
			}

			if (error != NULL) {
				g_warning("Backend error message (%s): %s", GST_MESSAGE_TYPE_NAME(message), error->message);
				g_error_free(error);
			}
		}
			break;
		case GST_MESSAGE_INFO:
		{
			gchar * debug;
			GError * error;
			gst_message_parse_info(message, &error, &debug);

			if (debug != NULL) {
				g_debug("Info Debug: %s", debug);
				g_free(debug);
			}

			if (error != NULL) {
				g_warning("Backend info message (%s): %s", GST_MESSAGE_TYPE_NAME(message), error->message);
				g_error_free(error);
			}
		}
			break;
		case GST_MESSAGE_WARNING:
		{
			gchar * debug;
			GError * error;
			gst_message_parse_warning(message, &error, &debug);

			if (debug != NULL) {
				g_debug("Warning Debug: %s", debug);
				g_free(debug);
			}

			if (error != NULL) {
				g_warning("Backend warning message (%s): %s", GST_MESSAGE_TYPE_NAME(message), error->message);
				g_error_free(error);
			}
		}
			break;
		case GST_MESSAGE_STREAM_STATUS:
		{
			GstElement * owner;
			GstStreamStatusType type;
			gst_message_parse_stream_status(message, &type, &owner);

			g_message("Got status %s from element %s", gst_ipcam_client_backend_stream_status_get_name(type), gst_element_get_name(owner));
		}
			break;
		default:
			g_message("got the message of type %s", gst_message_type_get_name(GST_MESSAGE_TYPE(message)));
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
void gst_ipcam_client_on_pad_added(GstElement *element, GstPad *pad) {
	g_debug("Signal: pad-added");
	GstCaps *caps;
	GstStructure *str;
	caps = gst_pad_get_caps(pad);
	g_assert(caps != NULL);

	str = gst_caps_get_structure(caps, 0);
	g_assert(str != NULL);

	/* get the media type, which will show us this's video or audio stream */
	gchar *media_type = g_strdup(gst_structure_get_string(str, "media"));
	/* get the stream type, which will show us the codec of the stream */
	gchar *stream_type = g_strdup(gst_structure_get_string(str, "encoding-name"));

	g_message("Stream type : %s\n", stream_type);
	{
		/* preprocess stream type */
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
	gst_caps_unref(caps);
	g_message("Pad name %s\n", media_type);

	if (g_strrstr(media_type, "video") != NULL) {
		g_warning("New video channel");

		g_free(media_type);

		if (video_type != NULL) {
			g_free(video_type);
		}
		video_type = g_strdup_printf("Video type: %s", stream_type);
		g_free(stream_type);

                gst_element_link(rtspsrc, video_tee);
	} else if (g_strrstr(media_type, "audio") != NULL) {
		g_warning("New audio channel");
		g_free(media_type);

		if (audio_type != NULL) {
			g_free(audio_type);
		}
		audio_type = g_strdup_printf("/Audio type: %s", stream_type);
		g_free(stream_type);

                gst_ipcam_client_backend_create_audio_branch(pipeline);
                
                gst_element_link(rtspsrc, audio_tee);

		/* set state to play to make sure audio branch will run */
		gst_element_set_state(audio_branch, GST_STATE_PLAYING);
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
gst_ipcam_client_backend_create_pipeline(const gchar *url) {
	GstBus *bus;
	/* create elements */
	pipeline = gst_pipeline_new("pipeline");
	/* watch for messages on the pipeline's bus (note that this will only
	 * work like this when a GLib main loop is running) */
	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	gst_bus_add_watch(bus, gst_ipcam_client_backend_bus_watch, g_strdup(url));
	gst_object_unref(bus);

	rtspsrc = gst_element_factory_make("rtspsrc", "rtspsource");

	g_object_set(G_OBJECT(rtspsrc), "location", url, NULL);
	/*g_object_set(G_OBJECT(rtspsrc), "debug", TRUE, NULL);*/
	g_signal_connect(rtspsrc, "pad-added", G_CALLBACK(gst_ipcam_client_on_pad_added), NULL);

	gst_bin_add_many(GST_BIN(pipeline), rtspsrc, NULL);

	gst_ipcam_client_backend_create_video_branch(pipeline);
}

/**
 * Read the video properties
 *
 * @param videosink GstElement *
 *
 * @return nothing
 */
void
gst_ipcam_client_read_video_props(GstElement *videosink) {
	return;
	gint fps_n, fps_d;
	gint width, height;
	gint datarate;
	gchar *capsInfor;
	GstStructure *str = NULL;
	GstPad *pad;
	GstCaps *caps;

	pad = gst_element_get_static_pad(videosink, "sink");
	if (pad == NULL) {
		return;
	}
	caps = gst_pad_get_negotiated_caps(pad);

	g_return_if_fail(gst_caps_is_fixed(caps));
	str = gst_caps_get_structure(caps, 0);

	if (str == NULL) {
		return;
	}

	capsInfor = gst_caps_to_string(caps);
	g_message("caps Infor: %s", capsInfor);

	gst_structure_get_fraction(str, "framerate", &fps_n, &fps_d);
	gst_structure_get_int(str, "width", &width);
	gst_structure_get_int(str, "height", &height);

	gst_structure_get_int(str, "datarate", &datarate);

	g_message("The datarate is : %d\n", datarate);

	g_message("frame rate %d/%d", fps_n, fps_d);
	g_message("The video size of this set of capabilities is %dx%d",
			width, height);
}

static gboolean gst_ipcam_client_backend_create_video_branch(GstElement* pipeline) {
	GstElement * video_branch = gst_parse_bin_from_description("tee name=video_tee ! queue name=video_queue "
                                                      "! decodebin ! ffmpegcolorspace ! identity name = connector", FALSE, NULL);
        video_tee = gst_bin_get_by_name(video_branch, "video_tee");

        GstElement * connector = gst_bin_get_by_name(GST_BIN(video_branch), "connector");
        g_assert(connector);

        videosink = gst_ipcam_client_backend_find_best_video_sink();
        g_assert(videosink);

        gst_bin_add_many(GST_BIN(pipeline), video_branch, videosink, NULL);
        gst_element_link(connector, videosink);

        g_object_set(G_OBJECT(videosink), "force-aspect-ratio", TRUE, NULL);
        if (GST_IS_X_OVERLAY(videosink)) {
            gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(videosink), GPOINTER_TO_INT(window));
	}

	return TRUE;
}

static void gst_ipcam_client_backend_decoder_on_pad_add(GstElement* decoder, GstPad* pad, gboolean last, gpointer data) {
	GstElement * next_element = GST_ELEMENT(data);
	GstPad * next_sink;

	/* for now, we do not check media type for our pad
	 * do not really need to check though
	 */
	next_sink = gst_element_get_static_pad(next_element, "sink");
	if (GST_PAD_IS_LINKED(next_sink)) {
		gst_object_unref(next_sink);
		return;
	}

	gst_pad_link(pad, next_sink);
}

static gboolean gst_ipcam_client_backend_create_audio_branch(GstElement* pipeline) {
	audio_branch = gst_parse_bin_from_description("tee name=audio_tee ! queue name=audio_queue !"
                                        " decodebin ! audioconvert ! autoaudiosink", FALSE, NULL);
        audio_tee = gst_bin_get_by_name(audio_branch, "audio_tee");
        gst_element_set_state(audio_branch, GST_STATE_PAUSED);

        
        gst_bin_add_many(GST_BIN(pipeline), audio_branch, NULL);
	return TRUE;
}

static const gchar * gst_ipcam_client_backend_stream_status_get_name(GstStreamStatusType type) {
	switch (type) {
		case GST_STREAM_STATUS_TYPE_CREATE:
			return "GST_STREAM_STATUS_TYPE_CREATE";
		case GST_STREAM_STATUS_TYPE_ENTER:
			return "GST_STREAM_STATUS_TYPE_ENTER";
		case GST_STREAM_STATUS_TYPE_LEAVE:
			return "GST_STREAM_STATUS_TYPE_LEAVE";
		case GST_STREAM_STATUS_TYPE_DESTROY:
			return "GST_STREAM_STATUS_TYPE_DESTROY";
		case GST_STREAM_STATUS_TYPE_START:
			return "GST_STREAM_STATUS_TYPE_START";
		case GST_STREAM_STATUS_TYPE_PAUSE:
			return "GST_STREAM_STATUS_TYPE_PAUSE";
		case GST_STREAM_STATUS_TYPE_STOP:
			return "GST_STREAM_STATUS_TYPE_STOP";
	}
	return "Unknow Stream status type";
}
