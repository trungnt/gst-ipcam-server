/*
 * \file:   gst-ipcam-client-backend.h
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 8-26-2009
 */

#ifndef _gst_ipcam_client_BACKEND_H
#define	_gst_ipcam_client_BACKEND_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * init for using gstreamer
 * This function is used whenever the main function is created
 *
 * @param argc int *
 * @param argv[] char **
 *
 * @return nothing
 */
void
gst_ipcam_client_backend_init (int *argc,
              char **argv[]);

/**
 * create the pipeline to get the data from the rtsp server
 *
 * @param pipeline_description const gchar * the pipeline description
 *
 * @return nothing
 */
void
gst_ipcam_client_backend_create_pipeline(const gchar *pipeline_description);

/**
 * Set the pointer window
 *
 * @param window_ gpointer the pointer to show video region in the main window
 *
 * @return nothing
 */
void
gst_ipcam_client_backend_set_window (gpointer window_);

/**
 * set the pipeline from the state GST_STATE_NULL to the state GST_STATE_PLAYING
 *
 * @param nothing
 *
 * @return Result of the state change
 */
gint
gst_ipcam_client_backend_play();

/**
 * set the pipeline from the state GST_STATE_PLAYING to the state GST_STATE_PAUSED
 *
 * @param nothing
 *
 * @return Result of the state change
 */
gint
gst_ipcam_client_backend_pause();

/**
 * set the pipeline from the state GST_STATE_PLAYING to the state GST_STATE_NULL
 *
 * @param nothing
 *
 * @return Result of the state change
 */
gint
gst_ipcam_client_backend_stop();

/**
 * set the pipeline from the state GST_STATE_PAUSED to the state GST_STATE_PLAYING
 *
 * @param nothing
 *
 * @return Result of the state change
 */
gint
gst_ipcam_client_backend_resume();
/**
 * free memories for pipeline and videosink pointer before exit the application
 *
 * @param nothing
 *
 * @return nothing
 */
void
gst_ipcam_client_backend_deinit();

#ifdef	__cplusplus
}
#endif

#endif	/* _gst_ipcam_client_BACKEND_H */

