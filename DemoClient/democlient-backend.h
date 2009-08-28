/* 
 * File:   democlient_backend.h
 * Author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * Created on August 26, 2009, 11:02 AM
 */

#ifndef _DEMOCLIENT_BACKEND_H
#define	_DEMOCLIENT_BACKEND_H

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
democlient_backend_init (int *argc,
              char **argv[]);

/**
 * create the pipeline to get the data from the rtsp server
 *
 * @param pipeline_description const gchar * the pipeline description
 *
 * @return nothing
 */
void
democlient_backend_create_pipeline(const gchar *pipeline_description);

/**
 * Set the pointer window
 *
 * @param window_ gpointer the pointer to show video region in the main window
 *
 * @return nothing
 */
void
democlient_backend_set_window (gpointer window_);

/**
 * set the pipeline from the state GST_STATE_NULL to the state GST_STATE_PLAYING
 *
 * @param nothing
 *
 * @return nothing
 */
void
democlient_backend_play();

/**
 * set the pipeline from the state GST_STATE_PLAYING to the state GST_STATE_PAUSED
 *
 * @param nothing
 *
 * @return nothing
 */
void
democlient_backend_pause();

/**
 * set the pipeline from the state GST_STATE_PLAYING to the state GST_STATE_NULL
 *
 * @param nothing
 *
 * @return nothing
 */
void
democlient_backend_stop();

/**
 * set the pipeline from the state GST_STATE_PAUSED to the state GST_STATE_PLAYING
 *
 * @param nothing
 *
 * @return nothing
 */
void
democlient_backend_resume();

/**
 * free memories for pipeline and videosink pointer before exit the application
 *
 * @param nothing
 *
 * @return nothing
 */
void
democlient_backend_deinit();

#ifdef	__cplusplus
}
#endif

#endif	/* _DEMOCLIENT_BACKEND_H */

