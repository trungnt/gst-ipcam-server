#include <glib.h>

#include "server-profile.h"
#include "profile/pipeline-profile.h"
#include "profile/server-configuration.h"

gboolean gst_rtsp_pipeline_profile_set_int_var(GstRTSPPipelineProfile * profile, const gchar * var, const gint value) {
	gchar * str_value = g_strdup_printf("%d", value);
	gboolean ret = gst_rtsp_pipeline_profile_set_var(profile, var, str_value);
	g_free(str_value);
	return ret;
}

gboolean gst_rtsp_pipeline_profile_set_video_width(GstRTSPPipelineProfile * profile, const gint width) {
	return gst_rtsp_pipeline_profile_set_int_var(profile, "video-width", width);
}

gboolean gst_rtsp_pipeline_profile_set_video_height(GstRTSPPipelineProfile * profile, const gint height) {
	return gst_rtsp_pipeline_profile_set_int_var(profile, "video-height", height);
}

gboolean gst_rtsp_pipeline_profile_set_video_framerate(GstRTSPPipelineProfile * profile, const gchar * framerate) {
	return gst_rtsp_pipeline_profile_set_var(profile, "video-framerate", framerate);
}
