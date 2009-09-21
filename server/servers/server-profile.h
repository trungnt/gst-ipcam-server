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

	/**
	 * Set integer value for a variable in the profile
	 * Return TRUE if everythin is ok, FALSE otherwise (like there's no variable with given name)
	 *
	 * @param profile GstRTSPPipelineProfile* profile to set
	 * @param var gchar* name of the variable to set
	 * @param value gint value to set
	 *
	 * @return gboolean
	 */
	gboolean gst_rtsp_pipeline_profile_set_int_var(GstRTSPPipelineProfile * profile, const gchar * var, const gint value);

	/**
	 * Set the width of video size. All servers have this variable.
	 *
	 * @param profile GstRTSPPipelineProfile* profile to set
	 * @param width gint value to set
	 *
	 * @return gboolean TRUE if everything is ok, FALSE otherwise
	 */
	gboolean gst_rtsp_pipeline_profile_set_video_width(GstRTSPPipelineProfile * profile, const gint width);

	/**
	 * Set the height of video size. All servers have this variable.
	 *
	 * @param profile GstRTSPPipelineProfile* profile to set
	 * @param height gint value to set
	 *
	 * @return gboolean TRUE if everything is ok, FALSE otherwise
	 */
	gboolean gst_rtsp_pipeline_profile_set_video_height(GstRTSPPipelineProfile * profile, const gint height);

	/**
	 * Set the framerate for the stream. All servers have this variable.
	 *
	 * @param profile GstRTSPPipelineProfile* profile to set
	 * @param framerate gchar* value to set
	 *
	 * @return gboolean TRUE if everything is ok, FALSE otherwise
	 */
	gboolean gst_rtsp_pipeline_profile_set_video_framerate(GstRTSPPipelineProfile * profile, const gchar * framerate);


#ifdef	__cplusplus
}
#endif

#endif	/* _SERVER_PROFILE_H */

