/*
 * \file:   gst-ipcam-client-interface.h
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 8-26-2009
 */

#include <gtk/gtk.h>

/*For main Dialog*/
GtkWidget *main_window;
GtkWidget *vbox;
GtkWidget *toolbar;
GtkIconSize tmp_toolbar_icon_size;
GtkWidget *toolitem_connect;
GtkWidget *btn_connect;
GtkWidget *btn_disconnect;

GtkWidget *toolitem_pause;
GtkWidget *btn_pause;
GtkWidget *btn_resume;
GtkWidget *img_resume;
GtkWidget *algn_resume;
GtkWidget *hbox_resume_button;
GtkWidget *lbl_resume_button;
GtkWidget *toolitem_about;
GtkWidget *btn_about;
GtkWidget *toolitem_quit;
GtkWidget *btn_quit;
GtkWidget *hbox4;
GtkWidget *vbox2;
GtkWidget *toolbar1;
GtkWidget *lbl_fps;
GtkWidget *lbl_fsize;
GtkWidget *lbl_fps_unit;
GtkWidget *lbl_fsize_unit;
GtkWidget *lbl_bitrate_unit;
GtkWidget *toolitem3;
GtkWidget *toolitem4;
GtkWidget *toolitem5;
GtkWidget *toolitem6;
GtkWidget *toolitem7;
GtkWidget *toolitem8;
GtkWidget *toolitem9;
GtkWidget *toolitem10;
GtkWidget *toolitem11;
GtkWidget *toolitem12;
GtkWidget *cbx_entry_fps;
GtkWidget *cbx_entry_fsize;
GtkWidget *spinbtn_fps_n;
GtkWidget *alignment1;
GtkWidget *alignment2;
GtkWidget *alignment3;
GtkWidget *lbl_bitrate;
GtkWidget *btn_change;
GtkWidget *prw_video;
GtkWidget *status_bar;
GtkWidget *entry_bitrate;
/* The vbox used in the status_bar. This widget will be return in the Init function.
   Infact, it contains a hseparator and a hbox.*/
GtkWidget * status_bar_vbox;
/* This hbox will contains all of the labels;*/
GtkWidget * status_bar_hbox;
/* The label for the status text*/
GtkWidget * statusbar_label_status;
GtkWidget * statusbar_label_video_type;
GtkWidget * statusbar_label_audio_type;
/* The label for Properties displaying*/
GtkWidget * statusbar_label_properties;

/*For Connection Dialog*/
GtkWidget *connection_dialog;
GtkWidget *dialog_vbox;
GtkWidget *tbl_con_info;
GtkWidget *lbl_url;
GtkWidget *entry_url;
GtkWidget *dialog_action_area;
GtkWidget *btn_connect_dialog;

static gboolean is_connect_button_clicked = FALSE;
/**
 * create the main window
 *
 * @param void
 *
 * @return nothing
 */
GtkWidget* gst_ipcam_client_create_main_window (void);

/**
 * create the connection dialog
 *
 * @param parent_window GtkWidget* parent window of this dialog
 *
 * @return nothing
 */
GtkWidget* gst_ipcam_client_create_connection_dialog (GtkWidget * parent_window);

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
void gst_ipcam_client_set_status_video_type(const gchar* type_name);

/**
 * show the audio type
 *
 * @param type_name const gchar* the audio type
 *
 * @return nothing
 */
void gst_ipcam_client_set_status_audio_type(const gchar* type_name);

/**
 * Create error dialog widget
 *
 * @param message gchar* the message to put into error dialog
 * @param parent GtkWidget* the parent window of this dialog
 *
 * @return GtkWidget* error dialog or NULL if something wrong
 */
GtkWidget * gst_ipcam_client_create_error_dialog(const gchar * message, GtkWindow * parent);