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
democlient_backend_set_window (gpointer window_);


#ifdef	__cplusplus
}
#endif

#endif	/* _DEMOCLIENT_BACKEND_H */

