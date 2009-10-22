/**
 * \file:   gst-ipcam-client-callbacks.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 * \author: Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com>
 *
 * \date 8-26-2009
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
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
gst_ipcam_client_on_btn_connect_clicked(GtkButton *button,
																				gpointer user_data)
{
	gtk_widget_set_sensitive(btn_connect, FALSE);
	connection_dialog = gst_ipcam_client_create_connection_dialog(main_window);

	if (URL != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(entry_url), URL);
	}

	gtk_widget_show(connection_dialog);
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
gst_ipcam_client_on_btn_disconnect_clicked(GtkButton *button,
																					 gpointer user_data)
{
	/*remove btn_Disconnect button from toolitem_Connect*/
	gtk_container_remove(GTK_CONTAINER(toolitem_connect), btn_disconnect);

	gtk_container_remove(GTK_CONTAINER(toolitem_pause), btn_resume);

	gtk_container_add(GTK_CONTAINER(toolitem_pause), btn_pause);
	gtk_widget_show(btn_pause);


	/*add btn_Connect to toolitem_Connect and stop the pipeline*/
	gtk_container_add(GTK_CONTAINER(toolitem_connect), btn_connect);
	gtk_widget_set_sensitive(btn_connect, TRUE);
	gtk_widget_show(btn_connect);

	gtk_widget_set_sensitive(btn_pause, FALSE);
	gst_ipcam_client_backend_stop();

	gst_ipcam_client_set_status_text("");
	gst_ipcam_client_set_status_video_type("");
	gst_ipcam_client_set_status_audio_type("");

	/*Set all values in change toolbar to be empty*/
	gtk_entry_set_text(entry_bitrate, "");
	//gtk_entry_set_text (GTK_ENTRY (GTK_COMBO (cbx_entry_fps)->entry), " ");
	//gtk_entry_set_text (GTK_ENTRY (GTK_COMBO (cbx_entry_fsize)->entry), " ");
	gtk_combo_box_insert_text(GTK_COMBO_BOX(cbx_entry_fps), 2, " ");
	gtk_combo_box_entry_set_text_column(cbx_entry_fps, 2);
	//gtk_combo_box_entry_set_text_column(cbx_entry_fps, -1);
	//gtk_combo_box_entry_set_text_column(cbx_entry_fsize, -1);
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
gst_ipcam_client_on_btn_pause_clicked(GtkButton *button,
																			gpointer user_data)
{
	/*remove btn_Pause from toolitem_Pause*/
	gtk_container_remove(GTK_CONTAINER(toolitem_pause), btn_pause);

	/*add btn_Resume to toolitem_Pause and pause the pipeline*/
	gtk_container_add(GTK_CONTAINER(toolitem_pause), btn_resume);
	gtk_widget_show(btn_resume);
	gtk_widget_set_sensitive(btn_resume, TRUE);
	gst_ipcam_client_backend_pause();

	/*Set change button to be insensitive*/
	gtk_widget_set_sensitive(btn_change, FALSE);
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
gst_ipcam_client_on_btn_resume_clicked(GtkButton *button,
																			 gpointer user_data)
{
	/*remove btn_Resume from toolitem_pause*/
	gtk_container_remove(GTK_CONTAINER(toolitem_pause), btn_resume);

	/*add btn_Pause to toolitem_pause and resume playing video*/
	gtk_container_add(GTK_CONTAINER(toolitem_pause), btn_pause);
	gtk_widget_show(btn_pause);
	gst_ipcam_client_backend_resume();

	/*Set change button to be sensitive*/
	gtk_widget_set_sensitive(btn_change, TRUE);
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
gst_ipcam_client_on_btn_about_clicked(GtkButton *button,
																			gpointer user_data)
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
gst_ipcam_client_on_btn_quit_clicked(GtkButton *button,
																		 gpointer user_data)
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
gst_ipcam_client_on_btn_connect_dialog_clicked(GtkButton *button,
																							 gpointer user_data)
{
	/* static gint counter = 0;*/
	is_connect_button_clicked = TRUE;
	gchar *url = gtk_entry_get_text(entry_url);
	URL = g_strconcat("", url, NULL);
	gst_ipcam_client_backend_set_window(GINT_TO_POINTER(GDK_WINDOW_XWINDOW(prw_video->window)));

	gst_ipcam_client_backend_create_pipeline(URL);

	g_message("PLAY request is sending...");
	/*set pipeline to playing status*/
	gst_ipcam_client_backend_play();

	g_message("PLAY request sent.");

	/*Resize the mainwindow to show Video got from server*/
	gtk_window_resize(GTK_WINDOW(main_window), 650, 500);
	gtk_widget_set_sensitive(vbox2, TRUE);

	/*remove btn_Connect button from toolitem_Connect*/
	gtk_container_remove(GTK_CONTAINER(toolitem_connect), btn_connect);

	/*add btn_Disconnect to toolitem_Connect*/
	gtk_container_add(GTK_CONTAINER(toolitem_connect), btn_disconnect);
	gtk_widget_show(btn_disconnect);

	/*Get the Pause button and Disconnect button to be sensitive;*/
	gtk_widget_set_sensitive(btn_pause, TRUE);
	gtk_widget_set_sensitive(btn_disconnect, TRUE);

	/*destroy the connection dialog*/
	gtk_widget_destroy(connection_dialog);
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
gst_ipcam_client_on_connection_dialog_destroy(GtkObject *object,
																							gpointer user_data)
{
	if (!is_connect_button_clicked)
	{
		/*Active the Connect button and Inactive the Pause button*/
		gtk_widget_set_sensitive(btn_pause, FALSE);
		gtk_widget_set_sensitive(btn_connect, TRUE);
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
void gst_ipcam_client_on_main_window_destroy(GtkObject *object,
																						 gpointer user_data)
{
	gst_ipcam_client_backend_stop();
	gtk_main_quit();
	gst_ipcam_client_backend_deinit();
}

/**
 * Handle the event when users press enter button in the connection_dialog
 * The application will exit and send Teardown to server
 *
 * @param argc GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
gboolean
gst_ipcam_client_on_connection_dialog_key_press_event(GtkWidget *widget,
																											GdkEventKey *event,
																											gpointer user_data)
{
	if (event->keyval == GDK_Return)
	{
		gst_ipcam_client_on_btn_connect_dialog_clicked(NULL, NULL);
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
gst_ipcam_client_on_btn_change_clicked(GtkButton *button,
																			 gpointer user_data)
{
	gchar *url_fps;
	gchar *url_fsize;
	gchar *url_bitrate;
	gchar * pipeline_description;

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
		url_fsize = g_strconcat("", "width=", __f_size[0], "&height=", __f_size[1], NULL);
	}
	else
	{
		url_fsize = "";
	}

	if (g_strcmp0(gtk_entry_get_text(entry_bitrate), "") != 0)
	{
		gchar *bitrate = gtk_entry_get_text(entry_bitrate);
		url_bitrate = g_strconcat("", "bitrate=", bitrate, NULL);
	}
	else
	{
		url_bitrate = "";
	}
	
	if (g_strcmp0(url_fps, "") != 0)
	{
		if (g_strcmp0(url_fsize, "") != 0)
		{
			if (g_strcmp0(url_bitrate, "") != 0)
				pipeline_description = g_strconcat("", URL, url_fps, "&", url_fsize, "&", url_bitrate, NULL);
			else
				pipeline_description = g_strconcat("", URL, url_fps, "&", url_fsize, NULL);
		}
		else
		{
			if (g_strcmp0(url_bitrate, "") != 0)
				pipeline_description = g_strconcat("", URL, url_fps, "&", url_bitrate, NULL);
			else
				pipeline_description = g_strconcat("", URL, url_fps, NULL);
		}
	}
	else if (g_strcmp0(url_fsize, "") != 0)
	{
		if (g_strcmp0(url_bitrate, "") != 0)
			pipeline_description = g_strconcat("", URL, "?", url_fsize, "&", url_bitrate, NULL);
		else
			pipeline_description = g_strconcat("", URL, "?", url_fsize, NULL);
	}
	else if (g_strcmp0(url_bitrate, "") != 0)
	{
		pipeline_description = g_strconcat("", URL, "?", url_bitrate, NULL);
	}

	g_message(pipeline_description);
	gst_ipcam_client_backend_create_pipeline(pipeline_description);
	gst_ipcam_client_backend_play();

	g_message("PLAY request sent.");
	/*Resize the mainwindow to show the video got from server*/
	gtk_window_resize(GTK_WINDOW(main_window), 650, 500);
	gtk_widget_set_sensitive(vbox2, TRUE);
}