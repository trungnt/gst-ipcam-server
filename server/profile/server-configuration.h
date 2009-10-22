/**
 * Gst-ipcam-server
 * server configuration
 * Copyright (C) 2009 Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
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
 *
 * Created on September 18, 2009, 5:30 PM
 */

#ifndef _SERVER_CONFIGURATION_H
#define	_SERVER_CONFIGURATION_H

#include "pipeline-profile.h"


#ifdef	__cplusplus
extern "C" {
#endif

	/**
	 * Server configuration structure.
	 * Server configuration contain one or more pipeline(s).
	 * Each pipeline will be stored in a GstRTSPPipelineProfile structure.
	 * The configuration is stored in a text file.
	 * The format of this file is as follow:
	 * ....
	 * # line start with # symbol is the comment and will be skipped
	 * # a configuration can stores more than one pipelines
	 * # each pipeline start with a line [pipeline]
	 * # pipeline must have basic information or NULL result will be return in the _load() function
	 * # pipeline-name: the name of pipeline
	 * # pipeline-type: the type of pipeline (can be video and audio for now)
	 * # pipeline-codec: the pipeline codec
	 * # pipeline-desc: the description of pipeline. For the format of description, see GstRTSPPipelineProfile
	 * # pipeline can have variables or not
	 * #
	 *
	 * [pipeline]
	 * pipeline-name=name
	 * pipeline-type=type
	 * pipeline-codec=codec
	 * pipeline-desc=description
	 *
	 * # the variable section (if have must be placed right after basic information section
	 * # the variable section must start with the line [<pipelinename>-variables] which pipelinename must be the name of current pipeline
	 * # each variable must be stored in the format name=variable
	 * # invalid variable(s) will be skipped
	 * #
	 * [name-variables]
	 * name=value
	 *
	 * To load the configuration use function gst_rtsp_server_configuration_load().
	 * Use gst_rtsp_server_configuration_free() to free the structure.
	 * The pipeline can be get by index or name using gst_rtsp_server_configuration_get_pipeline*()
	 */
	typedef struct GstRTSPServerConfiguration {
		GList * pipelines_name; /**< list of pipelines name */
		GHashTable * pipelines; /**< a hash table stored pipelines using pipeline name as key */
		GHashTable * pipelines_by_index; /**< same as above but using index as key */
		gchar * default_video_pipeline; /**< name of default video profile */
		gchar * default_audio_pipeline; /**< name of default audio profile */
	} GstRTSPServerConfiguration;

	/**
	 * Load the server configuration stored in configuration file
	 *
	 * @param config_file gchar* name of the configuration file
	 *
	 * @return GstRTSPServerConfiguration a structure to store configuration or NULL if something wrong
	 */
	GstRTSPServerConfiguration * gst_rtsp_server_configuration_load(const gchar * config_file);

	/**
	 * Free the server configuration structure load by above function
	 *
	 * @param config GstRTSPServerConfiguration* the server configuration structure to free
	 *
	 * @return None
	 */
	void gst_rtsp_server_configuration_free(GstRTSPServerConfiguration * config);

	/**
	 * Get the number of pipelines stored in the server configuration.
	 *
	 * @param config GstRTSPServerConfiguration* the server configuration structure to get
	 *
	 * @return the number of pipeline(s)
	 */
	gint gst_rtsp_server_configuration_get_number_of_pipelines(const GstRTSPServerConfiguration * config);

	/**
	 * Get the pipeline with given index. The index is start with 0
	 *
	 * @param config GstRTSPServerConfiguration* the server configuration to get
	 * @param index gint the index of the pipeline to get (must be less than number of pipeline)
	 *
	 * @return GstRTSPPipelineConfigration* the pipeline we want to get or NULL if the configuration does not contain the pipeline with given index
	 */
	GstRTSPPipelineProfile * gst_rtsp_server_configuration_get_pipeline_at(const GstRTSPServerConfiguration * config, gint index);

	/**
	 * Get the list of the pipeline names in the configuration.
	 *
	 * @param config GstRTSPServerConfiguration* the server configuration to get
	 *
	 * @return GList* a GList store the list of pipeline name
	 */
	GList * gst_rtsp_server_configuration_get_pipelines_name_list(const GstRTSPServerConfiguration * config);

	/**
	 * Get the pipeline in server configuration by pipeline name
	 *
	 * @param config GstRTSPServerConfiguration* the server configuration to get
	 * @param name gchar* the name of pipeline to get
	 *
	 * @return GstRTSPPipelineProfile* the pipeline we want to get or NULL if some thing wrong
	 */
	GstRTSPPipelineProfile * gst_rtsp_server_configuration_get_pipeline(const GstRTSPServerConfiguration * config, const gchar * name);

	/**
	 * set the default video pipeline for server configuration.
	 * If the pipeline is not existed or it's not a video pipeline, return FALSE and do nothing, else return TRUE
	 *
	 * @param config GstRTSPServerConfiguration* the server configuration to set
	 * @param pipeline_name gchar* name of pipeline to set to default video pipeline
	 * @return gboolean
	 */
	gboolean gst_rtsp_server_configuration_set_default_video_pipeline(GstRTSPServerConfiguration * config, const gchar * pipeline_name);

	/**
	 * get the default video profile name of server configuration
	 *
	 * @param config GstRTSPServerConfiguration* the server configuration to get
	 *
	 * @return gchar* name of default video pipeline or NULL if no one is set
	 */
	const gchar * gst_rtsp_server_configuration_get_default_video_pipeline_name(const GstRTSPServerConfiguration * config);

	/**
	 * get default video profile name of server configuration
	 *
	 * @param config GstRTSPServerConfiguration* the server configuration to get
	 *
	 * @return GstRTSPPipelineProfile* default video profile or NULL if no one is set
	 */
	GstRTSPPipelineProfile * gst_rtsp_server_configuration_get_default_video_pipeline(const GstRTSPServerConfiguration * config);

	/**
	 * set default audio profile for server configuration.
	 * If the profile is not existed or it's not an audio pipeline, nothing will be set and return FALSE.
	 * Return TRUE if everything is ok
	 *
	 * @param config GstRTSPServerConfiguration* server configuration to set
	 * @param pipeline_name gchar* name of audio profile to set to
	 *
	 * @return gboolean
	 */
	gboolean gst_rtsp_server_configuration_set_default_audio_pipeline(GstRTSPServerConfiguration * config, const gchar * pipeline_name);

	/**
	 * get default audio profile name of server configuration
	 *
	 * @param config GstRTSPServerConfiguration* server configuration to get
	 *
	 * @return gchar* name of default audio profile
	 */
	const gchar * gst_rtsp_server_configuration_get_default_audio_pipeline_name(const GstRTSPServerConfiguration * config);

	/**
	 * get default audio profile of server configuration.
	 * This's the convenion function for above
	 *
	 * @param config GstRTSPServerConfiguration* server configuration to get
	 *
	 * @return GstRTSPPipelineProfile* default audio profile or NULL if no one is set
	 */
	GstRTSPPipelineProfile * gst_rtsp_server_configuration_get_default_audio_pipeline(const GstRTSPServerConfiguration * config);

	/**
	 * Build the combined pipeline. The output will be the combine of default video and audio pipeline.
	 * If there's no default video pipeline, return NULL
	 * If there's no default audio pipeline, result will be the default video pipeline
	 *
	 * @param config GstRTSPServerConfiguration* server configuration to get
	 *
	 * @return gchar* pipeline to launch
	 */
	gchar * gst_rtsp_server_configuration_build_pipeline(GstRTSPServerConfiguration * config);

#ifdef	__cplusplus
}
#endif

#endif	/* _SERVER_CONFIGURATION_H */

