/**
 * \file:   democlient-callbacks.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 9-4-2009
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "democlient-callbacks.h"
#include "democlient-interface.h"
#include "democlient-support.h"

/**
 * Handle the event when clicking on the Cancel button in the options dialog.
 * The application will deny all options that a user make in this dialog
 * 
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Cancel_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{

}

/**
 * Handle the event when clicking on the Ok button in the options dialog.
 * The application will save all options that a user make in this dialog
 *
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Ok_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{

}

/**
 * Handle the event when clicking on the Quit item in the menu in the
 * mainwindow. The application will be exited.
 *
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_menuBtn_quit_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gtk_main_quit();
}

/**
 * Handle the event when clicking on the Configurations item in the menu in the
 * mainwindow. The option dialog will appear
 *
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_menuBtn_Options_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkWidget *optionDialog;
    optionDialog = democlient_create_optionDialog();
    gtk_widget_show(optionDialog);
}

/**
 * Handle the event when clicking on the About item in the menu in the
 * mainwindow. The About dialog will appear
 *
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_menuBtn_about_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

/**
 * Handle the event when clicking on the Connect1 button in the main window.
 * The button will change to the Disconnect1 button and the video got from server
 * will appear in the left maindow
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Connect1_clicked                (GtkButton       *button,
                                        gpointer        user_data)
{
    gtk_widget_hide (btn_Connect1);
    gtk_widget_show (btn_Disconnect1);
}

/**
 * Handle the event when clicking on the Disconnect1 button in the main window.
 * The button will change to the Connect1 button and the video got from server
 * in the left maindow will be stoped
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Disconnect1_clicked                (GtkButton       *button,
                                        gpointer        user_data)
{
    gtk_widget_hide (btn_Disconnect1);
    gtk_widget_show (btn_Connect1);
}

/**
 * Handle the event when clicking on the Pause1 button in the main window.
 * The button will change to the Resume1 button and the video got from server
 * in the left maindow will be paused
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Pause1_clicked                (GtkButton       *button,
                                        gpointer        user_data)
{
    gtk_widget_hide (btn_Pause1);
    gtk_widget_show (btn_Resume1);
}

/**
 * Handle the event when clicking on the Resume1 button in the main window.
 * The button will change to the the Pause1 button and the video got from server
 * in the left maindow will be resumed
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Resume1_clicked                (GtkButton       *button,
                                        gpointer        user_data)
{
    gtk_widget_hide (btn_Resume1);
    gtk_widget_show (btn_Pause1);
}

/**
 * Handle the event when clicking on the Connect2 button in the main window.
 * The button will change to the Disconnect1 button and the video got from server
 * will appear in the right maindow
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Connect2_clicked                (GtkButton       *button,
                                        gpointer        user_data)
{
    gtk_widget_hide (btn_Connect2);
    gtk_widget_show (btn_Disconnect2);
}

/**
 * Handle the event when clicking on the Disconnect2 button in the main window.
 * The button will change to the Connect2 button and the video got from server
 * in the left maindow will be stoped
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Disconnect2_clicked                (GtkButton       *button,
                                        gpointer        user_data)
{
    gtk_widget_hide (btn_Disconnect2);
    gtk_widget_show (btn_Connect2);
}

/**
 * Handle the event when clicking on the Pause1 button in the main window.
 * The button will change to the Resume1 button and the video got from server
 * in the left maindow will be paused
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Pause2_clicked                (GtkButton       *button,
                                        gpointer        user_data)
{
    gtk_widget_hide (btn_Pause2);
    gtk_widget_show (btn_Resume2);
}

/**
 * Handle the event when clicking on the Resume1 button in the main window.
 * The button will change to the Pause1 button and the video got from server
 * in the left maindow will be resumed
 *
 * @param button GtkButton *
 * @param user_data gpointer
 *
 * @return nothing
 */
void
democlient_on_btn_Resume2_clicked                (GtkButton       *button,
                                        gpointer        user_data)
{
    gtk_widget_hide (btn_Resume2);
    gtk_widget_show (btn_Pause2);
}