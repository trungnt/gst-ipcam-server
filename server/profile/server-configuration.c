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
#include "server-configuration.h"
#include "string.h"

/**
 * Allocation memory for a server configuration structure.
 *
 * @return GstRTSPServerConfiguration* an empty GstRTSPServerConfiguration structure or NULL if some thing wrong
 */
static GstRTSPServerConfiguration * gst_rtsp_server_configuration_alloc();

/**
 * Check if the server configuration has the pipeline with given pipeline name
 *
 * @param config GstRTSPServerConfiguration* the configuration to check
 * @param name gchar* the name of the pipeline to check
 *
 * @return gboolean TRUE if yes, FALSE other wise
 */
static gboolean gst_rtsp_server_configuration_has_pipeline(const GstRTSPServerConfiguration * config, const gchar * pipeline_name);

/**
 * Add a pipeline to server configuration structure.
 * If the server already contain a pipeline with same name then nothing will be added
 *
 * @param config GstRTSPServerConfiguration* the configuration to add
 * @param pipeline GstRTSPPipelineProfile* the pipeline profile to add to server configuration
 *
 * @return gboolean TRUE if the pipeline is added, FALSE if something wrong
 */
static gboolean gst_rtsp_server_configuration_add_pipeline(GstRTSPServerConfiguration * config, GstRTSPPipelineProfile * profile);

/**
 * wrap to gst_rtsp_pipeline_profile_free() for used in g_hash_table_new_full cause:
 * - g_hash_table_new_full require value_free(gpointer) as free function
 * - while gst_rtsp_pipeline_profile_free() function uses GstRTSPPipelineProfile* as paramter
 *
 * @param pipeline gpointer pointer to GstRTSPPipelineProfile
 *
 * @return None
 */
static void gst_rtsp_server_configuration_free_pipeline(gpointer pipeline);

static GstRTSPServerConfiguration * gst_rtsp_server_configuration_alloc() {
	GstRTSPServerConfiguration * config = g_malloc(sizeof (GstRTSPServerConfiguration));
	g_return_val_if_fail(config != NULL, NULL);

	config->pipelines_name = NULL; //g_list_alloc();

	config->pipelines = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, gst_rtsp_server_configuration_free_pipeline);

	if (config->pipelines == NULL) {
		gst_rtsp_server_configuration_free(config);
		return NULL;
	}

	config->pipelines_by_index = g_hash_table_new_full(g_int_hash, g_int_equal, g_free, NULL);
	if (config->pipelines_by_index == NULL) {
		gst_rtsp_server_configuration_free(config);
		return NULL;
	}

	config->default_audio_pipeline = NULL;
	config->default_video_pipeline = NULL;

	return config;
}

GstRTSPServerConfiguration * gst_rtsp_server_configuration_load(const gchar* config_file) {
	GstRTSPServerConfiguration * config = gst_rtsp_server_configuration_alloc();

	g_return_val_if_fail(config != NULL, NULL);
	g_return_val_if_fail(config_file != NULL, NULL);

	{
		// now read configuration file
		gchar * contents = NULL;
		GError * error = NULL;
		gchar ** lines = NULL;
		gint no_lines = 0;
		gint current_line = 0;
		gchar * line = NULL;
		GstRTSPPipelineProfile * tmp_profile = NULL;

		if (g_file_get_contents(config_file, &contents, NULL, &error) == FALSE) {
			g_warning("Server configuration loader error: %s", error->message);
			g_error_free(error);
			gst_rtsp_server_configuration_free(config);
			return NULL;
		}

		lines = g_strsplit(contents, "\n", -1);
		g_free(contents);
		no_lines = g_strv_length(lines);
		if (no_lines <= 0) {
			gst_rtsp_server_configuration_free(config);
			return NULL;
		}

		while (current_line < no_lines) {
			line = g_strstrip(lines[current_line]);
			// skip them comments (line start with #) and empty line
			if (gst_rtsp_server_configuration_should_skip_line(line) == TRUE) {
				current_line++;
				continue;
			}

			if (g_strcasecmp(line, "[pipeline]") == 0) {
				current_line++;
				// next section define a pipeline
				tmp_profile = gst_rtsp_pipeline_profile_load_from_text(lines, &current_line);
				if (tmp_profile != NULL) {
					// add the pipeline profile to the configuration
					if (gst_rtsp_server_configuration_add_pipeline(config, tmp_profile) == FALSE) {
						gst_rtsp_pipeline_profile_free(tmp_profile);
					}
				}
			} else {
				// now, anything else mean syntax error
				g_warning("Server configuration loader: invalid syntax at line %d in file '%s'", current_line, config_file);
				current_line++;
			}
		}
	}

	config->pipelines_name = g_list_first(config->pipelines_name);
	return config;
}

void gst_rtsp_server_configuration_free(GstRTSPServerConfiguration* config) {
	if (config != NULL) {
		if (config->pipelines_name != NULL) {
			g_list_free(config->pipelines_name);
		}

		if (config->pipelines != NULL) {
			g_hash_table_destroy(config->pipelines);
		}

		if (config->pipelines_by_index != NULL) {
			g_hash_table_destroy(config->pipelines_by_index);
		}

		if (config->default_video_pipeline != NULL) {
			g_free(config->default_video_pipeline);
		}

		if (config->default_audio_pipeline != NULL) {
			g_free(config->default_audio_pipeline);
		}
		
		g_free(config);
	}
}

GList * gst_rtsp_server_configuration_get_pipelines_name_list(const GstRTSPServerConfiguration* config) {
	g_return_val_if_fail(config != NULL, NULL);
	return config->pipelines_name;
}

GstRTSPPipelineProfile * gst_rtsp_server_configuration_get_pipeline(const GstRTSPServerConfiguration* config, const gchar* name) {
	g_return_val_if_fail(config != NULL, NULL);
	g_return_val_if_fail(name != NULL, NULL);

	if (!gst_rtsp_server_configuration_has_pipeline(config, name)) {
		return NULL;
	}

	return g_hash_table_lookup(config->pipelines, name);
}

GstRTSPPipelineProfile * gst_rtsp_server_configuration_get_pipeline_at(const GstRTSPServerConfiguration* config, gint index) {
	return g_hash_table_lookup(config->pipelines_by_index, &index);
}

static gboolean gst_rtsp_server_configuration_has_pipeline(const GstRTSPServerConfiguration* config, const gchar* pipeline_name) {
	GList *node;
	gchar *name;
	g_return_val_if_fail(config != NULL, FALSE);

	node = g_list_first(config->pipelines_name);
	//	node = first;

	while (node != NULL) {
		name = (gchar*) node->data;
		if (name != NULL && g_strcasecmp(pipeline_name, name) == 0) {
			return TRUE;
		}

		node = g_list_next(node);
	}

	return FALSE;
}

static gboolean gst_rtsp_server_configuration_add_pipeline(GstRTSPServerConfiguration* config, GstRTSPPipelineProfile* profile) {
	gint * index;
	g_return_val_if_fail(config != NULL, FALSE);
	g_return_val_if_fail(profile != NULL, FALSE);
	g_return_val_if_fail(profile->pipeline_name != NULL, FALSE);

	g_return_val_if_fail(gst_rtsp_server_configuration_has_pipeline(config, profile->pipeline_name) != TRUE, FALSE);

	// now adding hehe
	index = g_malloc(sizeof (gint));
	*index = gst_rtsp_server_configuration_get_number_of_pipelines(config);

	config->pipelines_name = g_list_append(config->pipelines_name, profile->pipeline_name);
	g_hash_table_insert(config->pipelines, profile->pipeline_name, profile);
	g_hash_table_insert(config->pipelines_by_index, index, profile);

	/* we will automatic set default video pipeline */
	if (config->default_video_pipeline == NULL && gst_rtsp_pipeline_profile_is_video(profile)) {
		gst_rtsp_server_configuration_set_default_video_pipeline(config, gst_rtsp_pipeline_profile_get_name(profile));
	}

	return TRUE;
}

gint gst_rtsp_server_configuration_get_number_of_pipelines(const GstRTSPServerConfiguration* config) {
	g_return_val_if_fail(config != NULL, -1);

	return g_list_length(config->pipelines_name);
}

static void gst_rtsp_server_configuration_free_pipeline(gpointer pipeline) {
	GstRTSPPipelineProfile * profile = (GstRTSPPipelineProfile *) pipeline;
	gst_rtsp_pipeline_profile_free(profile);
}

gboolean gst_rtsp_server_configuration_set_default_video_pipeline(GstRTSPServerConfiguration* config, const gchar* pipeline_name) {
	GstRTSPPipelineProfile * profile = gst_rtsp_server_configuration_get_pipeline(config, pipeline_name);

	g_return_val_if_fail(profile != NULL, FALSE);

	if (config->default_video_pipeline != NULL) {
		g_free(config->default_video_pipeline);
	}

	config->default_video_pipeline = g_strdup(pipeline_name);

	return TRUE;
}

const gchar * gst_rtsp_server_configuration_get_default_video_pipeline_name(const GstRTSPServerConfiguration* config) {
	g_return_val_if_fail(config != NULL, NULL);

	return config->default_video_pipeline;
}

GstRTSPPipelineProfile * gst_rtsp_server_configuration_get_default_video_pipeline(const GstRTSPServerConfiguration* config) {
	const gchar * profile_name = gst_rtsp_server_configuration_get_default_video_pipeline_name(config);
	return gst_rtsp_server_configuration_get_pipeline(config, profile_name);
}

gboolean gst_rtsp_server_configuration_set_default_audio_pipeline(GstRTSPServerConfiguration* config, const gchar* pipeline_name) {
	GstRTSPPipelineProfile * profile = gst_rtsp_server_configuration_get_pipeline(config, pipeline_name);

	g_return_val_if_fail(profile != NULL, FALSE);

	if (config->default_audio_pipeline != NULL) {
		g_free(config->default_audio_pipeline);
	}

	config->default_audio_pipeline = g_strdup(pipeline_name);

	return TRUE;
}

const gchar * gst_rtsp_server_configuration_get_default_audio_pipeline_name(const GstRTSPServerConfiguration* config) {
	g_return_val_if_fail(config != NULL, NULL);

	return config->default_audio_pipeline;
}

GstRTSPPipelineProfile * gst_rtsp_server_configuration_get_default_audio_pipeline(const GstRTSPServerConfiguration* config) {
	const gchar * profile_name = gst_rtsp_server_configuration_get_default_audio_pipeline_name(config);
	if (profile_name != NULL) {
		return gst_rtsp_server_configuration_get_pipeline(config, profile_name);
	}
	return NULL;
}

gchar * gst_rtsp_server_configuration_build_pipeline(GstRTSPServerConfiguration* config) {
	GstRTSPPipelineProfile * video_profile = gst_rtsp_server_configuration_get_default_video_pipeline(config);
	GstRTSPPipelineProfile * audio_profile = gst_rtsp_server_configuration_get_default_audio_pipeline(config);
	gchar * pipeline = NULL;
	gchar * audio_pipeline = NULL;

	g_assert(video_profile);

	pipeline = gst_rtsp_pipeline_profile_build_pipeline(video_profile);

	if (audio_profile != NULL) {
		audio_pipeline = gst_rtsp_pipeline_profile_build_pipeline(audio_profile);

		if (audio_pipeline != NULL) {
			pipeline = g_strdup_printf("%s%s", g_strndup(pipeline, strlen(pipeline) -1), audio_pipeline);			
		}
	}

	return pipeline;
}