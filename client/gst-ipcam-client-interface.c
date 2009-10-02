/**
 * \file:   gst-ipcam-client-interface.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 8-26-2009
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "gst-ipcam-client-callbacks.h"
#include "gst-ipcam-client-interface.h"
#include "gst-ipcam-client-support.h"
#include "gst-ipcam-client-windowid.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

/**
 * create the main window
 *
 * @param void
 *
 * @return nothing
 */
GtkWidget*
gst_ipcam_client_create_mainWindow (void)
{
  mainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gchar * main_window_title = gst_ipcam_client_window_create_title(_("Gst Ipcam Client"));
  gtk_window_set_title (GTK_WINDOW (mainWindow), main_window_title);
  g_free(main_window_title);
  gtk_window_set_default_size (GTK_WINDOW (mainWindow), 420, 50);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox);
  gtk_container_add (GTK_CONTAINER (mainWindow), vbox);

  toolbar = gtk_toolbar_new ();
  gtk_widget_show (toolbar);
  gtk_box_pack_start (GTK_BOX (vbox), toolbar, FALSE, FALSE, 0);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_BOTH);
  tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar));

  toolitem_Connect = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem_Connect);
  gtk_container_add (GTK_CONTAINER (toolbar), toolitem_Connect);

  btn_Connect = gtk_button_new_from_stock ("gtk-connect");
  gtk_widget_show (btn_Connect);
  gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Connect);

  btn_Disconnect = gtk_button_new_from_stock ("gtk-disconnect");
  //gtk_widget_show (btn_Disconnect);
  gtk_widget_set_sensitive(btn_Disconnect, FALSE);
  //gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);

  toolitem_Pause = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem_Pause);
  gtk_container_add (GTK_CONTAINER (toolbar), toolitem_Pause);

  btn_Pause = gtk_button_new_from_stock ("gtk-media-pause");
  gtk_widget_show (btn_Pause);
  gtk_widget_set_sensitive(btn_Pause, FALSE);
  gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Pause);

  btn_Resume = gtk_button_new ();
  //gtk_widget_show (btn_Resume);
  gtk_widget_set_sensitive(btn_Resume, FALSE);

  algn_Resume = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (algn_Resume);
  gtk_container_add (GTK_CONTAINER (btn_Resume), algn_Resume);

  hbox_ResumeButton = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_ResumeButton);
  gtk_container_add (GTK_CONTAINER (algn_Resume), hbox_ResumeButton);

  img_Resume = gtk_image_new_from_stock ("gtk-media-play", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (img_Resume);
  gtk_box_pack_start (GTK_BOX (hbox_ResumeButton), img_Resume, FALSE, FALSE, 0);

  lbl_ResumeButton = gtk_label_new_with_mnemonic ("_Resume");
  gtk_widget_show (lbl_ResumeButton);
  gtk_box_pack_start (GTK_BOX (hbox_ResumeButton), lbl_ResumeButton, FALSE, FALSE, 0);

  toolitem_About = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem_About);
  gtk_container_add (GTK_CONTAINER (toolbar), toolitem_About);

  btn_About = gtk_button_new_from_stock ("gtk-about");
  gtk_widget_show (btn_About);
  gtk_container_add (GTK_CONTAINER (toolitem_About), btn_About);

  toolitem_Quit = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem_Quit);
  gtk_container_add (GTK_CONTAINER (toolbar), toolitem_Quit);

  btn_Quit = gtk_button_new_from_stock ("gtk-quit");
  gtk_widget_show (btn_Quit);
  gtk_container_add (GTK_CONTAINER (toolitem_Quit), btn_Quit);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox), hbox1, TRUE, TRUE, 0);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_widget_set_sensitive(vbox2, FALSE);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox2, TRUE, TRUE, 0);

  toolbar1 = gtk_toolbar_new ();
  /*gtk_widget_show (toolbar1);*/
  gtk_box_pack_start (GTK_BOX (vbox2), toolbar1, FALSE, FALSE, 0);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar1), GTK_TOOLBAR_BOTH);
  tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1));

  toolitem6 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem6);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem6);

  lbl_fps = gtk_label_new (_("Frame rate:"));
  gtk_widget_show (lbl_fps);
  gtk_container_add (GTK_CONTAINER (toolitem6), lbl_fps);

  toolitem8 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem8);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem8);

  spinbtn_fps_n_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
  spinbtn_fps_n = gtk_spin_button_new (GTK_ADJUSTMENT (spinbtn_fps_n_adj), 1, 0);
  gtk_widget_show (spinbtn_fps_n);
  gtk_container_add (GTK_CONTAINER (toolitem8), spinbtn_fps_n);

  toolitem7 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem7);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem7);

  lbl__ = gtk_label_new (_("/"));
  gtk_widget_show (lbl__);
  gtk_container_add (GTK_CONTAINER (toolitem7), lbl__);

  toolitem9 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem9);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem9);

  spinbtn_fps_d_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
  spinbtn_fps_d = gtk_spin_button_new (GTK_ADJUSTMENT (spinbtn_fps_d_adj), 1, 0);
  gtk_widget_show (spinbtn_fps_d);
  gtk_container_add (GTK_CONTAINER (toolitem9), spinbtn_fps_d);

  toolitem15 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem15);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem15);

  alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (toolitem15), alignment2);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 41, 0);

  toolitem12 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem12);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem12);

  lbl_bitrate = gtk_label_new (_("Bit rate:"));
  gtk_widget_show (lbl_bitrate);
  gtk_container_add (GTK_CONTAINER (toolitem12), lbl_bitrate);

  toolitem13 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem13);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem13);

  spinbtn_bitrate_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
  spinbtn_bitrate = gtk_spin_button_new (GTK_ADJUSTMENT (spinbtn_bitrate_adj), 1, 0);
  gtk_widget_show (spinbtn_bitrate);
  gtk_container_add (GTK_CONTAINER (toolitem13), spinbtn_bitrate);

  toolitem14 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem14);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem14);

  btn_change = gtk_button_new_with_mnemonic (_("Change"));
  gtk_widget_show (btn_change);
  gtk_container_add (GTK_CONTAINER (toolitem14), btn_change);

  hbox_VideoPrew = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox_VideoPrew);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox_VideoPrew, TRUE, TRUE, 0);

  prw_GuestVideo = gtk_drawing_area_new ();//gtk_preview_new (GTK_PREVIEW_COLOR);
  gtk_widget_show (prw_GuestVideo);
  gtk_box_pack_start (GTK_BOX (hbox_VideoPrew), prw_GuestVideo, TRUE, TRUE, 0);

  statusBar = gst_ipcam_client_init_status_bar();
  gtk_widget_show (statusBar);
  gtk_box_pack_start (GTK_BOX (vbox), statusBar, FALSE, FALSE, 0);
  gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (statusBar), FALSE);

  g_signal_connect_swapped ((gpointer) btn_Connect, "clicked",
                            G_CALLBACK (gst_ipcam_client_on_btn_Connect_clicked),
                            GTK_OBJECT (NULL));

  g_signal_connect_swapped ((gpointer) btn_Disconnect, "clicked",
                            G_CALLBACK (gst_ipcam_client_on_btn_Disconnect_clicked),
                            GTK_OBJECT (NULL));

  g_signal_connect_swapped ((gpointer) btn_Pause, "clicked",
                            G_CALLBACK (gst_ipcam_client_on_btn_Pause_clicked),
                            GTK_OBJECT (NULL));
  
  g_signal_connect_swapped ((gpointer) btn_Resume, "clicked",
                            G_CALLBACK (gst_ipcam_client_on_btn_Resume_clicked),
                            GTK_OBJECT (NULL));

  g_signal_connect_swapped ((gpointer) btn_About, "clicked",
                            G_CALLBACK (gst_ipcam_client_on_btn_About_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_Quit, "clicked",
                            G_CALLBACK (gst_ipcam_client_on_btn_Quit_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) mainWindow, "destroy",
                            G_CALLBACK (gst_ipcam_client_on_mainWindow_destroy),
                            GTK_OBJECT (NULL));
  /*g_signal_connect_swapped ((gpointer) btn_change, "clicked",
                            G_CALLBACK (gst_ipcam_client_on_btn_Change_clicked),
                            GTK_OBJECT (NULL));*/

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (mainWindow, mainWindow, "mainWindow");
  GLADE_HOOKUP_OBJECT (mainWindow, vbox, "vbox");
  GLADE_HOOKUP_OBJECT (mainWindow, toolbar, "toolbar");
  GLADE_HOOKUP_OBJECT (mainWindow, toolitem_Connect, "toolitem_Connect");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Connect, "btn_Connect");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Disconnect, "btn_disconnect");
  GLADE_HOOKUP_OBJECT (mainWindow, toolitem_Pause, "toolitem_Pause");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Pause, "btn_Pause");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Resume, "btn_Resume");
  GLADE_HOOKUP_OBJECT (mainWindow, toolitem_Options, "toolitem_Options");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Options, "btn_Preferences");
  GLADE_HOOKUP_OBJECT (mainWindow, toolitem_About, "toolitem_About");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_About, "btn_About");
  GLADE_HOOKUP_OBJECT (mainWindow, toolitem_Quit, "toolitem_Quit");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Quit, "btn_Quit");
  GLADE_HOOKUP_OBJECT (mainWindow, hbox_VideoPrew, "hbox_VideoPrew");
  GLADE_HOOKUP_OBJECT (mainWindow, prw_GuestVideo, "prw_GuestVideo");
  GLADE_HOOKUP_OBJECT (mainWindow, statusBar, "statusBar");

  return mainWindow;
}

/**
 * create the connection dialog
 *
 * @param void
 *
 * @return nothing
 */
GtkWidget*
gst_ipcam_client_create_connectionDialog (void)
{
  connectionDialog = gtk_dialog_new ();
  gchar * connection_dialog_title = gst_ipcam_client_window_create_title(_("Connection"));
  gtk_window_set_title (GTK_WINDOW (connectionDialog), connection_dialog_title);
  g_free(connection_dialog_title);
  gtk_window_set_resizable (GTK_WINDOW (connectionDialog), FALSE);
  gtk_window_set_type_hint (GTK_WINDOW (connectionDialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox = GTK_DIALOG (connectionDialog)->vbox;
  gtk_widget_show (dialog_vbox);

  tbl_ConInfo = gtk_table_new (1, 2, FALSE);
  gtk_widget_show (tbl_ConInfo);
  gtk_box_pack_start (GTK_BOX (dialog_vbox), tbl_ConInfo, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_ConInfo), 3);
  gtk_table_set_row_spacings (GTK_TABLE (tbl_ConInfo), 4);
  gtk_table_set_col_spacings (GTK_TABLE (tbl_ConInfo), 11);

  lbl_Url = gtk_label_new (_("Url"));
  gtk_widget_show (lbl_Url);
  gtk_table_attach (GTK_TABLE (tbl_ConInfo), lbl_Url, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_Url), 0, 0.5);

  entry_Url = gtk_entry_new ();
  gtk_widget_show (entry_Url);
  gtk_table_attach (GTK_TABLE (tbl_ConInfo), entry_Url, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry_Url), 9679);

  dialog_action_area = GTK_DIALOG (connectionDialog)->action_area;
  gtk_widget_show (dialog_action_area);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);

  btn_ConnectDialog = gtk_button_new_from_stock ("gtk-connect");
  gtk_widget_show (btn_ConnectDialog);
  gtk_dialog_add_action_widget (GTK_DIALOG (connectionDialog), btn_ConnectDialog, 0);
  GTK_WIDGET_SET_FLAGS (btn_ConnectDialog, GTK_CAN_DEFAULT);

  gint id_response = gtk_dialog_get_response_for_widget(GTK_DIALOG(connectionDialog), GTK_WIDGET(btn_ConnectDialog));
  gtk_dialog_set_default_response(GTK_DIALOG(connectionDialog), id_response);
  gtk_entry_set_activates_default (GTK_ENTRY(entry_Url), TRUE);

  g_signal_connect ((gpointer) connectionDialog, "key_press_event",
                    G_CALLBACK (gst_ipcam_client_on_connectionDialog_key_press_event),
                    NULL);

  g_signal_connect_swapped ((gpointer) connectionDialog, "destroy",
                            G_CALLBACK (gst_ipcam_client_on_connectionDialog_destroy),
                            GTK_OBJECT (NULL));

  g_signal_connect_swapped ((gpointer) btn_ConnectDialog, "clicked",
                            G_CALLBACK (gst_ipcam_client_on_btn_ConnectDialog_clicked),
                            GTK_OBJECT (NULL));

  /** Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (connectionDialog, connectionDialog, "connectionDialog");
  GLADE_HOOKUP_OBJECT_NO_REF (connectionDialog, dialog_vbox, "dialog_vbox");
  GLADE_HOOKUP_OBJECT (connectionDialog, tbl_ConInfo, "tbl_ConInfo");
  GLADE_HOOKUP_OBJECT (connectionDialog, lbl_Url, "lbl_Url");
  GLADE_HOOKUP_OBJECT (connectionDialog, entry_Url, "entry_Url");
  GLADE_HOOKUP_OBJECT_NO_REF (connectionDialog, dialog_action_area, "dialog_action_area");
  GLADE_HOOKUP_OBJECT (connectionDialog, btn_ConnectDialog, "btn_ConnectDialog");

  return connectionDialog;
}

/**
 * init the statusbar widget
 * 
 * @param void
 *
 * @return the statusbar widget
 */
GtkWidget* gst_ipcam_client_init_status_bar()
{
    GtkWidget * separator;

    // init the outside containter
    statusBar_vBox = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(statusBar_vBox);

    separator = gtk_hseparator_new();

    gtk_widget_show(separator);
    gtk_box_pack_start(GTK_BOX(statusBar_vBox), separator, FALSE, FALSE, 0);

    // init the inside containter
    statusBar_hBox = gtk_hbox_new(FALSE, 0);
    gtk_widget_show(statusBar_hBox);

    gtk_widget_set_size_request(statusBar_hBox, 500, 25);
    gtk_box_pack_start(GTK_BOX(statusBar_vBox), statusBar_hBox, FALSE, FALSE, 0);

    // init the status label
    statusBar_labelStatus = gtk_label_new("");
    gtk_widget_show(statusBar_labelStatus);

    gtk_widget_set_size_request(statusBar_labelStatus, 60, -1);
    gtk_box_pack_start(GTK_BOX(statusBar_hBox), statusBar_labelStatus, FALSE, TRUE, 5);

    // separator
    separator = gtk_vseparator_new();
    gtk_widget_show(separator);
    
    gtk_box_pack_start(GTK_BOX(statusBar_hBox), separator, FALSE, FALSE, 0);

    {
        // Video type
        statusBar_labelVideoType = gtk_label_new("");
        gtk_widget_show(statusBar_labelVideoType);
        
        gtk_widget_set_size_request(statusBar_labelVideoType, 120, -1);
        gtk_box_pack_start(GTK_BOX(statusBar_hBox), statusBar_labelVideoType, FALSE, FALSE, 0);
        /*gtk_box_pack_start_defaults(GTK_BOX(statusBar_hBox), statusBar_labelVideoType);*/
        
        // Audio type
        statusBar_labelAudioType = gtk_label_new("");
        gtk_widget_show(statusBar_labelAudioType);
        
        gtk_widget_set_size_request(statusBar_labelAudioType, 120, -1);
        gtk_box_pack_start(GTK_BOX(statusBar_hBox), statusBar_labelAudioType, FALSE, FALSE, 0);
    }

    // separator
    separator = gtk_vseparator_new();
    //gtk_widget_show(separator);

    gtk_box_pack_start(GTK_BOX(statusBar_hBox), separator, FALSE, FALSE, 0);

    // Properties label
    statusBar_labelProperties = gtk_label_new("");
    //gtk_widget_show(statusBar_labelProperties);
    
    //    gtk_widget_set_size_request(__statusBar_labelSong, 500, -1);
    gtk_box_pack_start(GTK_BOX(statusBar_hBox), statusBar_labelProperties, FALSE, TRUE, 5);

    return statusBar_vBox;
}

/**
 *  set the status text on the status bar
 * 
 * @param void
 *
 * @return nothing
 */
void gst_ipcam_client_set_status_text(const gchar* text)
{
    gtk_label_set_text(GTK_LABEL(statusBar_labelStatus), text);
}

/**
 * show the properties
 * 
 * @param name const gchar* the status name
 *
 * @return nothing
 */
void gst_ipcam_client_set_status_properties(const gchar* name)
{
    gtk_label_set_text(GTK_LABEL(statusBar_labelProperties), name);
}

/**
 * show the video type
 * 
 * @param type_name const gchar* the video type
 *
 * @return nothing
 */
void gst_ipcam_client_set_status_Video_Type(const gchar* type_name)
{
    gtk_label_set_text(GTK_LABEL(statusBar_labelVideoType), type_name);
}

/**
 * show the audio type
 *
 * @param type_name const gchar* the audio type
 *
 * @return nothing
 */
void gst_ipcam_client_set_status_Audio_Type(const gchar* type_name)
{
    gtk_label_set_text(GTK_LABEL(statusBar_labelAudioType), type_name);
}