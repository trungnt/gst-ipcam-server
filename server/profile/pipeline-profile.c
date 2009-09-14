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
 * Allocate memory and initialize an empty GISProfile structure.
 *
 * @param None
 * @return GISProfile an empty GISProfile structure
 */
static GISProfile * gis_profile_alloc();

/**
 * Add a variable to a profile.
 * If a variable is already existed, nothing will be added and return FALSE
 *
 * @param profile GISProfile* the profile we want to use
 * @param var_name gchar* the variable name
 * @param init_value gchar* default value for the variable
 *
 * @return gboolean TRUE if the variable is added successful, FALSE otherwise
 */
static gboolean gis_profile_add_var(GISProfile * profile, const gchar * var_name, const gchar * init_value);

/**
 * Check if the given profile has a variable with given name.
 *
 * @param profile GISProfile* the profile we want to check
 * @param var_name gchar* name of variable to check
 *
 * @return gboolean TRUE if the variable is existed, FALSE otherwise
 */
static gboolean gis_profile_has_var(GISProfile * profile, const gchar * var_name);

/**
 * Function to replace variale it the pipeline with it's value.
 * This function will be passed to g_hash_table_replace() in gis_profile_build_pipeline() function and should not be called elsewhere
 *
 * @param key gpointer variable name.
 * @param value gpointer value of the variable
 * @param user_data pointer to a string storing pipeline description.
 *
 * @return None
 */
static void gis_profile_var_replacing_func(gpointer key, gpointer value, gpointer user_data);

static GISProfile * gis_profile_alloc() {
	GISProfile * profile = g_malloc(sizeof (GISProfile));
	g_return_val_if_fail(profile != NULL, NULL);

	profile->pipeline_desc = NULL;
	profile->vars = g_hash_table_new(g_str_hash, g_str_equal);
	if (profile->vars == NULL) {
		gis_profile_free(profile);
	}

	profile->vars_name = g_list_alloc();
	if (profile->vars_name == NULL) {
		gis_profile_free(profile);
	}

	return profile;
}

GISProfile * gis_profile_load(const gchar* file_name) {
	GISProfile * profile = gis_profile_alloc();
	g_return_val_if_fail(profile != NULL, NULL);

	g_return_val_if_fail(file_name != NULL, NULL);

	{// now read file
		gchar * contents;
		GError * error;
		if (g_file_get_contents(file_name, &contents, NULL, &error) == FALSE) {
			g_warning("Error: %s", error->message);
			g_error_free(error);
			gis_profile_free(profile);
			return NULL;
		}

		gchar ** lines = g_strsplit(contents, "\n", -1);
		g_free(contents);
		gint no_lines = g_strv_length(lines);
		if (no_lines <= 0) {
			gis_profile_free(profile);
			return NULL;
		}

		// first line will be the pipeline
		profile->pipeline_desc = g_strdup(lines[0]);
		// next will be vars with the format var_name=default_value
		gint i;
		for (i = 1; i < no_lines; i++) {
			gchar * line = lines[i];
			gchar ** vars = g_strsplit(line, "=", -1);
			if (g_strv_length(vars) != 2) {
				g_warning("invalid format at line %d", i);
				continue;
			}
			if (gis_profile_add_var(profile, vars[0], vars[1]) == FALSE) {
				g_warning("Error when inserting var '%s'", vars[0]);
			}
			g_strfreev(vars);
		}

		g_strfreev(lines);
	}

	profile->vars_name = g_list_first(profile->vars_name);

	return profile;
}

void gis_profile_free(GISProfile* profile) {
	if (profile != NULL) {
		if (profile->pipeline_desc != NULL) {
			g_free(profile->pipeline_desc);
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

gboolean gis_profile_set_var(GISProfile* profile, const gchar* var_name, const gchar* value) {
	g_return_val_if_fail(profile != NULL, FALSE);
	g_return_val_if_fail(var_name != NULL, FALSE);
	// what if in the case that we want to unset value for a var ??
	g_return_val_if_fail(value != NULL, FALSE);

	// do not allow to set an unexisted variable
	if (!gis_profile_has_var(profile, var_name)) {
		g_warning("gis_profile_set_var(): No variable name '%s'", var_name);
		return FALSE;
	}

	gchar * var = g_strdup(var_name);
	g_hash_table_replace(profile->vars, var, g_strdup(value));
	return TRUE;
}

static gboolean gis_profile_has_var(GISProfile* profile, const gchar* var_name) {
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

static gboolean gis_profile_add_var(GISProfile* profile, const gchar* var_name, const gchar* init_value) {
	g_return_val_if_fail(profile != NULL, FALSE);
	g_return_val_if_fail(var_name != NULL, FALSE);
	g_return_val_if_fail(init_value != NULL, FALSE);

	g_return_val_if_fail(gis_profile_has_var(profile, var_name) == FALSE, FALSE);

	profile->vars_name = g_list_append(profile->vars_name, g_strdup(var_name));
	g_hash_table_insert(profile->vars, g_strdup(var_name), g_strdup(init_value));

	return TRUE;
}

gchar * gis_profile_build_pipeline(GISProfile* profile) {
	g_return_val_if_fail(profile != NULL, NULL);

	gchar * pipeline_desc = g_strdup(profile->pipeline_desc);
	g_hash_table_foreach(profile->vars, gis_profile_var_replacing_func, &pipeline_desc);

	return pipeline_desc;
}

static void gis_profile_var_replacing_func(gpointer key, gpointer value, gpointer user_data) {
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

const GList * gis_profile_get_vars_list(GISProfile* profile) {
	g_return_val_if_fail(profile != NULL, NULL);

	return profile->vars_name;
}