/* 
 * File:   democlient_backend.h
 * Author: dqtuan
 *
 * Created on August 26, 2009, 11:02 AM
 */

#ifndef _DEMOCLIENT_BACKEND_H
#define	_DEMOCLIENT_BACKEND_H

#ifdef	__cplusplus
extern "C" {
#endif

void
democlient_backend_init (int *argc,
              char **argv[]);

void
democlient_backend_create_pipeline(const gchar *pipeline_description);

void
democlient_backend_set_window (gpointer window_);

void
democlient_backend_play();

void
democlient_backend_pause();

void
democlient_backend_stop();

void
democlient_backend_resume();

void
democlient_backend_deinit();

#ifdef	__cplusplus
}
#endif

#endif	/* _DEMOCLIENT_BACKEND_H */

