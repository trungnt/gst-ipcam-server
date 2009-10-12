/**
 * File:   gst-ipcam-client-windowid.h
 * @author: Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com>
 *
 * Created on September 24, 2009, 10:33 AM
 */

#include <getopt.h>
#include <unistd.h>

#include "gst-ipcam-client-windowid.h"

static gint my_client_id = 0;

/**
 * This structure define which long options will be support to our client from the command line
 * For now, we only use --id
 */
static struct option my_client_long_options[] = {
	{"id", required_argument, 0, 'i' },
	{0, 0, 0, 0}
};

gchar * gst_ipcam_client_window_create_title(const gchar* window_name) {
	gchar * title = g_strdup_printf("%s - %d", window_name, my_client_id);
	g_assert(title);
	return title;
}

void gst_ipcam_client_set_id(gint id) {
	my_client_id = id;
}

void gst_ipcam_client_process_options(gint argc, char** argv) {
	gint c = -1;
	gint option_index;
	gboolean is_id_set = FALSE;

	while (1) {
		c = getopt_long(argc, argv, "", my_client_long_options, &option_index);

		/* if the process is end*/
		if (c == -1) {
			break;
		}

		switch (c) {
                        /* got id option*/
			case 'i': 
			{
				gint id;
				id = g_ascii_strtod(optarg, NULL);
				if (id > 0) {
					gst_ipcam_client_set_id(id);
					is_id_set = TRUE;
				}
			}
				break;
			default:
				break;
		}
	}

	/* so no id was requested by the user, we'll use process id*/
	if (is_id_set == FALSE) {
		pid_t my_pid = getpid();
		gst_ipcam_client_set_id(my_pid);
	}
}
