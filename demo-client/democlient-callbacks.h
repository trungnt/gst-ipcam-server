/*
 * \file:   democlient-callbacks.h
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 8-26-2009
 */
#include <gtk/gtk.h>


//store the url in the current session
static gchar *URL;
//store the gstream type in the current session
static gint TYPE = 4;
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
                                        gpointer         user_data);

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
                                        gpointer         user_data);

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
                                        gpointer         user_data);

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
                                        gpointer         user_data);

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
                                        gpointer         user_data);*/

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
                                        gpointer         user_data);

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
                                        gpointer         user_data);

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
                                        gpointer         user_data);

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
                                        gpointer         user_data);

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
                                        gpointer         user_data);

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
                                        gpointer         user_data);
