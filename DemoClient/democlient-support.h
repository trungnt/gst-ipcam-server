/*
 * \file:   democlient-support.h
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 8-26-2009
 */
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  define Q_(String) g_strip_context ((String), gettext (String))
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define Q_(String) g_strip_context ((String), (String))
#  define N_(String) (String)
#endif


/**
 * Public Functions.
 */

/**
 *  Use this function to set the directory containing installed pixmaps.
 *  @param widget GtkWidget *
 *  @param widget_name GtkWidget *
 *
 *  @return widget GtkWidget *
 */
GtkWidget*  lookup_widget              (GtkWidget       *widget,
                                        const gchar     *widget_name);


/**
 *  Use this function to set the directory containing installed pixmaps.
 *  @param directory const gchar *
 *
 *  @return nothing
 */
void        democlient_add_pixmap_directory       (const gchar     *directory);


/*
 * Private Functions.
 */

/**
 *  This is an internally used function to find pixmap files.
 *
 *  @param filename const gchar *
 *  @return pixmap file gchar *
 */
static gchar*
find_pixmap_file                       (const gchar     *filename);

/** This is an internally used function to create pixmaps.
 *
 *  @param widget GtkWidget *
 *  @param filename const gchar *
 *
 *  @return pixmap GtkWidget *
 *
 */
GtkWidget*  create_pixmap              (GtkWidget       *widget,
                                        const gchar     *filename);

/**
 *  This is an internally used function to create pixmaps.
 *
 *  @param filename const gchar *
 *
 *  @return pixbuf GdkPixbuf *
 */
GdkPixbuf*  create_pixbuf              (const gchar     *filename);

/**
 *  This is used to set ATK action descriptions.
 *
 *  @param action AtkAction *
 *  @param action_name const gchar *
 *  @param description const gchar *
 *
 *  @return nothing
 */
void        democlient_glade_set_atk_action_description (AtkAction       *action,
                                              const gchar     *action_name,
                                              const gchar     *description);

