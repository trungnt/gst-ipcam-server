/*
 * \file:   gst-ipcam-client-interface.h
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 8-26-2009
 */

#include <gtk/gtk.h>

///For main Dialog
GtkWidget *mainWindow;
GtkWidget *vbox;
GtkWidget *toolbar;
GtkIconSize tmp_toolbar_icon_size;
GtkWidget *toolitem_Connect;
GtkWidget *btn_Connect;
GtkWidget *btn_Disconnect;

GtkWidget *toolitem_Pause;
GtkWidget *btn_Pause;
GtkWidget *btn_Resume;
GtkWidget *img_Resume;
GtkWidget *algn_Resume;
GtkWidget *hbox_ResumeButton;
GtkWidget *lbl_ResumeButton;
GtkWidget *toolitem_Options;
GtkWidget *btn_Options;
GtkWidget *img_Options;
GtkWidget *algn_Options;
GtkWidget *hbox_OptionsButton;
GtkWidget *lbl_OptionsButton;
GtkWidget *toolitem_About;
GtkWidget *btn_About;
GtkWidget *toolitem_Quit;
GtkWidget *btn_Quit;
GtkWidget *hbox1;
GtkWidget *vbox2;
GtkWidget *toolbar1;
GtkWidget *toolitem6;
GtkWidget *lbl_fps;
GtkWidget *toolitem8;
GtkObject *spinbtn_fps_n_adj;
GtkWidget *spinbtn_fps_n;
GtkWidget *toolitem7;
GtkWidget *lbl__;
GtkWidget *toolitem9;
GtkObject *spinbtn_fps_d_adj;
GtkWidget *spinbtn_fps_d;
GtkWidget *toolitem15;
GtkWidget *alignment2;
GtkWidget *toolitem10;
GtkWidget *btn_change_fps;
GtkWidget *toolitem11;
GtkWidget *vseparator1;
GtkWidget *toolitem12;
GtkWidget *lbl_bitrate;
GtkWidget *toolitem13;
GtkObject *spinbtn_bitrate_adj;
GtkWidget *spinbtn_bitrate;
GtkWidget *toolitem14;
GtkWidget *btn_change;
GtkWidget *hbox_VideoPrew;
GtkWidget *prw_GuestVideo;
GtkWidget *statusBar;
GtkWidget * statusBar_vBox; // the vbox used in the statusbar. This widget will be return in the Init function. Infact, it contains a hseparator and a hbox.
GtkWidget * statusBar_hBox; // this hbox will contains all of the labels;
GtkWidget * statusBar_labelStatus; // the label for the status text
GtkWidget * statusBar_labelVideoType;
GtkWidget * statusBar_labelAudioType;
GtkWidget * statusBar_labelProperties; // the label for Properties displaying

///For Connection Dialog
GtkWidget *connectionDialog;
GtkWidget *dialog_vbox;
GtkWidget *tbl_ConInfo;
GtkWidget *lbl_Url;
GtkWidget *entry_Url;
GtkWidget *dialog_action_area;
GtkWidget *btn_ConnectDialog;

static gboolean is_connect_button_clicked = FALSE;
/**
 * create the main window
 *
 * @param void
 *
 * @return nothing
 */
GtkWidget* gst_ipcam_client_create_mainWindow (void);

/**
 * create the connection dialog
 *
 * @param void
 *
 * @return nothing
 */
GtkWidget* gst_ipcam_client_create_connectionDialog (void);

/**
 * init the statusbar widget
 */
GtkWidget* gst_ipcam_client_init_status_bar();

/**
 *  set the status text on the status bar
 *
 * @param void
 *
 * @return nothing
 */
void gst_ipcam_client_set_status_text(const gchar* text);

/**
 * show the properties
 *
 * @param void
 *
 * @return nothing
 */
void gst_ipcam_client_set_status_properties(const gchar* name);

/**
 * show the video type
 *
 * @param type_name const gchar* the video type
 *
 * @return nothing
 */
void gst_ipcam_client_set_status_Video_Type(const gchar* type_name);

/**
 * show the audio type
 *
 * @param type_name const gchar* the audio type
 *
 * @return nothing
 */
void gst_ipcam_client_set_status_Audio_Type(const gchar* type_name);