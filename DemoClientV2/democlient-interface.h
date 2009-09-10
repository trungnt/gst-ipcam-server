/**
 * \file:   democlient-interface.h
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 9-4-2009
 */

GtkWidget* democlient_create_mainWindow (void);
GtkWidget* democlient_create_optionDialog (void);

///For main Window
GtkWidget *mainWindow;
GtkWidget *vbox1;
GtkWidget *menubar;
GtkWidget *menuitem_File;
GtkWidget *menuitem_File_menu;
GtkWidget *separatormenuitem1;
GtkWidget *menuBtn_Quit;
GtkWidget *menuitem_Options;
GtkWidget *menuitem_Options_menu;
GtkWidget *menuBtn_Preferences;
GtkWidget *menuitem_Help;
GtkWidget *menuitem_Help_menu;
GtkWidget *menuBtn_about;
GtkWidget *hbox1;
GtkWidget *vbox3;
GtkWidget *table1;
GtkWidget *vseparator4;
GtkWidget *vseparator3;
GtkWidget *hseparator2;
GtkWidget *vseparator5;
GtkWidget *hseparator3;
GtkWidget *hbox6;
GtkWidget *hbox_ActionButton1;
GtkWidget *btn_Connect1;
GtkWidget *btn_Disconnect1;
GtkWidget *btn_Pause1;
GtkWidget *btn_Resume1;
GtkWidget *vseparator1;
GtkWidget *vbox4;
GtkWidget *hscale_Fps1;
GtkWidget *lbl_fps1;
GtkWidget *vbox5;
GtkWidget *hscale_Bitrate1;
GtkWidget *lbl_bitrate1;
GtkWidget *hbox7;
GtkWidget *hbox_ActionButton2;
GtkWidget *btn_Connect2;
GtkWidget *btn_Disconnect2;
GtkWidget *btn_Pause2;
GtkWidget *btn_Resume2;
GtkWidget *vseparator2;
GtkWidget *vbox6;
GtkWidget *hscale_Fps2;
GtkWidget *lbl_fps2;
GtkWidget *vbox7;
GtkWidget *hscale_Bitrate2;
GtkWidget *lbl_bitrate2;
GtkWidget *prw1;
GtkWidget *prw2;
GtkWidget *statusBar;
GtkAccelGroup *accel_group;

///For Options Dialog
GtkWidget *optionDialog;
GtkWidget *dialog_vbox;
GtkWidget *tbl_Options;
GtkWidget *lbl_Url;
GtkWidget *entry_Url;
GtkWidget *lbl_VideoStreamType;
GtkWidget *cbx_VideoStreamType;
GtkWidget *dialog_action_area;
GtkWidget *btn_Cancel;
GtkWidget *btn_Ok;