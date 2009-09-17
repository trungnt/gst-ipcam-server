/**
 * \file:   democlient-callbacks.c
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
    //gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Connect);
    //btn_Connect = g_object_ref(btn_Connect);

    ///add btn_Disconnect button to toolitem_Connect
    //gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
    ///show the button
    //gtk_widget_show (btn_Disconnect);
    gtk_widget_set_sensitive(btn_Connect, FALSE);
    connectionDialog = democlient_create_connectionDialog(); 

    if (TYPE != 4)
    {
	gtk_combo_box_set_active(cbx_VideoStreamType, TYPE);
    }
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
democlient_on_btn_Disconnect_clicked              (GtkButton       *button,
                                           gpointer         user_data)
{
    ///remove btn_Disconnect button from toolitem_Connect
    gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
    //btn_Disconnect = g_object_ref(btn_Disconnect);

    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Resume);
    //btn_Resume = g_object_ref(btn_Resume);

    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Pause);
    gtk_widget_show (btn_Pause);


    ///add btn_Connect to toolitem_Connect and stop the pipeline
    gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Connect);
    gtk_widget_set_sensitive(btn_Connect, TRUE);
    gtk_widget_show (btn_Connect);

    gtk_widget_set_sensitive(btn_Pause, FALSE);
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
    //btn_Pause = g_object_ref(btn_Pause);

    ///add btn_Resume to toolitem_Pause and pause the pipeline
    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Resume);
    gtk_widget_show(btn_Resume);
    gtk_widget_set_sensitive(btn_Resume, TRUE);
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
    //btn_Resume = g_object_ref(btn_Resume);

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
    democlient_backend_stop();
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
    is_connect_button_clicked = TRUE;
    gchar *url;
    url = gtk_entry_get_text(entry_Url);
    URL = g_strconcat("", url, NULL);
    democlient_backend_set_window (GINT_TO_POINTER (GDK_WINDOW_XWINDOW (prw_GuestVideo->window)));

    ///if video stream type is JPEG
    if (g_strcmp0(gtk_combo_box_get_active_text(cbx_VideoStreamType), "Jpeg stream") == 0)
    {
        ///set url and create the pipeline to get jpeg stream
        url = g_strconcat("rtspsrc location=", url, " ! rtpjpegdepay ! jpegdec ! queue ! ffmpegcolorspace ! videoscale ! identity name=connector", NULL);
        TYPE = 0;
        democlient_backend_create_pipeline(url);
        
        g_free(url);
    }
    ///if video stream type is mpeg4
    else if (g_strcmp0(gtk_combo_box_get_active_text(cbx_VideoStreamType), "Mpeg4 stream") == 0)
    {
        ///set url and create the pipeline to get mpeg4 stream
        url = g_strconcat("rtspsrc location=", url, " ! rtpmp4vdepay ! ffdec_mpeg4 ! queue ! ffmpegcolorspace ! identity name=connector", NULL);
        TYPE = 1;
        democlient_backend_create_pipeline(url);
        
        g_free(url);
    }
    ///if video stream type is h264
    else if (g_strcmp0(gtk_combo_box_get_active_text(cbx_VideoStreamType), "H264 stream") == 0)
    {
        ///set url and create the pipeline to get h264 stream
        url = g_strconcat("rtspsrc location=", url, " ! rtph264depay ! ffdec_h264 ! ffmpegcolorspace ! identity name=connector", NULL);
	TYPE = 2;
        democlient_backend_create_pipeline(url);

        g_free(url);
    }
    g_message("PLAY request is sending...");
    ///set pipeline to playing status
    GstStateChangeReturn state = democlient_backend_play ();

    if (state == GST_STATE_CHANGE_FAILURE)
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
	gtk_widget_set_sensitive(btn_Connect, TRUE);
    }
    else
    {
	g_message("PLAY request sent.");
	///remove btn_Disconnect button from toolitem_Connect
        gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Connect);
        //btn_Disconnect = g_object_ref(btn_Connect);

        ///add btn_Connect to toolitem_Connect and stop the pipeline
        gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
        gtk_widget_show (btn_Disconnect);

        //Get the Pause button and Disconnect button to be sensitive;
        gtk_widget_set_sensitive(btn_Pause, TRUE);
        gtk_widget_set_sensitive(btn_Disconnect, TRUE);
    }

    //destroy the connection dialog
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
democlient_on_connectionDialog_destroy                (GtkObject       *object,
                                        gpointer         user_data)
{
    if (!is_connect_button_clicked)
    {
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
void democlient_on_mainWindow_destroy                (GtkObject       *object,
                                        gpointer         user_data)
{
    democlient_backend_stop();
    gtk_main_quit();
    democlient_backend_deinit();
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
democlient_on_connectionDialog_key_press_event        (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
    if (event->keyval == GDK_Return)
     {
         democlient_on_btn_ConnectDialog_clicked(NULL, NULL);
     }

    return FALSE;
}
