/*
 * File:   democlient-interface.c
 * Author: damquang.tuan@nomovok.com
 *
 * Created on August 26, 2009, 10:04 AM
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

#include "democlient-callbacks.h"
#include "democlient-interface.h"
#include "democlient-support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)
  
GtkWidget*
democlient_create_mainWindow (void)
{
  mainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (mainWindow), _("Video Conferencing"));
  gtk_window_set_default_size (GTK_WINDOW (mainWindow), 400, 200);
  //gtk_window_set_resizable (GTK_WINDOW (mainWindow), FALSE);

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
  gtk_widget_show (btn_Disconnect);
  gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);

  toolitem_Pause = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem_Pause);
  gtk_container_add (GTK_CONTAINER (toolbar), toolitem_Pause);

  btn_Pause = gtk_button_new_from_stock ("gtk-media-pause");
  gtk_widget_show (btn_Pause);
  gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Pause);

  btn_Resume = gtk_button_new ();
  gtk_widget_show (btn_Resume);

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

  toolitem_Options = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem_Options);
  gtk_container_add (GTK_CONTAINER (toolbar), toolitem_Options);

  btn_Options = gtk_button_new ();
  gtk_widget_show (btn_Options);
  gtk_container_add (GTK_CONTAINER (toolitem_Options), btn_Options);

  algn_Options = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (algn_Options);
  gtk_container_add (GTK_CONTAINER (btn_Options), algn_Options);

  hbox_OptionsButton = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_OptionsButton);
  gtk_container_add (GTK_CONTAINER (algn_Options), hbox_OptionsButton);

  img_Options = gtk_image_new_from_stock ("gtk-preferences", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (img_Options);
  gtk_box_pack_start (GTK_BOX (hbox_OptionsButton), img_Options, FALSE, FALSE, 0);

  lbl_OptionsButton = gtk_label_new_with_mnemonic ("_Options");
  gtk_widget_show (lbl_OptionsButton);
  gtk_box_pack_start (GTK_BOX (hbox_OptionsButton), lbl_OptionsButton, FALSE, FALSE, 0);

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

  prw_GuestVideo = gtk_preview_new (GTK_PREVIEW_COLOR);
  gtk_widget_show (prw_GuestVideo);
  gtk_box_pack_start (GTK_BOX (hbox_VideoPrew), prw_GuestVideo, TRUE, TRUE, 0);

  prw_HostVideo = gtk_preview_new (GTK_PREVIEW_COLOR);
  gtk_widget_show (prw_HostVideo);
  gtk_box_pack_start (GTK_BOX (hbox_VideoPrew), prw_HostVideo, TRUE, TRUE, 0);

  statusBar = gtk_statusbar_new ();
  gtk_widget_show (statusBar);
  gtk_box_pack_start (GTK_BOX (vbox), statusBar, FALSE, FALSE, 0);
  gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (statusBar), FALSE);

  g_signal_connect_swapped ((gpointer) btn_Connect, "clicked",
                            G_CALLBACK (democlient_on_btn_Connect_clicked),
                            GTK_OBJECT (NULL));

  g_signal_connect_swapped ((gpointer) btn_Disconnect, "clicked",
                            G_CALLBACK (democlient_on_btn_Disconnect_clicked),
                            GTK_OBJECT (NULL));

  g_signal_connect_swapped ((gpointer) btn_Pause, "clicked",
                            G_CALLBACK (democlient_on_btn_Pause_clicked),
                            GTK_OBJECT (NULL));
  
  g_signal_connect_swapped ((gpointer) btn_Resume, "clicked",
                            G_CALLBACK (democlient_on_btn_Resume_clicked),
                            GTK_OBJECT (NULL));

  g_signal_connect_swapped ((gpointer) btn_Options, "clicked",
                            G_CALLBACK (democlient_on_btn_Options_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_About, "clicked",
                            G_CALLBACK (democlient_on_btn_About_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_Quit, "clicked",
                            G_CALLBACK (democlient_on_btn_Quit_clicked),
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
  GLADE_HOOKUP_OBJECT (mainWindow, prw_HostVideo, "prw_HostVideo");
  GLADE_HOOKUP_OBJECT (mainWindow, statusBar, "statusBar");

  return mainWindow;
}

GtkWidget*
democlient_create_connectionDialog (void)
{
  GtkWidget *connectionDialog;
  GtkWidget *dialog_vbox;
  GtkWidget *tbl_ConInfo;
  GtkWidget *lbl_Url;
  GtkWidget *entry_Url;
  GtkWidget *lbl_VideoStreamType;
  GtkWidget *cbx_VideoStreamType;
  GtkWidget *dialog_action_area;
  GtkWidget *btn_ConnectDialog;

  connectionDialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (connectionDialog), _("Connection"));
  gtk_window_set_resizable (GTK_WINDOW (connectionDialog), FALSE);
  gtk_window_set_type_hint (GTK_WINDOW (connectionDialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox = GTK_DIALOG (connectionDialog)->vbox;
  gtk_widget_show (dialog_vbox);

  tbl_ConInfo = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (tbl_ConInfo);
  gtk_box_pack_start (GTK_BOX (dialog_vbox), tbl_ConInfo, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_ConInfo), 42);
  gtk_table_set_row_spacings (GTK_TABLE (tbl_ConInfo), 13);
  gtk_table_set_col_spacings (GTK_TABLE (tbl_ConInfo), 32);

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
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("H264"));

  dialog_action_area = GTK_DIALOG (connectionDialog)->action_area;
  gtk_widget_show (dialog_action_area);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);

  btn_ConnectDialog = gtk_button_new_from_stock ("gtk-connect");
  gtk_widget_show (btn_ConnectDialog);
  gtk_dialog_add_action_widget (GTK_DIALOG (connectionDialog), btn_ConnectDialog, 0);
  GTK_WIDGET_SET_FLAGS (btn_ConnectDialog, GTK_CAN_DEFAULT);

  g_signal_connect_swapped ((gpointer) btn_ConnectDialog, "clicked",
                            G_CALLBACK (democlient_on_btn_ConnectDialog_clicked),
                            GTK_OBJECT (NULL));

  /* Store pointers to all widgets, for use by lookup_widget(). */
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

GtkWidget*
democlient_create_optionDialog (void)
{
  GtkWidget *optionDialog;
  GtkWidget *dialog_vbox;
  GtkWidget *notebook;
  GtkWidget *vbox2;
  GtkWidget *frame_En_Decoding_Software;
  GtkWidget *algn_En_DecodingSoftware;
  GtkWidget *vbox3;
  GtkWidget *radiobtn_EncodingSoftware;
  GSList *radiobtn_EncodingSoftware_group = NULL;
  GtkWidget *radiobtn_DecodingSoftware;
  GtkWidget *lbl_En_DecodingSoftware;
  GtkWidget *frame_En_Decoding_Hardware;
  GtkWidget *aln_En_DecodingHardware;
  GtkWidget *vbox4;
  GtkWidget *radiobtn_EncodingHardware;
  GSList *radiobtn_EncodingHardware_group = NULL;
  GtkWidget *radiobtn_DecodingHardware;
  GtkWidget *lbl_En_DecodingHardware;
  GtkWidget *lbl_En_DecodingTypes;
  GtkWidget *empty_notebook_page;
  GtkWidget *lbl_En_DecodingProfiles;
  GtkWidget *vbox5;
  GtkWidget *frame_ConnPara;
  GtkWidget *alignment3;
  GtkWidget *tbl_ConnPar;
  GtkWidget *lbl_VideoPort;
  GtkWidget *entry_VideoPort;
  GtkWidget *lbl_AudioPort;
  GtkWidget *entry_AudioPort;
  GtkWidget *lbl_ConnPar;
  GtkWidget *frame_OtherOpts;
  GtkWidget *alignment;
  GtkWidget *hbox3;
  GtkWidget *lbl_ScreenRefreshRate;
  GtkWidget *entry_ScreenRefeshRate;
  GtkWidget *lbl_OtherOpts;
  GtkWidget *lbl_UserSpecificSettings;
  GtkWidget *dialog_action_area;
  GtkWidget *btn_Cancel;
  GtkWidget *btn_Ok;

  optionDialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (optionDialog), _("Options"));
  gtk_window_set_type_hint (GTK_WINDOW (optionDialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox = GTK_DIALOG (optionDialog)->vbox;
  gtk_widget_show (dialog_vbox);

  notebook = gtk_notebook_new ();
  gtk_widget_show (notebook);
  gtk_box_pack_start (GTK_BOX (dialog_vbox), notebook, TRUE, TRUE, 0);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (notebook), vbox2);

  frame_En_Decoding_Software = gtk_frame_new (NULL);
  gtk_widget_show (frame_En_Decoding_Software);
  gtk_box_pack_start (GTK_BOX (vbox2), frame_En_Decoding_Software, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame_En_Decoding_Software), 30);
  gtk_frame_set_shadow_type (GTK_FRAME (frame_En_Decoding_Software), GTK_SHADOW_NONE);

  algn_En_DecodingSoftware = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (algn_En_DecodingSoftware);
  gtk_container_add (GTK_CONTAINER (frame_En_Decoding_Software), algn_En_DecodingSoftware);
  gtk_alignment_set_padding (GTK_ALIGNMENT (algn_En_DecodingSoftware), 0, 0, 12, 0);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (algn_En_DecodingSoftware), vbox3);

  radiobtn_EncodingSoftware = gtk_radio_button_new_with_mnemonic (NULL, _("Encoding software"));
  gtk_widget_show (radiobtn_EncodingSoftware);
  gtk_box_pack_start (GTK_BOX (vbox3), radiobtn_EncodingSoftware, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (radiobtn_EncodingSoftware), 3);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobtn_EncodingSoftware), radiobtn_EncodingSoftware_group);
  radiobtn_EncodingSoftware_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobtn_EncodingSoftware));

  radiobtn_DecodingSoftware = gtk_radio_button_new_with_mnemonic (NULL, _("Decoding software"));
  gtk_widget_show (radiobtn_DecodingSoftware);
  gtk_box_pack_start (GTK_BOX (vbox3), radiobtn_DecodingSoftware, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (radiobtn_DecodingSoftware), 3);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobtn_DecodingSoftware), radiobtn_EncodingSoftware_group);
  radiobtn_EncodingSoftware_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobtn_DecodingSoftware));

  lbl_En_DecodingSoftware = gtk_label_new (_("En/Decoding software:"));
  gtk_widget_show (lbl_En_DecodingSoftware);
  gtk_frame_set_label_widget (GTK_FRAME (frame_En_Decoding_Software), lbl_En_DecodingSoftware);
  gtk_misc_set_alignment (GTK_MISC (lbl_En_DecodingSoftware), 0.11, 0.5);

  frame_En_Decoding_Hardware = gtk_frame_new (NULL);
  gtk_widget_show (frame_En_Decoding_Hardware);
  gtk_box_pack_start (GTK_BOX (vbox2), frame_En_Decoding_Hardware, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame_En_Decoding_Hardware), 30);
  gtk_frame_set_shadow_type (GTK_FRAME (frame_En_Decoding_Hardware), GTK_SHADOW_NONE);

  aln_En_DecodingHardware = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (aln_En_DecodingHardware);
  gtk_container_add (GTK_CONTAINER (frame_En_Decoding_Hardware), aln_En_DecodingHardware);
  gtk_alignment_set_padding (GTK_ALIGNMENT (aln_En_DecodingHardware), 0, 0, 12, 0);

  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox4);
  gtk_container_add (GTK_CONTAINER (aln_En_DecodingHardware), vbox4);

  radiobtn_EncodingHardware = gtk_radio_button_new_with_mnemonic (NULL, _("Encoding hardware"));
  gtk_widget_show (radiobtn_EncodingHardware);
  gtk_box_pack_start (GTK_BOX (vbox4), radiobtn_EncodingHardware, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (radiobtn_EncodingHardware), 3);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobtn_EncodingHardware), radiobtn_EncodingHardware_group);
  radiobtn_EncodingHardware_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobtn_EncodingHardware));

  radiobtn_DecodingHardware = gtk_radio_button_new_with_mnemonic (NULL, _("Decoding hardware"));
  gtk_widget_show (radiobtn_DecodingHardware);
  gtk_box_pack_start (GTK_BOX (vbox4), radiobtn_DecodingHardware, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (radiobtn_DecodingHardware), 3);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobtn_DecodingHardware), radiobtn_EncodingHardware_group);
  radiobtn_EncodingHardware_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobtn_DecodingHardware));

  lbl_En_DecodingHardware = gtk_label_new (_("En/Decoding Hardware:"));
  gtk_widget_show (lbl_En_DecodingHardware);
  gtk_frame_set_label_widget (GTK_FRAME (frame_En_Decoding_Hardware), lbl_En_DecodingHardware);
  gtk_misc_set_alignment (GTK_MISC (lbl_En_DecodingHardware), 0.11, 0.5);

  lbl_En_DecodingTypes = gtk_label_new (_("En/Decoding types"));
  gtk_widget_show (lbl_En_DecodingTypes);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), lbl_En_DecodingTypes);

  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook), empty_notebook_page);

  lbl_En_DecodingProfiles = gtk_label_new (_("En/Decoding profiles"));
  gtk_widget_show (lbl_En_DecodingProfiles);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 1), lbl_En_DecodingProfiles);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox5);
  gtk_container_add (GTK_CONTAINER (notebook), vbox5);

  frame_ConnPara = gtk_frame_new (NULL);
  gtk_widget_show (frame_ConnPara);
  gtk_box_pack_start (GTK_BOX (vbox5), frame_ConnPara, TRUE, TRUE, 0);

  alignment3 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment3);
  gtk_container_add (GTK_CONTAINER (frame_ConnPara), alignment3);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment3), 0, 0, 12, 0);

  tbl_ConnPar = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (tbl_ConnPar);
  gtk_container_add (GTK_CONTAINER (alignment3), tbl_ConnPar);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_ConnPar), 30);
  gtk_table_set_row_spacings (GTK_TABLE (tbl_ConnPar), 14);
  gtk_table_set_col_spacings (GTK_TABLE (tbl_ConnPar), 57);

  lbl_VideoPort = gtk_label_new (_("Video port:"));
  gtk_widget_show (lbl_VideoPort);
  gtk_table_attach (GTK_TABLE (tbl_ConnPar), lbl_VideoPort, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_VideoPort), 0, 0.5);

  entry_VideoPort = gtk_entry_new ();
  gtk_widget_show (entry_VideoPort);
  gtk_table_attach (GTK_TABLE (tbl_ConnPar), entry_VideoPort, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry_VideoPort), 9679);

  lbl_AudioPort = gtk_label_new (_("Audio port:"));
  gtk_widget_show (lbl_AudioPort);
  gtk_table_attach (GTK_TABLE (tbl_ConnPar), lbl_AudioPort, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_AudioPort), 0, 0.5);

  entry_AudioPort = gtk_entry_new ();
  gtk_widget_show (entry_AudioPort);
  gtk_table_attach (GTK_TABLE (tbl_ConnPar), entry_AudioPort, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry_AudioPort), 9679);

  lbl_ConnPar = gtk_label_new (_("<b>Connection parameters</b>"));
  gtk_widget_show (lbl_ConnPar);
  gtk_frame_set_label_widget (GTK_FRAME (frame_ConnPara), lbl_ConnPar);
  gtk_label_set_use_markup (GTK_LABEL (lbl_ConnPar), TRUE);

  frame_OtherOpts = gtk_frame_new (NULL);
  gtk_widget_show (frame_OtherOpts);
  gtk_box_pack_start (GTK_BOX (vbox5), frame_OtherOpts, TRUE, TRUE, 0);

  alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment);
  gtk_container_add (GTK_CONTAINER (frame_OtherOpts), alignment);
  gtk_container_set_border_width (GTK_CONTAINER (alignment), 4);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 2, 0, 33, 25);

  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox3);
  gtk_container_add (GTK_CONTAINER (alignment), hbox3);

  lbl_ScreenRefreshRate = gtk_label_new (_("Screen refresh rate:"));
  gtk_widget_show (lbl_ScreenRefreshRate);
  gtk_box_pack_start (GTK_BOX (hbox3), lbl_ScreenRefreshRate, FALSE, FALSE, 0);

  entry_ScreenRefeshRate = gtk_entry_new ();
  gtk_widget_show (entry_ScreenRefeshRate);
  gtk_box_pack_start (GTK_BOX (hbox3), entry_ScreenRefeshRate, TRUE, TRUE, 0);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry_ScreenRefeshRate), 9679);

  lbl_OtherOpts = gtk_label_new (_("<b>Other options</b>"));
  gtk_widget_show (lbl_OtherOpts);
  gtk_frame_set_label_widget (GTK_FRAME (frame_OtherOpts), lbl_OtherOpts);
  gtk_label_set_use_markup (GTK_LABEL (lbl_OtherOpts), TRUE);

  lbl_UserSpecificSettings = gtk_label_new (_("User-specific settings"));
  gtk_widget_show (lbl_UserSpecificSettings);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 2), lbl_UserSpecificSettings);

  dialog_action_area = GTK_DIALOG (optionDialog)->action_area;
  gtk_widget_show (dialog_action_area);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);

  btn_Cancel = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (btn_Cancel);
  gtk_dialog_add_action_widget (GTK_DIALOG (optionDialog), btn_Cancel, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (btn_Cancel, GTK_CAN_DEFAULT);

  btn_Ok = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (btn_Ok);
  gtk_dialog_add_action_widget (GTK_DIALOG (optionDialog), btn_Ok, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (btn_Ok, GTK_CAN_DEFAULT);

  g_signal_connect_swapped ((gpointer) btn_Cancel, "clicked",
                            G_CALLBACK (democlient_on_btn_Cancel_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_Ok, "clicked",
                            G_CALLBACK (democlient_on_btn_Ok_clicked),
                            GTK_OBJECT (NULL));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (optionDialog, optionDialog, "optionDialog");
  GLADE_HOOKUP_OBJECT_NO_REF (optionDialog, dialog_vbox, "dialog_vbox");
  GLADE_HOOKUP_OBJECT (optionDialog, notebook, "notebook");
  GLADE_HOOKUP_OBJECT (optionDialog, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (optionDialog, frame_En_Decoding_Software, "frame_En_Decoding_Software");
  GLADE_HOOKUP_OBJECT (optionDialog, algn_En_DecodingSoftware, "algn_En_DecodingSoftware");
  GLADE_HOOKUP_OBJECT (optionDialog, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (optionDialog, radiobtn_EncodingSoftware, "radiobtn_EncodingSoftware");
  GLADE_HOOKUP_OBJECT (optionDialog, radiobtn_DecodingSoftware, "radiobtn_DecodingSoftware");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_En_DecodingSoftware, "lbl_En_DecodingSoftware");
  GLADE_HOOKUP_OBJECT (optionDialog, frame_En_Decoding_Hardware, "frame_En_Decoding_Hardware");
  GLADE_HOOKUP_OBJECT (optionDialog, aln_En_DecodingHardware, "aln_En_DecodingHardware");
  GLADE_HOOKUP_OBJECT (optionDialog, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (optionDialog, radiobtn_EncodingHardware, "radiobtn_EncodingHardware");
  GLADE_HOOKUP_OBJECT (optionDialog, radiobtn_DecodingHardware, "radiobtn_DecodingHardware");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_En_DecodingHardware, "lbl_En_DecodingHardware");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_En_DecodingTypes, "lbl_En_DecodingTypes");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_En_DecodingProfiles, "lbl_En_DecodingProfiles");
  GLADE_HOOKUP_OBJECT (optionDialog, vbox5, "vbox5");
  GLADE_HOOKUP_OBJECT (optionDialog, frame_ConnPara, "frame_ConnPara");
  GLADE_HOOKUP_OBJECT (optionDialog, alignment3, "alignment3");
  GLADE_HOOKUP_OBJECT (optionDialog, tbl_ConnPar, "tbl_ConnPar");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_VideoPort, "lbl_VideoPort");
  GLADE_HOOKUP_OBJECT (optionDialog, entry_VideoPort, "entry_VideoPort");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_AudioPort, "lbl_AudioPort");
  GLADE_HOOKUP_OBJECT (optionDialog, entry_AudioPort, "entry_AudioPort");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_ConnPar, "lbl_ConnPar");
  GLADE_HOOKUP_OBJECT (optionDialog, frame_OtherOpts, "frame_OtherOpts");
  GLADE_HOOKUP_OBJECT (optionDialog, alignment, "alignment");
  GLADE_HOOKUP_OBJECT (optionDialog, hbox3, "hbox3");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_ScreenRefreshRate, "lbl_ScreenRefreshRate");
  GLADE_HOOKUP_OBJECT (optionDialog, entry_ScreenRefeshRate, "entry_ScreenRefeshRate");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_OtherOpts, "lbl_OtherOpts");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_UserSpecificSettings, "lbl_UserSpecificSettings");
  GLADE_HOOKUP_OBJECT_NO_REF (optionDialog, dialog_action_area, "dialog_action_area");
  GLADE_HOOKUP_OBJECT (optionDialog, btn_Cancel, "btn_Cancel");
  GLADE_HOOKUP_OBJECT (optionDialog, btn_Ok, "btn_Ok");

  return optionDialog;
}

