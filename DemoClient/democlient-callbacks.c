/*
 * \file:   democlient-callbacks.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 8-26-2009
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

/**
 * Handle the event when clicking on Connect button in the main window.
 * The button will change to Disconnect button and the connection dialog will appear
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Connect_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    ///remove btn_Connect button from toolitem_Connect
    gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Connect);
    btn_Connect = g_object_ref(btn_Connect);

    ///add btn_Disconnect button to toolitem_Connect
    gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
    //show the button
    gtk_widget_show (btn_Disconnect);

    ///Create the connection Dialog and show it
    GtkWidget *connectionDialog;
    connectionDialog = democlient_create_connectionDialog();
    gtk_widget_show(connectionDialog);

}

/**
 * Handle the event when clicking on Disconnect button in the main window.
 * The button will change to connect button and the application will stop playing
 * the receiving video
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Disconnect_clicked              (GtkButton       *button,
                                           gpointer         user_data)
{
    ///remove btn_Disconnect button from toolitem_Connect
    gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
    btn_Disconnect = g_object_ref(btn_Disconnect);

    ///add btn_Connect to toolitem_Connect and stop the pipeline
    gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Connect);
    gtk_widget_show (btn_Connect);
    democlient_backend_stop();
}

/**
 * Handle the event when clicking on the Pause button in the main window
 * The button will change to Resume button and  the application will pause playing
 * the receving video
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Pause_clicked                   (GtkButton       *button,
                                            gpointer         user_data)
{
    ///remove btn_Pause from toolitem_Pause
    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Pause);
    btn_Pause = g_object_ref(btn_Pause);

    ///add btn_Resume to toolitem_Pause and pause the pipeline
    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Resume);
    democlient_backend_pause();
}

/**
 * Handle the event when clicking on the Resume button in the main window
 * The button will change to Pause button and the application will Resume playing
 * the receving video
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Resume_clicked                  (GtkButton       *button,
                                            gpointer         user_data)
{
    ///remove btn_Resume from toolitem_Pause
    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Resume);
    btn_Resume = g_object_ref(btn_Resume);

    ///add btn_Pause to toolitem_Pause and resume playing video
    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Pause);
    gtk_widget_show (btn_Pause);
    democlient_backend_resume();
}

/**
 * Handle the event when clicking on the Options button.
 * Show the options dialog
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
/*void
democlient_on_btn_Options_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *optionDialog;
    optionDialog = democlient_create_optionDialog();
    gtk_widget_show(optionDialog);
}*/

/**
 * Handle the event when clicking on the About button
 * Show the About dialog
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_About_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}

/**
 * Handle the event when clicking on the quit button
 * free all memories and close the application
 *
 * @param argc GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Quit_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
    gtk_main_quit();
    democlient_backend_deinit();
}

/**
 * Handle the event when clicking on the connect button in the connection dialog.
 * Get the url and Video stream type, close the dialog and play video
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_ConnectDialog_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    gchar *url;
    url = gtk_entry_get_text(entry_Url);
    democlient_backend_set_window (GINT_TO_POINTER (GDK_WINDOW_XWINDOW (prw_GuestVideo->window)));

    ///if video stream type is JPEG
    if (g_strcmp0(gtk_combo_box_get_active_text(cbx_VideoStreamType), "Jpeg stream") == 0)
    {
        ///set url and create the pipeline to get jpeg stream
        url = g_strconcat("rtspsrc location=", url, " ! rtpjpegdepay ! jpegdec ! queue ! ffmpegcolorspace ! videoscale ! xvimagesink name=videosink", NULL);
        g_print(url);
        democlient_backend_create_pipeline(url);
        g_free(url);
    }
    ///if video stream type is mpeg4
    else if (g_strcmp0(gtk_combo_box_get_active_text(cbx_VideoStreamType), "Mpeg4 stream") == 0)
    {
        ///set url and create the pipeline to get mpeg4 stream
        url = g_strconcat("rtspsrc location=", url, " ! rtpmp4vdepay ! ffdec_mpeg4 ! queue ! ffmpegcolorspace ! xvimagesink name=videosink", NULL);
        g_print(url);
        democlient_backend_create_pipeline(url);
        g_free(url);
    }
    ///if video stream type is h264
    else if (g_strcmp0(gtk_combo_box_get_active_text(cbx_VideoStreamType), "H264 stream") == 0)
    {
        ///set url and create the pipeline to get h264 stream
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
    ///set pipeline to playing status
    democlient_backend_play ();
    ///destroy the connection dialog
    gtk_widget_destroy(connectionDialog);
}

/**
 * Handle the event when clicking on the Cancel button in the Options Dialog.
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Cancel_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{

}

/**
 * Handle the event when clicking on the OK button in the Options Dialog.
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Ok_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{

}