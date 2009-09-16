/* 
 * File:   server-profile.h
 * Author: trungnt
 *
 * Created on September 16, 2009, 11:17 AM
 */

#ifndef _SERVER_PROFILE_H
#define	_SERVER_PROFILE_H

#include "profile/pipeline-profile.h"

#ifdef	__cplusplus
extern "C" {
#endif

    gboolean gst_rtsp_pipeline_profile_set_int_var(GstRTSPPipelineProfile * profile, const gchar * var, const gint value);
    gboolean gst_rtsp_pipeline_profile_set_video_width(GstRTSPPipelineProfile * profile, const gint width);
    gboolean gst_rtsp_pipeline_profile_set_video_height(GstRTSPPipelineProfile * profile, const gint height);
    gboolean gst_rtsp_pipeline_profile_set_video_framerate(GstRTSPPipelineProfile * profile, const gchar * framerate);


#ifdef	__cplusplus
}
#endif

#endif	/* _SERVER_PROFILE_H */

