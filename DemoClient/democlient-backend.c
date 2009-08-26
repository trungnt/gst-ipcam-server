/*
 * File:   democlient_backend.c
 * Author: dqtuan
 *
 * Created on August 26, 2009, 11:02 AM
 */
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>
#include "democlient-backend.h"

static gpointer window;
static GstElement *pipeline;
static GstElement *videosink;

/**
 * init for using gstreamer
 * This function is used whenever the main function is created
 *
 * @param argc int *
 * @param argv[] char **
 *
 * @return nothing
 *
 * @author Dam Quang Tuan <damquang.tuan@nomovok.com>
 */
void
democlient_backend_init (int *argc,
              char **argv[])
{
    gst_init (argc, argv);
}

/**
 * create the pipeline to get the data from the rtsp server
 *
 * @param pipeline_description const gchar * the pipeline description
 *
 * @return nothing
 *
 * @author Dam Quang Tuan <damquang.tuan@nomovok.com>
 */
void
democlient_backend_create_pipeline(const gchar *pipeline_description)
{
    pipeline = gst_parse_launch(pipeline_description, NULL);
    videosink = gst_bin_get_by_name (GST_BIN(pipeline), "videosink");

    g_object_set (G_OBJECT (videosink), "force-aspect-ratio", TRUE, NULL);

    if (GST_IS_X_OVERLAY (videosink))
    {
        gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GPOINTER_TO_INT (window));
    }
}

void
democlient_backend_set_window (gpointer window_)
{
    window = window_;
}

void
democlient_backend_play()
{
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_print("Setting to Play.....Done\n");
}

void
democlient_backend_pause()
{
    gst_element_set_state(pipeline, GST_STATE_PAUSED);
    g_print("Setting to Pause.....Done\n");
}

void
democlient_backend_stop()
{
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_print("Setting to Stop.....Done\n");
}

void
democlient_backend_resume()
{
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_print("Setting to Resume.....Done\n");
}

void
democlient_backend_deinit()
{
    if (pipeline != NULL)
    {
        gst_object_unref(pipeline);
        pipeline = NULL;
    }
    if (videosink != NULL)
    {
        gst_object_unref(videosink);
        videosink = NULL;
    }
}