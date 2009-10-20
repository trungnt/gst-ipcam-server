/**
 * File:   gst-ipcam-client-windowid.h
 * @author: Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com>
 *
 * Created on September 24, 2009, 10:33 AM
 */

#ifndef _GST_IPCAM_CLIENT_WINDOWID_H
#define	_GST_IPCAM_CLIENT_WINDOWID_H

#include <glib.h>

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * Create the window title, this just a text with origin title plus the id.
     * Return string can be used to set title to the window
     *
     * @param window_name gchar* window title we want to set
     * @return gchar* window name + application id
     */
    gchar * gst_ipcam_client_window_create_title(const gchar * window_name);

    /**
     * Set the id to our client.
     * The id will be append to windows' title.
     * This action won't affect opened windows
     *
     * @param id gint id we want to set
     * @return None
     */
    void gst_ipcam_client_set_id(gint id);

    /**
     * Process client command line options to check if the user want to set id for our client
     *
     * @param argc gint number of agruments
     * @param argv char** array of agruments
     *
     * @return None
     */
    void gst_ipcam_client_process_options(gint argc, char** argv);

#ifdef	__cplusplus
}
#endif

#endif	/* _GST_IPCAM_CLIENT_WINDOWID_H */

