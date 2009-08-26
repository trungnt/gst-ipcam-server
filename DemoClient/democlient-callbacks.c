/*
 * File:   democlient-callbacks.c
 * Author: damquang.tuan@nomovok.com
 *
 * Created on August 26, 2009, 10:04 AM
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "democlient-callbacks.h"
#include "democlient-interface.h"
#include "democlient-support.h"


void
democlient_on_btn_Connect_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Connect);
    btn_Connect = g_object_ref(btn_Connect);
    
    gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
    gtk_widget_show (btn_Disconnect);
    
    GtkWidget *connectionDialog;
    connectionDialog = democlient_create_connectionDialog();
    gtk_widget_show(connectionDialog);

}

void
democlient_on_btn_Disconnect_clicked              (GtkButton       *button,
                                           gpointer         user_data)
{
    gtk_container_remove (GTK_CONTAINER (toolitem_Connect), btn_Disconnect);
    btn_Disconnect = g_object_ref(btn_Disconnect);
    
    gtk_container_add (GTK_CONTAINER (toolitem_Connect), btn_Connect);
    gtk_widget_show (btn_Connect);
}

void
democlient_on_btn_Pause_clicked                   (GtkButton       *button,
                                            gpointer         user_data)
{
    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Pause);
    btn_Pause = g_object_ref(btn_Pause);

    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Resume);
}

void
democlient_on_btn_Resume_clicked                  (GtkButton       *button,
                                            gpointer         user_data)
{
    gtk_container_remove (GTK_CONTAINER (toolitem_Pause), btn_Resume);
    btn_Resume = g_object_ref(btn_Resume);

    gtk_container_add (GTK_CONTAINER (toolitem_Pause), btn_Pause);
    gtk_widget_show (btn_Pause);
}

void
democlient_on_btn_Options_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *optionDialog;
    optionDialog = democlient_create_optionDialog();
    gtk_widget_show(optionDialog);
}


void
democlient_on_btn_About_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
democlient_on_btn_Quit_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
democlient_on_btn_ConnectDialog_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
democlient_on_btn_Cancel_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
democlient_on_btn_Ok_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{

}

