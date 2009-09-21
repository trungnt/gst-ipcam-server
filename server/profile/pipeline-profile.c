/**
 * Gst-ipcam-server
 * Server profile
 * Copyright (C) 2009 Duong Chien Thang <duongchien.thang@nomovok.com>
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
 * Created on September 11, 2009, 5:30 PM
 */
#include <glib.h>
#include <glib/gprintf.h>

#include "pipeline-profile.h"

/**
 * Allocate memory and initialize an empty GstRTSPPipelineProfile structure.
 *
 * @return GstRTSPPipelineProfile an empty GstRTSPPipelineProfile structure
 */
static GstRTSPPipelineProfile * gst_rtsp_pipeline_profile_alloc();

/**
 * Add a variable to a profile.
 * If a variable is already existed, nothing will be added and return FALSE
 *
 * @param profile GstRTSPPipelineProfile* the profile we want to use
 * @param var_name gchar* the variable name
 * @param init_value gchar* default value for the variable
 *
 * @return gboolean TRUE if the variable is added successful, FALSE otherwise
 */
static gboolean gst_rtsp_pipeline_profile_add_var(GstRTSPPipelineProfile * profile, const gchar * var_name, const gchar * init_value);

/**
 * Check if the given profile has a variable with given name.
 *
 * @param profile GstRTSPPipelineProfile* the profile we want to check
 * @param var_name gchar* name of variable to check
 *
 * @return gboolean TRUE if the variable is existed, FALSE otherwise
 */
static gboolean gst_rtsp_pipeline_profile_has_var(GstRTSPPipelineProfile * profile, const gchar * var_name);

/**
 * Function to replace variale it the pipeline with it's value.
 * This function will be passed to g_hash_table_replace() in gst_rtsp_pipeline_profile_build_pipeline() function and should not be called elsewhere
 *
 * @param key gpointer variable name.
 * @param value gpointer value of the variable
 * @param user_data pointer to a string storing pipeline description.
 *
 * @return None
 */
static void gst_rtsp_pipeline_profile_var_replacing_func(gpointer key, gpointer value, gpointer user_data);

/**
 * Set the type for the given pipeline profile.
 * The pipeline type will be store in the configuration file in text form.
 * This function will check and set to GstRTSPPipelineType form.
 * We have the type with the text form as following:
 * GST_RTSP_PIPELINE_TYPE_VIDEO "video"
 * GST_RTSP_PIPELINE_TYPE_AUDIO "audio"
 *
 * Anything else will be set as GST_RTSP_PIPELINE_TYPE_INVALID and FALSE value will be return.
 *
 * @param profile GstRTSPPipelineProfile* the pipeline profile to set type
 * @param type gchar* text form of the type
 * @param line gint the line store pipeline-type informations. Use for warning message only
 *
 * @return gboolean TRUE if the type is set successful, FALSE if something wrong
 */
static gboolean gst_rtsp_pipeline_profile_set_type(GstRTSPPipelineProfile * profile, const gchar * type, gint line);

/**
 * Load the basic information of the pipeline.
 * For the list of basic information, refer to the format of server configuration file.
 *
 * @param profile GstRTSPPipelineProfile* pipeline profile structure to load information to
 * @param lines gchar** array of text line (the contents of the configuration file). Information will be load from here
 * @param start_line gint* the pointer to the start line to parse
 *
 * @return gboolean TRUE if all basic information is load successful, FALSE otherwise
 */
static gboolean gst_rtsp_pipeline_profile_load_basic_info(GstRTSPPipelineProfile * profile, gchar ** lines, gint * start_line);

/**
 * Load the variables to given pipeline
 *
 * @param profile GstRTSPPipelineProfile* pipeline profile structure to load variables to
 * @param lines gchar** contents of the configuration file
 * @param start_line gint* pointer to the start line to parse
 *
 * @return gboolean TRUE if everything is ok, FALSE otherwise
 */
static gboolean gst_rtsp_pipeline_profile_load_vars(GstRTSPPipelineProfile * profile, gchar ** lines, gint * start_line);

static GstRTSPPipelineProfile * gst_rtsp_pipeline_profile_alloc() {
	GstRTSPPipelineProfile * profile = g_malloc(sizeof (GstRTSPPipelineProfile));
	g_return_val_if_fail(profile != NULL, NULL);

	profile->pipeline_name = NULL;
	profile->pipeline_desc = NULL;
	profile->pipeline_codec = NULL;
	profile->vars = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
	if (profile->vars == NULL) {
		gst_rtsp_pipeline_profile_free(profile);
	}

	profile->vars_name = NULL;

	return profile;
}

static gboolean gst_rtsp_pipeline_profile_load_basic_info(GstRTSPPipelineProfile * profile, gchar ** lines, gint * start_line) {
	gint no_lines = g_strv_length(lines);
	gboolean has_name, has_type, has_codec, has_desc;

	g_return_val_if_fail(profile != NULL, FALSE);
	g_return_val_if_fail(lines != NULL, FALSE);

	if (no_lines <= 0 || *start_line >= no_lines) {
		return FALSE;
	}

	// a pipeline need to has name, type, codec and desc
	has_name = has_type = has_codec = has_desc = FALSE;

	while (*start_line < no_lines) {
		if (has_desc == TRUE) { // read all information
			return TRUE;
		}
		gchar * line = g_strstrip(lines[*start_line]);
		if (gst_rtsp_server_configuration_should_skip_line(line) == TRUE) {
			// skip empty line or comments
			(*start_line)++;
			continue;
		}

		gchar ** vars = g_strsplit(line, "=", 2);
		if (g_strv_length(vars) != 2) {
			g_warning("Pipeline profile loader: invalid syntax at line %d", *start_line);
			g_strfreev(vars);
			return FALSE;
		}

		// remove spaces
		g_strstrip(vars[0]);
		g_strstrip(vars[1]);

#define GST_RTSP_PIPELINE_PROFILE_GET_INFO(vars, info_name, has_it, current_line, set_info_code) {\
	if (!has_it) {\
		if (g_strcasecmp(info_name, vars[0]) != 0) {\
			g_warning("Pipeline profile loader: invalid pipeline syntax at line %d (need '" info_name "' but found '%s'", current_line, vars[0]); \
			(current_line)++; \
			g_strfreev(vars); \
			return FALSE; \
		} else { \
			has_it = TRUE; \
			(current_line)++; \
			{ \
				set_info_code; \
			} \
			g_strfreev(vars); \
			continue; \
		} \
	} \
}

		// check for pipeline name
		GST_RTSP_PIPELINE_PROFILE_GET_INFO(vars, "pipeline-name", has_name, *start_line, gst_rtsp_pipeline_profile_set_name(profile, vars[1]));
		// check for pipeline type
		GST_RTSP_PIPELINE_PROFILE_GET_INFO(vars, "pipeline-type", has_type, *start_line,
				// set the type of pipeline
				if (gst_rtsp_pipeline_profile_set_type(profile, vars[1], *start_line) == FALSE) {
					g_strfreev(vars);
					return FALSE;
				});

		// check for pipeline codec
		GST_RTSP_PIPELINE_PROFILE_GET_INFO(vars, "pipeline-codec", has_codec, *start_line, profile->pipeline_codec = g_strdup(vars[1]));
		// check for pipeline description
		GST_RTSP_PIPELINE_PROFILE_GET_INFO(vars, "pipeline-desc", has_desc, *start_line, profile->pipeline_desc = g_strdup(vars[1]));

#undef GST_RTSP_PIPELINE_PROFILE_GET_INFO

		/* old checking code */
		// check for pipeline name
/*
		if (!has_name) {
			if (g_strcasecmp("pipeline-name", g_strstrip(vars[0])) != 0) {
				// we need to has the name first
				g_warning("Pipeline profile loader: invalid pipeline syntax at line %d (need pipeline-name but found '%s')", *start_line, vars[0]);
				(*start_line)++;
				g_strfreev(vars);
				return FALSE;
			} else {
				g_message("set pipeline name");
				has_name = TRUE;
				gst_rtsp_pipeline_profile_set_name(profile, vars[1]);
				g_message("Done with settings");
				g_strfreev(vars);
				(*start_line)++;
				continue;
			}
		}
*/

/*
		g_message("Checking pipeline type");
*/
		// check for pipeline type
/*
		if (!has_type) {
			if (g_strcasecmp("pipeline-type", g_strstrip(vars[0])) != 0) {
				g_warning("Pipeline profile loader: invalid pipeline syntax at line %d (need pipeline-type but found '%s')", *start_line, vars[0]);
				(*start_line)++;
				g_strfreev(vars);
				return FALSE;
			} else {
				has_type = TRUE;
				(*start_line)++;
				if (gst_rtsp_pipeline_profile_set_type(profile, vars[1]) == FALSE) {
					g_strfreev(vars);
					return FALSE;
				}
				g_strfreev(vars);
				continue;
			}
		}
*/

		// check for codec
/*
		if (!has_codec) {
			if (g_strcasecmp("pipeline-codec", g_strstrip(vars[0])) != 0) {
				g_warning("Pipeline profile loader: invalid pipeline syntax at line %d (need pipeline-codec but found '%s')", *start_line, vars[0]);
				(*start_line)++;
				g_strfreev(vars);
				return FALSE;
			} else {
				has_codec = TRUE;
				(*start_line)++;
				profile->pipeline_codec = g_strdup(vars[1]);
				g_strfreev(vars);
				continue;
			}
		}
*/

		// check for pipeline desc
/*
		if (!has_desc) {
			if (g_strcasecmp("pipeline-desc", g_strstrip(vars[0])) != 0) {
				g_warning("Pipeline profile loader: invalid pipeline syntax at line %d (need pipeline-desc but found '%s')", *start_line, vars[0]);
				(*start_line)++;
				g_strfreev(vars);
				return FALSE;
			} else {
				has_desc = TRUE;
				(*start_line)++;
				profile->pipeline_desc = g_strdup(vars[1]);
				g_strfreev(vars);
				return TRUE; // everything is passed at this step
			}
		}
*/
	}
	return TRUE;
}

static gboolean gst_rtsp_pipeline_profile_load_vars(GstRTSPPipelineProfile* profile, gchar** lines, gint* start_line) {
	gint no_lines = g_strv_length(lines);
	gboolean can_read_vars = FALSE;
	gchar * pipeline_vars_section_name = NULL;
	gchar * line;

	g_return_val_if_fail(profile != NULL, FALSE);
	g_return_val_if_fail(lines != NULL, FALSE);

	if (no_lines <= 0 || *start_line >= no_lines) {
		return FALSE;
	}

	pipeline_vars_section_name = g_strconcat("[", profile->pipeline_name, "-variables]", NULL);

	while (*start_line < no_lines) {
		line = g_strstrip(lines[*start_line]);
		// skip empty line or comments
		if (gst_rtsp_server_configuration_should_skip_line(line) == TRUE) {
			(*start_line)++;
			continue;
		}

		// check for new pipeline
		if (g_strcasecmp("[pipeline]", line) == 0) {
			return TRUE;
		}

		// check for the section name
		if (!can_read_vars) {
			if (g_strcasecmp(pipeline_vars_section_name, line) == 0) {
				can_read_vars = TRUE;
				g_free(pipeline_vars_section_name);
			} else {
				g_warning("Pipeline profile loader: invalid syntax at line %d (need the variable section '%s' but found '%s'", *start_line, pipeline_vars_section_name, line);
			}
			(*start_line)++;
			continue;
		}

		// now we should read variables
		{
			gchar ** vars = g_strsplit(line, "=", -1);
			if (g_strv_length(vars) != 2) {
				g_warning("Pipeline profile loader: invalid variable syntax at line %d", *start_line);
				(*start_line)++;
				g_strfreev(vars);
				continue;
			}
			// remove space
			g_strstrip(vars[0]);
			g_strstrip(vars[1]);
			if (gst_rtsp_pipeline_profile_add_var(profile, vars[0], vars[1]) == FALSE) {
				g_warning("Error when inserting var '%s'", vars[0]);
			}
			(*start_line)++;
			g_strfreev(vars);
		}
	}

	return TRUE;
}

GstRTSPPipelineProfile * gst_rtsp_pipeline_profile_load(const gchar* name) {
	gchar * contents;
	GError * error;
	gchar ** lines;
	gchar * current_line;
	gint start_line = 0;
	gint no_lines;
	GstRTSPPipelineProfile * profile = NULL;

	// we can load if the given name is NULL
	g_return_val_if_fail(name != NULL, NULL);

	// get the contents
	if (g_file_get_contents(name, &contents, NULL, &error) == FALSE) {
		g_warning("Pipeline profile loader error: %s", error->message);
		g_error_free(error);
		return NULL;
	}

	g_return_val_if_fail(contents != NULL, NULL);
	// parse the contents
	lines = g_strsplit(contents, "\n", -1);
	g_free(contents);

	// searching for the first [pipeline] section
	no_lines = g_strv_length(lines);
	while (start_line < no_lines) { 
		current_line = g_strstrip(lines[start_line]);
		start_line++;
		if (g_strcasecmp("[pipeline]", current_line) == 0) {
			break;
		}
	}
	profile = gst_rtsp_pipeline_profile_load_from_text(lines, &start_line);

	g_strfreev(lines);

	return profile;
}

GstRTSPPipelineProfile * gst_rtsp_pipeline_profile_load_from_text(gchar ** lines, gint * start_line) {
	GstRTSPPipelineProfile * profile = gst_rtsp_pipeline_profile_alloc();
	g_return_val_if_fail(profile != NULL, NULL);


	{
		gint no_lines = g_strv_length(lines);
		if (no_lines <= 0 || (*start_line) >= no_lines) {
			gst_rtsp_pipeline_profile_free(profile);
			return NULL;
		}

		// load pipeline basic info
		if (gst_rtsp_pipeline_profile_load_basic_info(profile, lines, start_line) == FALSE) {
			gst_rtsp_pipeline_profile_free(profile);
			return NULL;
		}
		// next will be vars with the format var_name=default_value

		gst_rtsp_pipeline_profile_load_vars(profile, lines, start_line);
	}

	profile->vars_name = g_list_first(profile->vars_name);

	return profile;
}

void gst_rtsp_pipeline_profile_free(GstRTSPPipelineProfile* profile) {
	if (profile != NULL) {
		if (profile->pipeline_name != NULL) {
			g_free(profile->pipeline_name);
		}

		if (profile->pipeline_desc != NULL) {
			g_free(profile->pipeline_desc);
		}

		if (profile->pipeline_codec != NULL) {
			g_free(profile->pipeline_codec);
		}
		
		if (profile->vars != NULL) {
			g_hash_table_destroy(profile->vars);
			//g_free(profile->vars);
		}

		if (profile->vars_name != NULL) {
			g_list_free(profile->vars_name);
			//g_free(profile->vars_name);
		}

		g_free(profile);
	}
}

void gst_rtsp_pipeline_profile_set_name(GstRTSPPipelineProfile * profile, const gchar * name) {
	g_return_if_fail(profile != NULL);

	if (profile->pipeline_name != NULL) {
		g_free(profile->pipeline_name);
	}

	profile->pipeline_name = g_strdup(name);
}

const gchar * gst_rtsp_pipeline_profile_get_name(const GstRTSPPipelineProfile * profile) {
	g_return_val_if_fail(profile != NULL, NULL);

	return profile->pipeline_name;
}

GstRTSPPipelineType gst_rtsp_pipeline_profile_get_type(const GstRTSPPipelineProfile* profile) {
	g_return_val_if_fail(profile != NULL, GST_RTSP_PIPELINE_TYPE_INVALID);

	return profile->pipeline_type;
}

gboolean gst_rtsp_pipeline_profile_set_var(GstRTSPPipelineProfile * profile, const gchar * var_name, const gchar* value) {
	g_return_val_if_fail(profile != NULL, FALSE);
	g_return_val_if_fail(var_name != NULL, FALSE);
	// what if in the case that we want to unset value for a var ??
	g_return_val_if_fail(value != NULL, FALSE);

	// do not allow to set an unexisted variable
	if (!gst_rtsp_pipeline_profile_has_var(profile, var_name)) {
		g_warning("gis_profile_set_var(): No variable name '%s'", var_name);
		return FALSE;
	}

	gchar * var = g_strdup(var_name);
	g_hash_table_replace(profile->vars, var, g_strdup(value));
	return TRUE;
}

static gboolean gst_rtsp_pipeline_profile_has_var(GstRTSPPipelineProfile * profile, const gchar * var_name) {
	GList * first = profile->vars_name;
	GList * node = first;

	gchar * var;
	while (node != NULL) {
		var = (gchar*) node->data;
		if (var != NULL && (g_strcasecmp(var, var_name) == 0)) {
			return TRUE;
		}
		node = g_list_next(node);
	}

	return FALSE;
}

static gboolean gst_rtsp_pipeline_profile_add_var(GstRTSPPipelineProfile * profile, const gchar * var_name, const gchar* init_value) {
	gchar * name;
	g_return_val_if_fail(profile != NULL, FALSE);
	g_return_val_if_fail(var_name != NULL, FALSE);
	g_return_val_if_fail(init_value != NULL, FALSE);

	g_return_val_if_fail(gst_rtsp_pipeline_profile_has_var(profile, var_name) == FALSE, FALSE);

	name = g_strdup(var_name);
	profile->vars_name = g_list_append(profile->vars_name, name);
	g_hash_table_insert(profile->vars, name, g_strdup(init_value));

	return TRUE;
}

gchar * gst_rtsp_pipeline_profile_build_pipeline(GstRTSPPipelineProfile * profile) {
	g_return_val_if_fail(profile != NULL, NULL);

	gchar * pipeline_desc = g_strdup(profile->pipeline_desc);
	g_hash_table_foreach(profile->vars, gst_rtsp_pipeline_profile_var_replacing_func, &pipeline_desc);

	return pipeline_desc;
}

static void gst_rtsp_pipeline_profile_var_replacing_func(gpointer key, gpointer value, gpointer user_data) {
	gchar ** pipeline_desc_ptr = (gchar**) user_data;
	gchar * pipeline_desc = *pipeline_desc_ptr;
	// var name is 'name', but in the pipeline description, it's represented as '${name}'
	gchar * var = g_strconcat("${", (gchar*) key, "}", NULL);
	gchar * var_value = (gchar*) value;
	gchar * temp_pipeline = NULL;

	gchar ** strv = g_strsplit(pipeline_desc, var, -1);
	temp_pipeline = g_strjoinv(var_value, strv);
	g_strfreev(strv);
	g_free(pipeline_desc);
	*pipeline_desc_ptr = temp_pipeline;
}

const GList * gst_rtsp_pipeline_profile_get_vars_list(GstRTSPPipelineProfile * profile) {
	g_return_val_if_fail(profile != NULL, NULL);

	return profile->vars_name;
}

static gboolean gst_rtsp_pipeline_profile_set_type(GstRTSPPipelineProfile* profile, const gchar* type, gint line) {
	g_return_val_if_fail(profile != NULL, FALSE);

	if (g_strcasecmp(type, "video") == 0) {
		profile->pipeline_type = GST_RTSP_PIPELINE_TYPE_VIDEO;
		return TRUE;
	} else if (g_strcasecmp(type, "audio") == 0) {
		profile->pipeline_type = GST_RTSP_PIPELINE_TYPE_AUDIO;
		return TRUE;
	} else {
		profile->pipeline_type = GST_RTSP_PIPELINE_TYPE_INVALID;
		g_warning("Pipeline profile loader: invalid pipeline type '%s' at line %d", type, line);
	}

	return FALSE;
}

gboolean gst_rtsp_server_configuration_should_skip_line(const gchar* line) {
	if (line != NULL) {
		if (line[0] == '\0' || line[0] == '#') {
			return TRUE;
		}
	}

	return FALSE;
}