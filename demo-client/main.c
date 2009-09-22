/**
 * \file:   main.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 8-26-2009
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "demo-client-interface.h"
#include "demo-client-support.h"
#include "demo-client-backend.h"

int
main (int argc, char *argv[])
{
  GtkWidget *mainWindow;
  //GMainLoop *loop;
#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);
  demo_client_backend_init (&argc, &argv);
  //add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  mainWindow = demo_client_create_mainWindow ();
  gtk_widget_show (mainWindow);
  //loop = g_main_loop_new(NULL, FALSE);
  //g_main_loop_run(loop);
  
  gtk_main ();
  return 0;
}

