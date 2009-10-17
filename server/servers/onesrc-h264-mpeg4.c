/* GStreamer
 * Copyright (C) 2008 Wim Taymans <wim.taymans at gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#include "profile/pipeline-profile-ext.h"
#include "profile/server-configuration.h"

/* default profile file for this server */
#define DEFAULT_PROFILE_FILE_VIDEO_H264 "onesrc-h264.ini"
#define DEFAULT_PROFILE_FILE_VIDEO_MPEG4 "onesrc-mpeg4.ini"

static gboolean
timeout(GstRTSPServer *server, gboolean ignored) {
	GstRTSPSessionPool *pool;

	pool = gst_rtsp_server_get_session_pool(server);
	gst_rtsp_session_pool_cleanup(pool);
	g_object_unref(pool);

	return TRUE;
}

int
main(int argc, char *argv[]) {
	GMainLoop *loop;
	GstRTSPServer *server;
	GstRTSPMediaMapping *mapping;
	GstRTSPMediaFactory *factory_h264, *factory_mpeg4;
	GstRTSPServerConfiguration * server_config = NULL;
	gchar * audio_profile_name = NULL;

	gst_init(&argc, &argv);

	loop = g_main_loop_new(NULL, FALSE);

	/* create a server instance */
	server = gst_rtsp_server_new();
	/* get the mapping for this server, every server has a default mapper object
	 * that be used to map uri mount points to media factories */

	mapping = gst_rtsp_server_get_media_mapping(server);

	/* make a media factory for a h264 video stream and audio (aac, g711 or g726) stream. The default media factory can use
	 * gst-launch syntax to create pipelines.
	 * any launch line works as long as it contains elements named pay%d. Each
	 * element with pay%d names will be a stream */
	factory_h264 = gst_rtsp_media_factory_new();
	/* set webcam source and port to listen for factory */
	gst_rtsp_factory_set_device_source(factory_h264, "v4l2src", "/dev/video0", 3000);

	/* start building the pipeline */
	server_config = gst_rtsp_server_configuration_load(DEFAULT_PROFILE_FILE_VIDEO_H264);

	/* we can set some common server parameter by using functions in server-profile.h
	 * but default values will be used here
	 */
	
	if (argc > 1 && server_config != NULL) {
		if (g_strrstr(argv[1], "aac")) {
			audio_profile_name = "audio AAC";
		} else if (g_strrstr(argv[1], "g726")) {
			audio_profile_name = "audio G726";
		} else if (g_strrstr(argv[1], "g711")) {
			audio_profile_name = "audio G711";
		}
		if (audio_profile_name != NULL) {
			gst_rtsp_server_configuration_set_default_audio_pipeline(server_config, audio_profile_name);
		}
	}

	gst_rtsp_media_factory_set_server_configuration(factory_h264, server_config);

	/* share the pipeline with multiple clients */
	gst_rtsp_media_factory_set_shared(factory_h264, TRUE);

	/* attach the test factory to the /h264 url */
	gst_rtsp_media_mapping_add_factory(mapping, "/h264", factory_h264);
	
	/* make a media factory for a mpeg 4 video stream and audio (aac, g711 or g726) stream. The default media factory can use
	 * gst-launch syntax to create pipelines.
	 * any launch line works as long as it contains elements named pay%d. Each
	 * element with pay%d names will be a stream */
	factory_mpeg4 = gst_rtsp_media_factory_new();

	/* set webcam source and port to listen for factory */
	/* gst_rtsp_factory_set_device_source (factory_jpg, "v4l2src", "/dev/video0", 3000); */
   factory_mpeg4->v4l2src_pipeline = factory_h264->v4l2src_pipeline;  
   factory_mpeg4->v4l2src_port = factory_h264->v4l2src_port + 1;
   factory_mpeg4->multiudpsink = factory_h264->multiudpsink;

	/* start building the pipeline */
	server_config = gst_rtsp_server_configuration_load(DEFAULT_PROFILE_FILE_VIDEO_MPEG4);
	
	if (audio_profile_name != NULL) {
		gst_rtsp_server_configuration_set_default_audio_pipeline(server_config, audio_profile_name);
	}

	gst_rtsp_media_factory_set_server_configuration(factory_mpeg4, server_config);

	/* share the pipeline with multiple clients */
	gst_rtsp_media_factory_set_shared(factory_mpeg4, TRUE);

	/* attach the test factory to the /mp4 url */
	gst_rtsp_media_mapping_add_factory(mapping, "/mp4", factory_mpeg4);

	/* don't need the ref to the mapper anymore */
	g_object_unref(mapping);

	/* attach the server to the default maincontext */
	gst_rtsp_server_attach(server, NULL);

	g_timeout_add_seconds(2, (GSourceFunc) timeout, server);

	/* start serving */
	g_main_loop_run(loop);

	return 0;
}
