/**
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
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
 */

/**
 * SECTION:element-bitrate
 *
 * FIXME:Describe bitrate here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! bitrate ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>
#include <glib-2.0/glib/gtypes.h>

#include "gstbitrate.h"

GST_DEBUG_CATEGORY_STATIC(gst_bit_rate_debug);
#define GST_CAT_DEFAULT gst_bit_rate_debug

/* Filter signals and args */
enum {
	/* FILL ME */
	LAST_SIGNAL
};

/**
 * Property ids of bitrate element
 */
enum {
	PROP_0,
	PROP_ENCODER_NAME,
	PROP_BITRATE
};

/** default value for bitrate */
#define GST_BIT_RATE_DEFAULT_BITRATE 300000
#define GST_BIT_RATE_MIN_BITRATE 0
#define GST_BIT_RATE_MAX_BITRATE G_MAXULONG

/**
 * the capabilities of the inputs and outputs.
 * We accept anything here
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE("sink",
		GST_PAD_SINK,
		GST_PAD_ALWAYS,
		GST_STATIC_CAPS("ANY")
		);

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE("src",
		GST_PAD_SRC,
		GST_PAD_ALWAYS,
		GST_STATIC_CAPS("ANY")
		);

GST_BOILERPLATE(GstBitRate, gst_bit_rate, GstElement,
		GST_TYPE_ELEMENT);

/**
 * Set property method of bitrate element
 * Params as normal set property method.
 * This method will be called by g_object_set*()
 */
static void gst_bit_rate_set_property(GObject * object, guint prop_id,
		const GValue * value, GParamSpec * pspec);

/**
 * Get property method of bitrate element
 * Use same function signature as normal GObject get property method
 * This method will be called by g_object_get*()
 */
static void gst_bit_rate_get_property(GObject * object, guint prop_id,
		GValue * value, GParamSpec * pspec);

/**
 * Set caps for bitrate's pad
 *
 * @param pad GstPad* pad to set
 * @param caps GstCaps* caps to set to
 *
 * @return gboolean TRUE if everything is ok, FALSE otherwise
 */
static gboolean gst_bit_rate_set_caps(GstPad * pad, GstCaps * caps);

/**
 * Chain function for bitrate element.
 * We won't do anything else here more than pushing data from sinkpad to srcpad
 */
static GstFlowReturn gst_bit_rate_chain(GstPad * pad, GstBuffer * buf);

/*static void gst_bit_rate_detect_encoder(GstBitRate * bitrate_element);*/

/**
 * Set encoder name to bitrate element.
 * Find the corespoding encoder and set to bitrate element too
 *
 * @param bitrate_element GstBitRate* element to set encoder name
 * @param encoder_name gchar* name of the encoder to control
 *
 * @return gboolean TRUE if found the encoder with same name, FALSE otherwise
 */
static gboolean gst_bit_rate_set_encoder_with_name(GstBitRate * bitrate_element, const gchar * encoder_name);

/**
 * Set bitrate to the controlled encoder.
 * If there isn't any encoder under controlled of bitrate element, nothing will be changed.
 * Also, if new bitrate value is the same as the old one, nothing will be changed.
 *
 * @param bitrate_element GstBitRate* bitrate_element that control encoder.
 * @param bitrate gulong bitrate value to set to
 *
 * @return gboolean TRUE if everything is ok, FALSE otherwise
 */
static gboolean gst_bit_rate_set_bitrate(GstBitRate * bitrate_element, gulong bitrate);

/**
 * Stop pipeline to set bitrate
 *
 * @param bitrate_element GstBitRate* bitrate element that control the encoder
 *
 * @return None
 */
static void gst_bit_rate_stop_pipeline(GstBitRate * bitrate_element);

/**
 * Start pipeline to apply new bitrate value.
 *
 * @param bitrate_element GstBitRate* bitrate element that control the encoder
 *
 * @return None
 */
static void gst_bit_rate_start_pipeline(GstBitRate * bitrate_element);

/**
 * Idle function to set bitrate if we can't set it before.
 *
 * @param data gpointer pointer to bitrate element
 * @return gboolean FALSE if this idle function should be remove from timeout list, TRUE otherwise.
 */
static gboolean gst_bit_rate_set_bitrate_idle(gpointer data);

/**
 * Idle function to set encoder name if we can't set it before
 *
 * @param data gpointer pointer to bitrate element
 * @return gboolean FALSE if finished setting encoder name (idle function won't be called again), TRUE otherwise
 */
static gboolean gst_bit_rate_set_encoder_name_idle(gpointer data);

/**
 * for debugging colorization
 */

/* GObject vmethod implementations */

/**
 * Init basic information for bitrate element
 */
static void
gst_bit_rate_base_init(gpointer gclass) {
	GstElementClass *element_class = GST_ELEMENT_CLASS(gclass);

	gst_element_class_set_details_simple(element_class,
			"BitRate",
			"Effect/Video/Bitrate",
			"Runtime adjust video encoder's bitrate",
			"Duong Chien Thang <duongchien.thang@nomovok.com> & Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com>");

	gst_element_class_add_pad_template(element_class,
			gst_static_pad_template_get(&src_factory));
	gst_element_class_add_pad_template(element_class,
			gst_static_pad_template_get(&sink_factory));
}

/**
 * initialize the bitrate's class
 */
static void
gst_bit_rate_class_init(GstBitRateClass * klass) {
	GObjectClass *gobject_class;
	GstElementClass *gstelement_class;

	gobject_class = (GObjectClass *) klass;
	gstelement_class = (GstElementClass *) klass;

	gobject_class->set_property = gst_bit_rate_set_property;
	gobject_class->get_property = gst_bit_rate_get_property;

	g_object_class_install_property(gobject_class, PROP_ENCODER_NAME,
			g_param_spec_string("encoder-name", "Encoder name", "Video encoder object name",
			NULL, G_PARAM_READWRITE));
	g_object_class_install_property(gobject_class, PROP_BITRATE,
			g_param_spec_ulong("bitrate", "Bitrate", "bitrate value for the encoder",
			GST_BIT_RATE_MIN_BITRATE, GST_BIT_RATE_MAX_BITRATE,
			GST_BIT_RATE_DEFAULT_BITRATE, G_PARAM_READWRITE));
}

/**
 * initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 * 
 */
static void
gst_bit_rate_init(GstBitRate * bitrate_element,
		GstBitRateClass * gclass) {
	bitrate_element->sinkpad = gst_pad_new_from_static_template(&sink_factory, "sink");
	gst_pad_set_setcaps_function(bitrate_element->sinkpad,
			GST_DEBUG_FUNCPTR(gst_bit_rate_set_caps));
	gst_pad_set_getcaps_function(bitrate_element->sinkpad,
			GST_DEBUG_FUNCPTR(gst_pad_proxy_getcaps));
	gst_pad_set_chain_function(bitrate_element->sinkpad,
			GST_DEBUG_FUNCPTR(gst_bit_rate_chain));

	bitrate_element->srcpad = gst_pad_new_from_static_template(&src_factory, "src");
	gst_pad_set_getcaps_function(bitrate_element->srcpad,
			GST_DEBUG_FUNCPTR(gst_pad_proxy_getcaps));

	gst_element_add_pad(GST_ELEMENT(bitrate_element), bitrate_element->sinkpad);
	gst_element_add_pad(GST_ELEMENT(bitrate_element), bitrate_element->srcpad);
	bitrate_element->encoder_name = NULL;
	bitrate_element->encoder = NULL;
	bitrate_element->is_bitrate_waiting = FALSE;
	bitrate_element->waiting_bitrate = 0;
	bitrate_element->waiting_encoder_name = NULL;
	bitrate_element->is_encoder_name_waiting = FALSE;

/*
	gst_bit_rate_detect_encoder(bitrate_element);
*/
}

static void
gst_bit_rate_set_property(GObject * object, guint prop_id,
		const GValue * value, GParamSpec * pspec) {
	GstBitRate *bitrate_element = GST_BITRATE(object);

	switch (prop_id) {
		case PROP_ENCODER_NAME:
		{
			const gchar * name = g_value_get_string(value);
			gst_bit_rate_set_encoder_with_name(bitrate_element, name);
		}
			break;
		case PROP_BITRATE:
		{
			gulong bitrate = g_value_get_ulong(value);
			gst_bit_rate_set_bitrate(bitrate_element, bitrate);
		}
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

static void
gst_bit_rate_get_property(GObject * object, guint prop_id,
		GValue * value, GParamSpec * pspec) {
	GstBitRate *bitrate_element = GST_BITRATE(object);

	switch (prop_id) {
		case PROP_ENCODER_NAME:
			g_value_set_string(value, bitrate_element->encoder_name);
			break;
		case PROP_BITRATE:
			if (bitrate_element->encoder != NULL) {
				g_object_get(G_OBJECT(bitrate_element->encoder), "bitrate", value, NULL);
			} else {
				g_value_set_ulong(value, 0);
			}
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

/* GstElement vmethod implementations */

/* this function handles the link with other elements */
static gboolean
gst_bit_rate_set_caps(GstPad * pad, GstCaps * caps) {
	GstBitRate *bitrate_element;
	GstPad *otherpad;

	bitrate_element = GST_BITRATE(gst_pad_get_parent(pad));
	otherpad = (pad == bitrate_element->srcpad) ? bitrate_element->sinkpad : bitrate_element->srcpad;
	gst_object_unref(bitrate_element);

	return gst_pad_set_caps(otherpad, caps);
}

/**
 * chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_bit_rate_chain(GstPad * pad, GstBuffer * buf) {
	GstBitRate *bitrate_element;

	bitrate_element = GST_BITRATE(GST_OBJECT_PARENT(pad));

	/* just push out the incoming buffer without touching it */
	return gst_pad_push(bitrate_element->srcpad, buf);
}

/*
static void gst_bit_rate_detect_encoder(GstBitRate* bitrate_element) {
	g_debug("Finding the encoder");
	GstElement * peer = NULL;
	GstPad * peer_pad = NULL;
	const gchar * peer_klass = NULL;
	GstElementFactory * peer_factory = NULL;
	if (gst_pad_is_linked(bitrate_element->sinkpad)) {
		g_debug("Yes, sink pad is linked");
		peer_pad = gst_pad_get_peer(bitrate_element->sinkpad);
		g_return_if_fail(peer_pad != NULL);
		peer = gst_pad_get_parent_element(peer_pad);
		g_return_if_fail(peer != NULL);
		// now check for peer class
		peer_factory = gst_element_get_factory(peer);
		peer_klass = gst_element_factory_get_klass(peer_factory);
		if (g_strcasecmp(peer_klass, "Codec/Encoder/Video") == 0) {
			g_debug("Found the encoder with name %s", gst_element_get_name(peer));
			// ok, this's a video encoder
			bitrate_element->encoder = peer;
			bitrate_element->encoder_name = g_strdup(gst_element_get_name(peer));
		} else {
			g_debug("Can not find the encoder");
			gst_object_unref(peer);
		}
		gst_object_unref(peer_pad);
	}
}
*/

static gboolean gst_bit_rate_set_encoder_with_name(GstBitRate* bitrate_element, const gchar* encoder_name) {
	GstElement * pipeline = GST_ELEMENT(gst_element_get_parent(bitrate_element));
	if (pipeline != NULL) {
		GstElement * encoder;
		encoder = gst_bin_get_by_name(GST_BIN(pipeline), encoder_name);
		if (encoder != NULL) {
			bitrate_element->encoder = encoder;
			if (bitrate_element->encoder_name != NULL) {
				g_free(bitrate_element->encoder_name);
			}
			bitrate_element->encoder_name = g_strdup(encoder_name);
		}
		bitrate_element->is_encoder_name_waiting = FALSE;
		if (bitrate_element->waiting_encoder_name != NULL) {
			g_free(bitrate_element->waiting_encoder_name);
			bitrate_element->waiting_encoder_name = NULL;
		}
		return TRUE;
	}

	if (g_strcmp0(bitrate_element->waiting_encoder_name, encoder_name) != 0) {
		if (bitrate_element->waiting_encoder_name != NULL) {
			g_free(bitrate_element->waiting_encoder_name);
		}
		bitrate_element->waiting_encoder_name = g_strdup(encoder_name);
	}
	if (bitrate_element->is_encoder_name_waiting == FALSE) {
		bitrate_element->is_encoder_name_waiting = TRUE;
		g_timeout_add_seconds(1, gst_bit_rate_set_encoder_name_idle, bitrate_element);
	}
	return FALSE;
}

static gboolean gst_bit_rate_set_bitrate(GstBitRate* bitrate_element, gulong bitrate) {
	if (bitrate_element->encoder != NULL) {
		gulong old_bitrate = 0;
		g_object_get(bitrate_element->encoder, "bitrate", &old_bitrate, NULL);
		if (old_bitrate != bitrate) {
			gst_bit_rate_stop_pipeline(bitrate_element);
			g_object_set(G_OBJECT(bitrate_element->encoder), "bitrate", bitrate, NULL);
			gst_bit_rate_start_pipeline(bitrate_element);
		}
		bitrate_element->is_bitrate_waiting = FALSE;
		return TRUE;
	}
	/* set waiting value and idle function */
	bitrate_element->waiting_bitrate = bitrate;
	if (bitrate_element->is_bitrate_waiting == FALSE) {
		bitrate_element->is_bitrate_waiting = TRUE;
		g_timeout_add_seconds(1, gst_bit_rate_set_bitrate_idle, bitrate_element);
	}

	return FALSE;
}

/**
 * stop the pipeline if needed
 * We need to stop pipeline and then start it again for renegotiation process
 *
 * @param bitrate_element GstBitRate* our element to get the pipeline
 */
static void gst_bit_rate_stop_pipeline(GstBitRate* bitrate_element) {
	GstElement * pipeline = GST_ELEMENT(gst_element_get_parent(bitrate_element));
	if (pipeline != NULL) {
		gst_element_set_state(pipeline, GST_STATE_READY);
	}
}

/**
 * start the pipeline if needed
 *
 * @param bitrate_element GstBitRate* our element to get the pipeline
 */
static void gst_bit_rate_start_pipeline(GstBitRate* bitrate_element) {
	GstElement * pipeline = GST_ELEMENT(gst_element_get_parent(bitrate_element));
	if (pipeline != NULL) {
		gst_element_set_state(pipeline, GST_STATE_PLAYING);
	}
}

static gboolean gst_bit_rate_set_bitrate_idle(gpointer data) {
	GstBitRate * bitrate_element = (GstBitRate *) data;
	if (bitrate_element->is_bitrate_waiting == TRUE) {
		return (!gst_bit_rate_set_bitrate(bitrate_element, bitrate_element->waiting_bitrate));
	}

	return FALSE;
}

static gboolean gst_bit_rate_set_encoder_name_idle(gpointer data) {
	GstBitRate * bitrate_element = (GstBitRate *) data;
	if (bitrate_element->is_encoder_name_waiting == TRUE) {
		return (!gst_bit_rate_set_encoder_with_name(bitrate_element, bitrate_element->waiting_encoder_name));
	}

	return FALSE;
}

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
bitrate_init(GstPlugin * bitrate) {
	/* debug category for fltering log messages
	 *
	 * exchange the string 'Template bitrate' with your description
	 */
	GST_DEBUG_CATEGORY_INIT(gst_bit_rate_debug, "bitrate",
			0, "runtime adjust video encoder's bitrate");

	return gst_element_register(bitrate, "bitrate", GST_RANK_NONE,
			GST_TYPE_BITRATE);
}

/* gstreamer looks for this structure to register bitrates
 *
 * exchange the string 'Template bitrate' with your bitrate description
 */
GST_PLUGIN_DEFINE(
		GST_VERSION_MAJOR,
		GST_VERSION_MINOR,
		"bitrate",
		"runtime adjust video encoder's bitrate",
		bitrate_init,
		VERSION,
		"LGPL",
		"GStreamer",
		"http://gstreamer.net/"
		)
