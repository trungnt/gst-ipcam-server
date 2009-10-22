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

#include "gst-ipcam-client-interface.h"
#include "gst-ipcam-client-support.h"
#include "gst-ipcam-client-backend.h"
#include "gst-ipcam-client-windowid.h"

int
main (int argc, char *argv[])
{
    GtkWidget *main_window;

#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif

    gtk_set_locale();
    gtk_init(&argc, &argv);
    gst_ipcam_client_backend_init(&argc, &argv);
    gst_ipcam_client_process_options(argc, argv);

    /*
     * The following code was added by Glade to create one of each component
     * (except popup menus), just so that you see something after building
     * the project. Delete any components that you don't want shown initially.
     */
    main_window = gst_ipcam_client_create_main_window();
    gtk_widget_show(main_window);

    gtk_main();
    return 0;
}

