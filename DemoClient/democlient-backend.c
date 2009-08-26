/*
 * File:   democlient_backend.c
 * Author: dqtuan
 *
 * Created on August 26, 2009, 11:02 AM
 */
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>
#include "democlient_backend.h"
static GstElement *pipeline;
void
democlient_backend_init (int *argc,
              char **argv[])
{
    gst_init (argc, argv);
}

void
democlient_backend_set_window (gpointer window_)
{
    window = window_;
}