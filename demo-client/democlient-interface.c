/**
 * \file:   democlient-interface.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 9-4-2009
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

/**
 * create the main window
 *
 * @param void
 *
 * @return nothing
 */
GtkWidget*
democlient_create_mainWindow (void)
{
  accel_group = gtk_accel_group_new ();

  mainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (mainWindow), _("Demo Client"));
  gtk_window_set_default_size (GTK_WINDOW (mainWindow), 600, 350);
  ///gtk_window_set_resizable (GTK_WINDOW (mainWindow), FALSE);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (mainWindow), vbox1);

  menubar = gtk_menu_bar_new ();
  gtk_widget_show (menubar);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar, FALSE, FALSE, 0);

  menuitem_File = gtk_menu_item_new_with_mnemonic (_("_File"));
  gtk_widget_show (menuitem_File);
  gtk_container_add (GTK_CONTAINER (menubar), menuitem_File);

  menuitem_File_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_File), menuitem_File_menu);

  separatormenuitem1 = gtk_separator_menu_item_new ();
  gtk_widget_show (separatormenuitem1);
  gtk_container_add (GTK_CONTAINER (menuitem_File_menu), separatormenuitem1);
  gtk_widget_set_sensitive (separatormenuitem1, FALSE);

  menuBtn_Quit = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (menuBtn_Quit);
  gtk_container_add (GTK_CONTAINER (menuitem_File_menu), menuBtn_Quit);

  menuitem_Options = gtk_menu_item_new_with_mnemonic (_("_Options"));
  gtk_widget_show (menuitem_Options);
  gtk_container_add (GTK_CONTAINER (menubar), menuitem_Options);

  menuitem_Options_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_Options), menuitem_Options_menu);

  menuBtn_Preferences = gtk_image_menu_item_new_from_stock ("gtk-preferences", accel_group);
  gtk_widget_show (menuBtn_Preferences);
  gtk_container_add (GTK_CONTAINER (menuitem_Options_menu), menuBtn_Preferences);

  menuitem_Help = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (menuitem_Help);
  gtk_container_add (GTK_CONTAINER (menubar), menuitem_Help);

  menuitem_Help_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_Help), menuitem_Help_menu);

  menuBtn_about = gtk_menu_item_new_with_mnemonic (_("_About"));
  gtk_widget_show (menuBtn_about);
  gtk_container_add (GTK_CONTAINER (menuitem_Help_menu), menuBtn_about);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox3);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox3, TRUE, TRUE, 0);

  table1 = gtk_table_new (3, 3, FALSE);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (vbox3), table1, TRUE, TRUE, 0);

  vseparator4 = gtk_vseparator_new ();
  gtk_widget_show (vseparator4);
  gtk_table_attach (GTK_TABLE (table1), vseparator4, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  vseparator3 = gtk_vseparator_new ();
  gtk_widget_show (vseparator3);
  gtk_table_attach (GTK_TABLE (table1), vseparator3, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  hseparator2 = gtk_hseparator_new ();
  gtk_widget_show (hseparator2);
  gtk_table_attach (GTK_TABLE (table1), hseparator2, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  vseparator5 = gtk_vseparator_new ();
  gtk_widget_show (vseparator5);
  gtk_table_attach (GTK_TABLE (table1), vseparator5, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  hseparator3 = gtk_hseparator_new ();
  gtk_widget_show (hseparator3);
  gtk_table_attach (GTK_TABLE (table1), hseparator3, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  hbox6 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox6);
  gtk_table_attach (GTK_TABLE (table1), hbox6, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  hbox_ActionButton1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox_ActionButton1);
  gtk_box_pack_start (GTK_BOX (hbox6), hbox_ActionButton1, TRUE, TRUE, 0);

  btn_Connect1 = gtk_button_new_from_stock ("gtk-connect");
  gtk_widget_show (btn_Connect1);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton1), btn_Connect1, FALSE, FALSE, 0);
  
  btn_Disconnect1 = gtk_button_new_from_stock ("gtk-disconnect");
  gtk_widget_hide (btn_Disconnect1);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton1), btn_Disconnect1, FALSE, FALSE, 0);
  
  btn_Pause1 = gtk_button_new_from_stock ("gtk-media-pause");
  gtk_widget_show (btn_Pause1);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton1), btn_Pause1, FALSE, FALSE, 0);

  btn_Resume1 = gtk_button_new_from_stock ("gtk-media-play");
  gtk_widget_hide (btn_Resume1);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton1), btn_Resume1, FALSE, FALSE, 0);
  
  vseparator1 = gtk_vseparator_new ();
  gtk_widget_show (vseparator1);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton1), vseparator1, FALSE, TRUE, 0);

  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox4);
  gtk_box_pack_start (GTK_BOX (hbox6), vbox4, TRUE, TRUE, 0);

  hscale_Fps1 = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 0, 0, 0, 0)));
  gtk_widget_show (hscale_Fps1);
  gtk_box_pack_start (GTK_BOX (vbox4), hscale_Fps1, TRUE, TRUE, 0);

  lbl_fps1 = gtk_label_new (_("Fps    "));
  gtk_widget_show (lbl_fps1);
  gtk_box_pack_start (GTK_BOX (vbox4), lbl_fps1, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_fps1), 0, 0);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox5);
  gtk_box_pack_start (GTK_BOX (hbox6), vbox5, TRUE, TRUE, 0);

  hscale_Bitrate1 = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 0, 0, 0, 0)));
  gtk_widget_show (hscale_Bitrate1);
  gtk_box_pack_start (GTK_BOX (vbox5), hscale_Bitrate1, TRUE, TRUE, 0);

  lbl_bitrate1 = gtk_label_new (_("Bitrate"));
  gtk_widget_show (lbl_bitrate1);
  gtk_box_pack_start (GTK_BOX (vbox5), lbl_bitrate1, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_bitrate1), 0, 0);

  hbox7 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox7);
  gtk_table_attach (GTK_TABLE (table1), hbox7, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  hbox_ActionButton2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox_ActionButton2);
  gtk_box_pack_start (GTK_BOX (hbox7), hbox_ActionButton2, TRUE, TRUE, 0);

  btn_Connect2 = gtk_button_new_from_stock ("gtk-connect");
  gtk_widget_show (btn_Connect2);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton2), btn_Connect2, FALSE, FALSE, 0);

  btn_Disconnect2 = gtk_button_new_from_stock ("gtk-disconnect");
  gtk_widget_hide (btn_Disconnect2);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton2), btn_Disconnect2, FALSE, FALSE, 0);

  btn_Pause2 = gtk_button_new_from_stock ("gtk-media-pause");
  gtk_widget_show (btn_Pause2);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton2), btn_Pause2, FALSE, FALSE, 0);

  btn_Resume2 = gtk_button_new_from_stock ("gtk-media-play");
  gtk_widget_hide (btn_Resume2);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton2), btn_Resume2, FALSE, FALSE, 0);

  vseparator2 = gtk_vseparator_new ();
  gtk_widget_show (vseparator2);
  gtk_box_pack_start (GTK_BOX (hbox_ActionButton2), vseparator2, FALSE, TRUE, 0);

  vbox6 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox6);
  gtk_box_pack_start (GTK_BOX (hbox7), vbox6, TRUE, TRUE, 0);

  hscale_Fps2 = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 0, 0, 0, 0)));
  gtk_widget_show (hscale_Fps2);
  gtk_box_pack_start (GTK_BOX (vbox6), hscale_Fps2, TRUE, TRUE, 0);

  lbl_fps2 = gtk_label_new (_("Fps    "));
  gtk_widget_show (lbl_fps2);
  gtk_box_pack_start (GTK_BOX (vbox6), lbl_fps2, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_fps2), 0, 0);

  vbox7 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox7);
  gtk_box_pack_start (GTK_BOX (hbox7), vbox7, TRUE, TRUE, 0);

  hscale_Bitrate2 = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 0, 0, 0, 0)));
  gtk_widget_show (hscale_Bitrate2);
  gtk_box_pack_start (GTK_BOX (vbox7), hscale_Bitrate2, TRUE, TRUE, 0);

  lbl_bitrate2 = gtk_label_new (_("Bitrate"));
  gtk_widget_show (lbl_bitrate2);
  gtk_box_pack_start (GTK_BOX (vbox7), lbl_bitrate2, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_bitrate2), 0, 0);

  prw1 = gtk_preview_new (GTK_PREVIEW_COLOR);
  gtk_widget_show (prw1);
  gtk_table_attach (GTK_TABLE (table1), prw1, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  prw2 = gtk_preview_new (GTK_PREVIEW_COLOR);
  gtk_widget_show (prw2);
  gtk_table_attach (GTK_TABLE (table1), prw2, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  statusBar = gtk_statusbar_new ();
  gtk_widget_show (statusBar);
  gtk_box_pack_start (GTK_BOX (vbox1), statusBar, FALSE, FALSE, 0);
  gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (statusBar), FALSE);

  g_signal_connect ((gpointer) menuBtn_Quit, "activate",
                    G_CALLBACK (democlient_on_menuBtn_quit_activate),
                    NULL);
  g_signal_connect ((gpointer) menuBtn_Preferences, "activate",
                    G_CALLBACK (democlient_on_menuBtn_Options_activate),
                    NULL);
  g_signal_connect ((gpointer) menuBtn_about, "activate",
                    G_CALLBACK (democlient_on_menuBtn_about_activate),
                    NULL);

  g_signal_connect_swapped ((gpointer) btn_Connect1, "clicked",
                            G_CALLBACK (democlient_on_btn_Connect1_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_Disconnect1, "clicked",
                            G_CALLBACK (democlient_on_btn_Disconnect1_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_Pause1, "clicked",
                            G_CALLBACK (democlient_on_btn_Pause1_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_Resume1, "clicked",
                            G_CALLBACK (democlient_on_btn_Resume1_clicked),
                            GTK_OBJECT (NULL));
  
  g_signal_connect_swapped ((gpointer) btn_Connect2, "clicked",
                            G_CALLBACK (democlient_on_btn_Connect2_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_Disconnect2, "clicked",
                            G_CALLBACK (democlient_on_btn_Disconnect2_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_Pause2, "clicked",
                            G_CALLBACK (democlient_on_btn_Pause2_clicked),
                            GTK_OBJECT (NULL));
  g_signal_connect_swapped ((gpointer) btn_Resume2, "clicked",
                            G_CALLBACK (democlient_on_btn_Resume2_clicked),
                            GTK_OBJECT (NULL));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (mainWindow, mainWindow, "mainWindow");
  GLADE_HOOKUP_OBJECT (mainWindow, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (mainWindow, menubar, "menubar");
  GLADE_HOOKUP_OBJECT (mainWindow, menuitem_File, "menuitem_File");
  GLADE_HOOKUP_OBJECT (mainWindow, menuitem_File_menu, "menuitem_File_menu");
  GLADE_HOOKUP_OBJECT (mainWindow, separatormenuitem1, "separatormenuitem1");
  GLADE_HOOKUP_OBJECT (mainWindow, menuBtn_Quit, "menuBtn_Quit");
  GLADE_HOOKUP_OBJECT (mainWindow, menuitem_Options, "menuitem_Options");
  GLADE_HOOKUP_OBJECT (mainWindow, menuitem_Options_menu, "menuitem_Options_menu");
  GLADE_HOOKUP_OBJECT (mainWindow, menuBtn_Preferences, "menuBtn_Preferences");
  GLADE_HOOKUP_OBJECT (mainWindow, menuitem_Help, "menuitem_Help");
  GLADE_HOOKUP_OBJECT (mainWindow, menuitem_Help_menu, "menuitem_Help_menu");
  GLADE_HOOKUP_OBJECT (mainWindow, menuBtn_about, "menuBtn_about");
  GLADE_HOOKUP_OBJECT (mainWindow, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (mainWindow, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (mainWindow, table1, "table1");
  GLADE_HOOKUP_OBJECT (mainWindow, vseparator4, "vseparator4");
  GLADE_HOOKUP_OBJECT (mainWindow, vseparator3, "vseparator3");
  GLADE_HOOKUP_OBJECT (mainWindow, hseparator2, "hseparator2");
  GLADE_HOOKUP_OBJECT (mainWindow, vseparator5, "vseparator5");
  GLADE_HOOKUP_OBJECT (mainWindow, hseparator3, "hseparator3");
  GLADE_HOOKUP_OBJECT (mainWindow, hbox6, "hbox6");
  GLADE_HOOKUP_OBJECT (mainWindow, hbox_ActionButton1, "hbox_ActionButton1");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Connect1, "btn_Connect1");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Pause1, "btn_Pause1");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Disconnect1, "btn_Disconnect1");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Resume1, "btn_Resume1");
  GLADE_HOOKUP_OBJECT (mainWindow, vseparator1, "vseparator1");
  GLADE_HOOKUP_OBJECT (mainWindow, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (mainWindow, hscale_Fps1, "hscale_Fps1");
  GLADE_HOOKUP_OBJECT (mainWindow, lbl_fps1, "lbl_fps1");
  GLADE_HOOKUP_OBJECT (mainWindow, vbox5, "vbox5");
  GLADE_HOOKUP_OBJECT (mainWindow, hscale_Bitrate1, "hscale_Bitrate1");
  GLADE_HOOKUP_OBJECT (mainWindow, lbl_bitrate1, "lbl_bitrate1");
  GLADE_HOOKUP_OBJECT (mainWindow, hbox7, "hbox7");
  GLADE_HOOKUP_OBJECT (mainWindow, hbox_ActionButton2, "hbox_ActionButton2");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Connect2, "btn_Connect2");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Pause2, "btn_Pause2");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Disconnect2, "btn_Disconnect2");
  GLADE_HOOKUP_OBJECT (mainWindow, btn_Resume2, "btn_Resume2");
  GLADE_HOOKUP_OBJECT (mainWindow, vseparator2, "vseparator2");
  GLADE_HOOKUP_OBJECT (mainWindow, vbox6, "vbox6");
  GLADE_HOOKUP_OBJECT (mainWindow, hscale_Fps2, "hscale_Fps2");
  GLADE_HOOKUP_OBJECT (mainWindow, lbl_fps2, "lbl_fps2");
  GLADE_HOOKUP_OBJECT (mainWindow, vbox7, "vbox7");
  GLADE_HOOKUP_OBJECT (mainWindow, hscale_Bitrate2, "hscale_Bitrate2");
  GLADE_HOOKUP_OBJECT (mainWindow, lbl_bitrate2, "lbl_bitrate2");
  GLADE_HOOKUP_OBJECT (mainWindow, prw1, "prw1");
  GLADE_HOOKUP_OBJECT (mainWindow, prw2, "prw2");
  GLADE_HOOKUP_OBJECT (mainWindow, statusBar, "statusBar");

  gtk_window_add_accel_group (GTK_WINDOW (mainWindow), accel_group);

  return mainWindow;
}

/**
 * create the option dialog
 *
 * @param void
 *
 * @return nothing
 */
GtkWidget*
democlient_create_optionDialog (void)
{
  optionDialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (optionDialog), _("Options"));
  gtk_window_set_resizable (GTK_WINDOW (optionDialog), FALSE);
  gtk_window_set_destroy_with_parent (GTK_WINDOW (optionDialog), TRUE);
  gtk_window_set_type_hint (GTK_WINDOW (optionDialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox = GTK_DIALOG (optionDialog)->vbox;
  gtk_widget_show (dialog_vbox);

  tbl_Options = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (tbl_Options);
  gtk_box_pack_start (GTK_BOX (dialog_vbox), tbl_Options, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_Options), 42);
  gtk_table_set_row_spacings (GTK_TABLE (tbl_Options), 13);
  gtk_table_set_col_spacings (GTK_TABLE (tbl_Options), 32);

  lbl_Url = gtk_label_new (_("Url"));
  gtk_widget_show (lbl_Url);
  gtk_table_attach (GTK_TABLE (tbl_Options), lbl_Url, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_Url), 0, 0.5);

  entry_Url = gtk_entry_new ();
  gtk_widget_show (entry_Url);
  gtk_table_attach (GTK_TABLE (tbl_Options), entry_Url, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry_Url), 9679);

  lbl_VideoStreamType = gtk_label_new (_("Video stream type"));
  gtk_widget_show (lbl_VideoStreamType);
  gtk_table_attach (GTK_TABLE (tbl_Options), lbl_VideoStreamType, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_VideoStreamType), 0, 0.5);

  cbx_VideoStreamType = gtk_combo_box_entry_new_text ();
  gtk_widget_show (cbx_VideoStreamType);
  gtk_table_attach (GTK_TABLE (tbl_Options), cbx_VideoStreamType, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("Jpeg stream"));
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("Mpeg4 stream"));
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("H264"));
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("Jpeg & Mpeg4 stream"));
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("Mpeg4 & H274 stream"));
  gtk_combo_box_append_text (GTK_COMBO_BOX (cbx_VideoStreamType), _("H264 & Jpeg stream"));

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
  GLADE_HOOKUP_OBJECT (optionDialog, tbl_Options, "tbl_Options");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_Url, "lbl_Url");
  GLADE_HOOKUP_OBJECT (optionDialog, entry_Url, "entry_Url");
  GLADE_HOOKUP_OBJECT (optionDialog, lbl_VideoStreamType, "lbl_VideoStreamType");
  GLADE_HOOKUP_OBJECT (optionDialog, cbx_VideoStreamType, "cbx_VideoStreamType");
  GLADE_HOOKUP_OBJECT_NO_REF (optionDialog, dialog_action_area, "dialog_action_area");
  GLADE_HOOKUP_OBJECT (optionDialog, btn_Cancel, "btn_Cancel");
  GLADE_HOOKUP_OBJECT (optionDialog, btn_Ok, "btn_Ok");

  return optionDialog;
}

