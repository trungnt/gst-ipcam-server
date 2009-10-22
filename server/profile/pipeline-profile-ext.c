#include <glib.h>

#include "pipeline-profile-ext.h"

gboolean gst_rtsp_pipeline_profile_set_int_var(GstRTSPPipelineProfile * profile, const gchar * var, const gint value) {
	gchar * str_value = g_strdup_printf("%d", value);
	gboolean ret = gst_rtsp_pipeline_profile_set_var(profile, var, str_value);
	g_free(str_value);
	return ret;
}

gboolean gst_rtsp_pipeline_profile_video_set_width_int(GstRTSPPipelineProfile * profile, const gint width) {
	g_return_val_if_fail(gst_rtsp_pipeline_profile_is_video(profile), FALSE);
	return gst_rtsp_pipeline_profile_set_int_var(profile, "video-width", width);
}

gboolean gst_rtsp_pipeline_profile_video_set_height_int(GstRTSPPipelineProfile * profile, const gint height) {
	g_return_val_if_fail(gst_rtsp_pipeline_profile_is_video(profile), FALSE);
	return gst_rtsp_pipeline_profile_set_int_var(profile, "video-height", height);
}

gboolean gst_rtsp_pipeline_profile_video_set_framerate(GstRTSPPipelineProfile * profile, const gchar * framerate) {
	g_return_val_if_fail(gst_rtsp_pipeline_profile_is_video(profile), FALSE);
	return gst_rtsp_pipeline_profile_set_var(profile, "video-framerate", framerate);
}

gboolean gst_rtsp_pipeline_profile_video_set_bitrate_int(GstRTSPPipelineProfile* profile, gint bitrate) {
	g_return_val_if_fail(gst_rtsp_pipeline_profile_is_video(profile), FALSE);
	return gst_rtsp_pipeline_profile_set_int_var(profile, "video-bitrate", bitrate);
}

gboolean gst_rtsp_pipeline_profile_video_set_width(GstRTSPPipelineProfile* profile, const gchar* width) {
	g_return_val_if_fail(gst_rtsp_pipeline_profile_is_video(profile), FALSE);
	return gst_rtsp_pipeline_profile_set_var(profile, "video-width", width);
}

gboolean gst_rtsp_pipeline_profile_video_set_height(GstRTSPPipelineProfile* profile, const gchar * height) {
	g_return_val_if_fail(gst_rtsp_pipeline_profile_is_video(profile), FALSE);
	return gst_rtsp_pipeline_profile_set_var(profile, "video-height", height);
}

gboolean gst_rtsp_pipeline_profile_video_set_bitrate(GstRTSPPipelineProfile* profile, const gchar * bitrate) {
	g_return_val_if_fail(gst_rtsp_pipeline_profile_is_video(profile), FALSE);
	return gst_rtsp_pipeline_profile_set_var(profile, "video-bitrate", bitrate);
}
