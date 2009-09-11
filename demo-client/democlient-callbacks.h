/**
 * \file:   democlient-callbacks.h
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 9-4-2009
 */
#include <gtk/gtk.h>

void
democlient_on_btn_Cancel_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
democlient_on_btn_Ok_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
democlient_on_menuBtn_quit_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
democlient_on_menuBtn_Options_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
democlient_on_menuBtn_about_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
democlient_on_btn_Connect1_clicked                (GtkButton       *button,
                                        gpointer        user_data);

void
democlient_on_btn_Disconnect1_clicked                (GtkButton       *button,
                                        gpointer        user_data);

void
democlient_on_btn_Pause1_clicked                (GtkButton       *button,
                                        gpointer        user_data);

void
democlient_on_btn_Resume1_clicked                (GtkButton       *button,
                                        gpointer        user_data);

void
democlient_on_btn_Connect2_clicked                (GtkButton       *button,
                                        gpointer        user_data);

void
democlient_on_btn_Disconnect2_clicked                (GtkButton       *button,
                                        gpointer        user_data);

void
democlient_on_btn_Pause2_clicked                (GtkButton       *button,
                                        gpointer        user_data);

void
democlient_on_btn_Resume2_clicked                (GtkButton       *button,
                                        gpointer        user_data);