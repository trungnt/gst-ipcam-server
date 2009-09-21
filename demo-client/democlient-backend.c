/**
 * \file:   democlient-backend.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 * \author: Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com>
 *
 * \date 8-26-2009
 */
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>
#include <gstreamer-0.10/gst/gstmessage.h>

#include "democlient-backend.h"
#include "democlient-interface.h"
#include "democlient-callbacks.h"

static gpointer window;
static GstElement *pipeline;

/**
 * This function will try to find the best suitable video sink used for current window system.
 *
 * @return GstElement* a video sink that can be worked for current window system
 */
static GstElement * democlient_backend_find_best_video_sink();

/**
 * Compare ranks of 2 plugin features to sort the list of features.
 * Copied from GstAutoVideoSink source (gst-plugins-good-0.10.15)
 *
 * @param f1 GstPluginFeature* the first feature
 * @param f2 GstPluginFeature* the 2nd feature
 *
 * @return gint 0 if they are equal, -1 if f1 is less than f2 and 1 if f1 is greater
 */
static gint democlient_backend_video_sink_compare_ranks(GstPluginFeature * f1, GstPluginFeature * f2);

/**
 * Check if we should list given feature in to search list
 * Copied from GstAutoVideoSink source (gst-plugins-good-0.10.15)
 *
 * @param feature GstPluginFeature* feature to check
 * @param data gpointer userdata to callback function - unused
 *
 * @return gboolean TRUE if should/ FALSE if not
 */
static gboolean democlient_backend_video_sink_factory_filter(GstPluginFeature * feature, gpointer data);

/**
 * Create a sink with given factory
 *
 * @param factory GstElementFactory the factory to create element
 *
 * @return GstElement* an element created with given factory
 */
static GstElement * democlient_backend_create_sink(GstElementFactory * factory);

/**
 * handle messages from pipeline bus.
 *
 * @param bus GstBus* the bus that contains the messages
 * @param msg GstMessage* current bus message
 * @param data gpointer addition user data
 *
 * @return gboolean will comment on this later
 */
static gboolean democlient_backend_bus_watch(GstBus * bus, GstMessage * msg, gpointer data);

/**
 * Parse and print gstreamer message in the case that it's info/warning or error message.
 *
 * @param message GstMessage* the message that we need to parse and print
 *
 * @return Nothing
 */
static void democlient_backend_print_gst_message(GstMessage * message);

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
democlient_backend_init(int *argc,
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
void
democlient_backend_create_pipeline(const gchar *pipeline_description) {
	GstElement * connector;
	GstElement * videosink;
	g_message("SETUP request is sending...");
	g_debug("the pipeline is :%s", pipeline_description);
	pipeline = gst_parse_launch(pipeline_description, NULL);
	g_message("SETUP request sent.");

	videosink = democlient_backend_find_best_video_sink();
	connector = gst_bin_get_by_name(GST_BIN(pipeline), "connector");
	gst_bin_add(GST_BIN(pipeline), videosink);
	gst_element_link(connector, videosink);
	g_object_set(G_OBJECT(videosink), "force-aspect-ratio", TRUE, NULL);
	gst_object_unref(connector);

	// set the bus message handling function
	{
		GstBus * bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
                gst_bus_add_watch(bus, democlient_backend_bus_watch, NULL);
		gst_object_unref(bus);
	}

	if (GST_IS_X_OVERLAY(videosink)) {
		gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(videosink), GPOINTER_TO_INT(window));
	}
}

/**
 * Set the pointer window
 *
 * @param window_ gpointer the pointer to show video region in the main window
 *
 * @return nothing
 */
void
democlient_backend_set_window(gpointer window_) {
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
democlient_backend_play() {
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
democlient_backend_pause() {
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
democlient_backend_stop() {
	gtk_window_resize(GTK_WINDOW(mainWindow), 420, 50);
	GstStateChangeReturn stateReturn;

	stateReturn = gst_element_set_state(pipeline, GST_STATE_NULL);
	g_message("Setting to Stop.....Done");

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
democlient_backend_resume() {
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
democlient_backend_deinit() {
	if (pipeline != NULL) {
		gst_object_unref(pipeline);
		pipeline = NULL;
	}
}

static GstElement * democlient_backend_find_best_video_sink() {
	GList *list, *item;
	GstElement *choice = NULL;
//	GstMessage *message = NULL;
	GSList *errors = NULL;
	GstBus *bus = gst_bus_new();

	GstElement * sink = gst_element_factory_make("fakesink", "fake-video-sink");

	list = gst_registry_feature_filter(gst_registry_get_default(),
			(GstPluginFeatureFilter) democlient_backend_video_sink_factory_filter, FALSE, sink);
	list = g_list_sort(list, (GCompareFunc) democlient_backend_video_sink_compare_ranks);

	for (item = list; item != NULL; item = item->next) {
		GstElementFactory *f = GST_ELEMENT_FACTORY(item->data);
		GstElement *el;

		if ((el = democlient_backend_create_sink(f))) {
			GstStateChangeReturn ret;

			gst_element_set_bus(el, bus);
			ret = gst_element_set_state(el, GST_STATE_READY);
			if (ret == GST_STATE_CHANGE_SUCCESS) {
				choice = el;
				g_debug("So we are using '%s'", GST_PLUGIN_FEATURE (f)->name);
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
			democlient_backend_print_gst_message(errors->data);
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

static gint democlient_backend_video_sink_compare_ranks(GstPluginFeature * f1, GstPluginFeature * f2) {
	gint diff;

	diff = gst_plugin_feature_get_rank(f2) - gst_plugin_feature_get_rank(f1);
	if (diff != 0)
		return diff;
	return strcmp(gst_plugin_feature_get_name(f2),
			gst_plugin_feature_get_name(f1));
}

static gboolean democlient_backend_video_sink_factory_filter(GstPluginFeature * feature, gpointer data) {
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

static GstElement * democlient_backend_create_sink(GstElementFactory* factory) {
	GstElement * sink;
	sink = gst_element_factory_create(factory, "video-sink");
	return sink;
}

static gboolean democlient_backend_bus_watch(GstBus* bus, GstMessage* msg, gpointer data) {
        switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
            {
		gchar * debug;
		GError * error;
                gst_message_parse_error(msg, &error, &debug);

                g_message("PLAY request could not be sent.");
                
                GtkWidget *dialog;
                dialog = gtk_message_dialog_new(NULL,
                                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                                GTK_MESSAGE_ERROR,
                                                GTK_BUTTONS_CLOSE,
                                                "The connection is failed. Please try again");

                gtk_dialog_run (GTK_DIALOG (dialog));
                gtk_widget_destroy (dialog);

                democlient_on_btn_Disconnect_clicked(NULL, NULL);
                g_free(debug);

                g_warning("Pipeline error: %s", error->message);

                g_error_free(error);
            }
                break;
            default:
		break;
        }
	return TRUE;
}

static void democlient_backend_print_gst_message(GstMessage* message) {
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
