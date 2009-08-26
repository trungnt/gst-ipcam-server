/*
 * File:   democlient-callbacks.c
 * Author: damquang.tuan@nomovok.com
 *
 * Created on August 26, 2009, 10:04 AM
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gst/interfaces/xoverlay.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

#include "democlient-callbacks.h"
#include "democlient-interface.h"
#include "democlient-support.h"
#include "democlient-backend.h"

/*
 *
 *
 */
void
democlient_on_btn_Connect_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Connect);
    btn_Connect = g_object_ref(btn_Connect);
    
    gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
    gtk_widget_show (btn_Disconnect);
    
    GtkWidget *connectionDialog;
    connectionDialog = democlient_create_connectionDialog();
    gtk_widget_show(connectionDialog);

}

void
democlient_on_btn_Disconnect_clicked              (GtkButton       *button,
                                           gpointer         user_data)
{
    gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
    btn_Disconnect = g_object_ref(btn_Disconnect);
    
    gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Connect);
    gtk_widget_show (btn_Connect);
    democlient_backend_stop();
}

void
democlient_on_btn_Pause_clicked                   (GtkButton       *button,
                                            gpointer         user_data)
{
    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Pause);
    btn_Pause = g_object_ref(btn_Pause);

    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Resume);
    democlient_backend_pause();
}

void
democlient_on_btn_Resume_clicked                  (GtkButton       *button,
                                            gpointer         user_data)
{
    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Resume);
    btn_Resume = g_object_ref(btn_Resume);

    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Pause);
    gtk_widget_show (btn_Pause);
    democlient_backend_resume();
}

void
democlient_on_btn_Options_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *optionDialog;
    optionDialog = democlient_create_optionDialog();
    gtk_widget_show(optionDialog);
}


void
democlient_on_btn_About_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
democlient_on_btn_Quit_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
    gtk_main_quit();
    democlient_backend_deinit();
}


void
democlient_on_btn_ConnectDialog_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    gchar *url;
    url = gtk_entry_get_text(entry_Url);
    democlient_backend_set_window (GINT_TO_POINTER (GDK_WINDOW_XWINDOW (prw_GuestVideo->window)));

    if (g_strcmp0(gtk_combo_box_get_active_text(cbx_VideoStreamType), "Jpeg stream") == 0)
    {
        url = g_strconcat("rtspsrc location=", url, " ! rtpjpegdepay ! jpegdec ! queue ! ffmpegcolorspace ! videoscale ! xvimagesink name=videosink", NULL);
        g_print(url);
        democlient_backend_create_pipeline(url);
        g_free(url);
    }
    else if (g_strcmp0(gtk_combo_box_get_active_text(cbx_VideoStreamType), "Mpeg4 stream") == 0)
    {
        url = g_strconcat("rtspsrc location=", url, " ! rtpmp4vdepay ! ffdec_mpeg4 ! queue ! ffmpegcolorspace ! xvimagesink name=videosink", NULL);
        g_print(url);
        democlient_backend_create_pipeline(url);
        g_free(url);
    }
    else if (g_strcmp0(gtk_combo_box_get_active_text(cbx_VideoStreamType), "H264 stream") == 0)
    {
        url = g_strconcat("rtspsrc location=", url, " ! rtph264depay ! ffdec_h264 ! ffmpegcolorspace ! xvimagesink name=videosink", NULL);
        g_print(url);
        democlient_backend_create_pipeline(url);
        g_free(url);
    }
    else
    {
        g_print("Error");
        gtk_main_quit();
    }
    democlient_backend_play ();
    gtk_widget_destroy(connectionDialog);
}


void
democlient_on_btn_Cancel_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
democlient_on_btn_Ok_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{

}