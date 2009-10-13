/**
 * \file:   gst-ipcam-client-callbacks.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 * \author: Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com>
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

#include "gst-ipcam-client-callbacks.h"
#include "gst-ipcam-client-interface.h"
#include "gst-ipcam-client-support.h"
#include "gst-ipcam-client-backend.h"
#include "gst-ipcam-client-windowid.h"


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
gst_ipcam_client_on_btn_Connect_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    gtk_widget_set_sensitive(btn_Connect, FALSE);
    connectionDialog = gst_ipcam_client_create_connectionDialog(); 

    if (URL != NULL)
    {
	gtk_entry_set_text(GTK_ENTRY(entry_Url), URL);
    }

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
gst_ipcam_client_on_btn_Disconnect_clicked              (GtkButton       *button,
                                           gpointer         user_data)
{
    /*remove btn_Disconnect button from toolitem_Connect*/
    gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);

    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Resume);

    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Pause);
    gtk_widget_show (btn_Pause);


    /*add btn_Connect to toolitem_Connect and stop the pipeline*/
    gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Connect);
    gtk_widget_set_sensitive(btn_Connect, TRUE);
    gtk_widget_show (btn_Connect);

    gtk_widget_set_sensitive(btn_Pause, FALSE);
    gst_ipcam_client_backend_stop();
    
    gst_ipcam_client_set_status_text("");
    gst_ipcam_client_set_status_Video_Type("");
    gst_ipcam_client_set_status_Audio_Type("");
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
gst_ipcam_client_on_btn_Pause_clicked                   (GtkButton       *button,
                                            gpointer         user_data)
{
    /*remove btn_Pause from toolitem_Pause*/
    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Pause);

    /*add btn_Resume to toolitem_Pause and pause the pipeline*/
    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Resume);
    gtk_widget_show(btn_Resume);
    gtk_widget_set_sensitive(btn_Resume, TRUE);
    gst_ipcam_client_backend_pause();
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
gst_ipcam_client_on_btn_Resume_clicked                  (GtkButton       *button,
                                            gpointer         user_data)
{
    /*remove btn_Resume from toolitem_Pause*/
    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Resume);

    /*add btn_Pause to toolitem_Pause and resume playing video*/
    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Pause);
    gtk_widget_show (btn_Pause);
    gst_ipcam_client_backend_resume();
}

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
gst_ipcam_client_on_btn_About_clicked                   (GtkButton       *button,
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
gst_ipcam_client_on_btn_Quit_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
    gst_ipcam_client_backend_stop();
    gtk_main_quit();
    gst_ipcam_client_backend_deinit();
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
gst_ipcam_client_on_btn_ConnectDialog_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    /* static gint counter = 0;*/
    is_connect_button_clicked = TRUE;
    gchar *url = gtk_entry_get_text(entry_Url);
    URL = g_strconcat("", url, NULL);
    gst_ipcam_client_backend_set_window (GINT_TO_POINTER (GDK_WINDOW_XWINDOW (prw_GuestVideo->window)));

    gst_ipcam_client_backend_create_pipeline(URL);

    g_message("PLAY request is sending...");
    /*set pipeline to playing status*/
    gst_ipcam_client_backend_play ();

    g_message("PLAY request sent.");

    /*Resize the mainwindow to show Video got from server*/
    gtk_window_resize(GTK_WINDOW(mainWindow), 550, 500);
    gtk_widget_set_sensitive(vbox2, TRUE);

    /*remove btn_Connect button from toolitem_Connect*/
    gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Connect);

    /*add btn_Disconnect to toolitem_Connect*/
    gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
    gtk_widget_show (btn_Disconnect);

    /*Get the Pause button and Disconnect button to be sensitive;*/
    gtk_widget_set_sensitive(btn_Pause, TRUE);
    gtk_widget_set_sensitive(btn_Disconnect, TRUE);

    /*destroy the connection dialog*/
    gtk_widget_destroy(connectionDialog);
    is_connect_button_clicked = FALSE;
}

/**
 * Handle the event when user close the connection dialog
 * The application will change the Disconnect button to connect button
 *
 * @param argc GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
gst_ipcam_client_on_connectionDialog_destroy                (GtkObject       *object,
                                        gpointer         user_data)
{
    if (!is_connect_button_clicked)
    {
        /*Active the Connect button and Inactive the Pause button*/
        gtk_widget_set_sensitive(btn_Pause, FALSE);
        gtk_widget_set_sensitive(btn_Connect, TRUE);
    }
}

/**
 * Handle the event when user close the main window
 * The application will exit and send Teardown to server
 *
 * @param argc GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void gst_ipcam_client_on_mainWindow_destroy                (GtkObject       *object,
                                        gpointer         user_data)
{
    gst_ipcam_client_backend_stop();
    gtk_main_quit();
    gst_ipcam_client_backend_deinit();
}

/**
 * Handle the event when users press enter button in the connectionDialog
 * The application will exit and send Teardown to server
 *
 * @param argc GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
gboolean
gst_ipcam_client_on_connectionDialog_key_press_event        (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
    if (event->keyval == GDK_Return)
     {
         gst_ipcam_client_on_btn_ConnectDialog_clicked(NULL, NULL);
     }

    return FALSE;
}

/**
 * Handle the event when clicking on the Change button in the main window.
 * the video will change fps and bitrate followed the input from users
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
gst_ipcam_client_on_btn_Change_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    gchar *url_fps;
    gchar *url_fsize;

    gst_ipcam_client_backend_stop();
    if (g_strcmp0(gtk_combo_box_get_active_text(cbx_entry_fps), "") != 0)
    {
        url_fps = g_strconcat("", "?framerate=", gtk_combo_box_get_active_text(cbx_entry_fps), NULL);
    }
    else
    {
        url_fps = "";
    }

    if (g_strcmp0(gtk_combo_box_get_active_text(cbx_entry_fsize), "") != 0)
    {
        gchar *f_size = gtk_combo_box_get_active_text(cbx_entry_fsize);
        gchar **__f_size = g_strsplit(f_size, "x", 0);
        url_fsize = g_strconcat("", "&width=", __f_size[0], "&height=", __f_size[1], NULL);
    }
    else
    {
        url_fsize = "";
    }

    gst_ipcam_client_backend_create_pipeline(g_strconcat("", URL, url_fps, url_fsize, NULL));
    gst_ipcam_client_backend_play();

    g_message("PLAY request sent.");
    /*Resize the mainwindow to show the video got from server*/
    gtk_window_resize(GTK_WINDOW(mainWindow), 550, 500);
    gtk_widget_set_sensitive(vbox2, TRUE);
}