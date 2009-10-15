/* 
 * File:   server-profile.h
 * Author: trungnt
 *
 * Created on September 16, 2009, 11:17 AM
 */

#ifndef _PIPELINE_PROFILE_EXT_H
#define	_PIPELINE_PROFILE_EXT_H

#include "pipeline-profile.h"
#include "server-configuration.h"

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
	 * If the pipeline is not a video pipeline, then return FALSE.
	 *
	 * @param profile GstRTSPPipelineProfile* profile to set
	 * @param width gint value to set
	 *
	 * @return gboolean TRUE if everything is ok, FALSE otherwise
	 */
	gboolean gst_rtsp_pipeline_profile_video_set_width_int(GstRTSPPipelineProfile * profile, gint width);

	/**
	 * Set the height of video size. All servers have this variable.
	 * If the pipeline is not a video pipeline, then return FALSE.
	 *
	 * @param profile GstRTSPPipelineProfile* profile to set
	 * @param height gint value to set
	 *
	 * @return gboolean TRUE if everything is ok, FALSE otherwise
	 */
	gboolean gst_rtsp_pipeline_profile_video_set_height_int(GstRTSPPipelineProfile * profile, gint height);

	/**
	 * Set the framerate for the stream. All servers have this variable.
	 * If the pipeline is not a video pipeline, then return FALSE.
	 *
	 * @param profile GstRTSPPipelineProfile* profile to set
	 * @param framerate gchar* value to set
	 *
	 * @return gboolean TRUE if everything is ok, FALSE otherwise
	 */
	gboolean gst_rtsp_pipeline_profile_video_set_framerate(GstRTSPPipelineProfile * profile, const gchar * framerate);

	/**
	 * Set the bitrate to the video encoder. Not all pipeline has this variable.
	 * If current pipeline is not a video pipeline and does not has "video-bitrate" variable: return FALSE
	 *
	 * @param profile GstRTSPPipelineProfile* profile to set
	 * @param bitrate gint value to set
	 *
	 * @return gboolean TRUE if everything is ok, FALSE otherwise
	 */
	gboolean gst_rtsp_pipeline_profile_video_set_bitrate_int(GstRTSPPipelineProfile * profile, gint bitrate);

	gboolean gst_rtsp_pipeline_profile_video_set_width(GstRTSPPipelineProfile * profile, const gchar * width);
	gboolean gst_rtsp_pipeline_profile_video_set_height(GstRTSPPipelineProfile * profile, const gchar * height);
	gboolean gst_rtsp_pipeline_profile_video_set_bitrate(GstRTSPPipelineProfile * profile, const gchar * bitrate);
#ifdef	__cplusplus
}
#endif

#endif	/* _PIPELINE_PROFILE_EXT_H */

