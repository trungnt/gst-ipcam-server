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
  gtk_window_set_title (GTK_WINDOW (mainWindow), _("Demo Client"));
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

  hbox_VideoPrew = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox_VideoPrew);
  gtk_box_pack_start (GTK_BOX (vbox), hbox_VideoPrew, TRUE, TRUE, 0);

  prw_GuestVideo = gtk_drawing_area_new ();//gtk_preview_new (GTK_PREVIEW_COLOR);
  gtk_widget_show (prw_GuestVideo);
  gtk_box_pack_start (GTK_BOX (hbox_VideoPrew), prw_GuestVideo, TRUE, TRUE, 0);

  statusBar = gtk_statusbar_new ();
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
  gtk_window_set_title (GTK_WINDOW (connectionDialog), _("Connection"));
  gtk_window_set_resizable (GTK_WINDOW (connectionDialog), FALSE);
  gtk_window_set_type_hint (GTK_WINDOW (connectionDialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox = GTK_DIALOG (connectionDialog)->vbox;
  gtk_widget_show (dialog_vbox);

  tbl_ConInfo = gtk_table_new (2, 2, FALSE);
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

  lbl_VideoStreamType = gtk_label_new (_("Video stream type"));
  gtk_widget_show (lbl_VideoStreamType);
  gtk_table_attach (GTK_TABLE (tbl_ConInfo), lbl_VideoStreamType, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_VideoStreamType), 0, 0.5);

  cbx_VideoStreamType = gtk_combo_box_entry_new_text ();
  gtk_widget_show (cbx_VideoStreamType);
  gtk_table_attach (GTK_TABLE (tbl_ConInfo), cbx_VideoStreamType, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("Jpeg stream"));
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("Mpeg4 stream"));
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("H264 stream"));

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
  GLADE_HOOKUP_OBJECT (connectionDialog, lbl_VideoStreamType, "lbl_VideoStreamType");
  GLADE_HOOKUP_OBJECT (connectionDialog, cbx_VideoStreamType, "cbx_VideoStreamType");
  GLADE_HOOKUP_OBJECT_NO_REF (connectionDialog, dialog_action_area, "dialog_action_area");
  GLADE_HOOKUP_OBJECT (connectionDialog, btn_ConnectDialog, "btn_ConnectDialog");

  return connectionDialog;
}
