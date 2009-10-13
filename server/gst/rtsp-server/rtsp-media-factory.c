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

#include "rtsp-media-factory.h"
#include "string.h"

#define DEFAULT_LAUNCH         NULL
#define DEFAULT_SHARED         FALSE

enum
{
  PROP_0,
  PROP_LAUNCH,
  PROP_SHARED,
  PROP_LAST
};

static void gst_rtsp_media_factory_get_property (GObject *object, guint propid,
    GValue *value, GParamSpec *pspec);
static void gst_rtsp_media_factory_set_property (GObject *object, guint propid,
    const GValue *value, GParamSpec *pspec);
static void gst_rtsp_media_factory_finalize (GObject * obj);

static gchar * default_gen_key (GstRTSPMediaFactory *factory, const GstRTSPUrl *url);
static GstElement * default_get_element (GstRTSPMediaFactory *factory, const GstRTSPUrl *url);
static GstRTSPMedia * default_construct (GstRTSPMediaFactory *factory, const GstRTSPUrl *url);
static void default_configure (GstRTSPMediaFactory *factory, GstRTSPMedia *media);
static GstElement* default_create_pipeline (GstRTSPMediaFactory *factory, GstRTSPMedia *media);

G_DEFINE_TYPE (GstRTSPMediaFactory, gst_rtsp_media_factory, G_TYPE_OBJECT);

static void
gst_rtsp_media_factory_class_init (GstRTSPMediaFactoryClass * klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->get_property = gst_rtsp_media_factory_get_property;
  gobject_class->set_property = gst_rtsp_media_factory_set_property;
  gobject_class->finalize = gst_rtsp_media_factory_finalize;

  /**
   * GstRTSPMediaFactory::launch
   *
   * The gst_parse_launch() line to use for constructing the pipeline in the
   * default prepare vmethod.
   *
   * The pipeline description should return a GstBin as the toplevel element
   * which can be accomplished by enclosing the dscription with brackets '('
   * ')'.
   *
   * The description should return a pipeline with payloaders named pay0, pay1,
   * etc.. Each of the payloaders will result in a stream.
   */
  g_object_class_install_property (gobject_class, PROP_LAUNCH,
      g_param_spec_string ("launch", "Launch", "A launch description of the pipeline",
          DEFAULT_LAUNCH, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_SHARED,
      g_param_spec_boolean ("shared", "Shared", "If media from this factory is shared",
          DEFAULT_SHARED, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  klass->gen_key = default_gen_key;
  klass->get_element = default_get_element;
  klass->construct = default_construct;
  klass->configure = default_configure;
  klass->create_pipeline = default_create_pipeline;
}

static void
gst_rtsp_media_factory_init (GstRTSPMediaFactory * factory)
{
  factory->launch = g_strdup (DEFAULT_LAUNCH);
  factory->shared = DEFAULT_SHARED;

  factory->lock = g_mutex_new ();
  factory->medias_lock = g_mutex_new ();
  factory->medias = g_hash_table_new_full (g_str_hash, g_str_equal,
		  g_free, g_object_unref);
}

static void
gst_rtsp_media_factory_finalize (GObject * obj)
{
  GstRTSPMediaFactory *factory = GST_RTSP_MEDIA_FACTORY (obj);

  g_hash_table_unref (factory->medias);
  g_mutex_free (factory->medias_lock);
  g_free (factory->launch);
  g_mutex_free (factory->lock);

  G_OBJECT_CLASS (gst_rtsp_media_factory_parent_class)->finalize (obj);
}

static void
gst_rtsp_media_factory_get_property (GObject *object, guint propid,
    GValue *value, GParamSpec *pspec)
{
  GstRTSPMediaFactory *factory = GST_RTSP_MEDIA_FACTORY (object);

  switch (propid) {
    case PROP_LAUNCH:
      g_value_take_string (value, gst_rtsp_media_factory_get_launch (factory));
      break;
    case PROP_SHARED:
      g_value_set_boolean (value, gst_rtsp_media_factory_is_shared (factory));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, propid, pspec);
  }
}

static void
gst_rtsp_media_factory_set_property (GObject *object, guint propid,
    const GValue *value, GParamSpec *pspec)
{
  GstRTSPMediaFactory *factory = GST_RTSP_MEDIA_FACTORY (object);

  switch (propid) {
    case PROP_LAUNCH:
      gst_rtsp_media_factory_set_launch (factory, g_value_get_string (value));
      break;
    case PROP_SHARED:
      gst_rtsp_media_factory_set_shared (factory, g_value_get_boolean (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, propid, pspec);
  }
}

/**
 * gst_rtsp_media_factory_new:
 *
 * Create a new #GstRTSPMediaFactory instance.
 *
 * Returns: a new #GstRTSPMediaFactory object.
 */
GstRTSPMediaFactory *
gst_rtsp_media_factory_new (void)
{
  GstRTSPMediaFactory *result;

  result = g_object_new (GST_TYPE_RTSP_MEDIA_FACTORY, NULL);

  return result;
}

/**
 * gst_rtsp_media_factory_set_launch:
 * @factory: a #GstRTSPMediaFactory
 * @launch: the launch description
 *
 *
 * The gst_parse_launch() line to use for constructing the pipeline in the
 * default prepare vmethod.
 *
 * The pipeline description should return a GstBin as the toplevel element
 * which can be accomplished by enclosing the dscription with brackets '('
 * ')'.
 *
 * The description should return a pipeline with payloaders named pay0, pay1,
 * etc.. Each of the payloaders will result in a stream.
 */
void
gst_rtsp_media_factory_set_launch (GstRTSPMediaFactory *factory, const gchar *launch)
{
  g_return_if_fail (GST_IS_RTSP_MEDIA_FACTORY (factory));
  g_return_if_fail (launch != NULL);

  g_mutex_lock (factory->lock);
  g_free (factory->launch);
  factory->launch = g_strdup (launch);
  g_mutex_unlock (factory->lock);
}

/**
 * gst_rtsp_media_factory_get_launch:
 * @factory: a #GstRTSPMediaFactory
 *
 * Get the gst_parse_launch() pipeline description that will be used in the
 * default prepare vmethod.
 *
 * Returns: the configured launch description. g_free() after usage.
 */
gchar *
gst_rtsp_media_factory_get_launch (GstRTSPMediaFactory *factory)
{
  gchar *result;

  g_return_val_if_fail (GST_IS_RTSP_MEDIA_FACTORY (factory), NULL);

  g_mutex_lock (factory->lock);
  result = g_strdup (factory->launch);
  g_mutex_unlock (factory->lock);

  return result;
}

/**
 * gst_rtsp_media_factory_set_shared:
 * @factory: a #GstRTSPMediaFactory
 * @shared: the new value
 *
 * Configure if media created from this factory can be shared between clients.
 */
void
gst_rtsp_media_factory_set_shared (GstRTSPMediaFactory *factory,
    gboolean shared)
{
  g_return_if_fail (GST_IS_RTSP_MEDIA_FACTORY (factory));

  g_mutex_lock (factory->lock);
  factory->shared = shared;
  g_mutex_unlock (factory->lock);
}

/**
 * gst_rtsp_media_factory_is_shared:
 * @factory: a #GstRTSPMediaFactory
 *
 * Get if media created from this factory can be shared between clients.
 *
 * Returns: %TRUE if the media will be shared between clients.
 */
gboolean
gst_rtsp_media_factory_is_shared (GstRTSPMediaFactory *factory)
{
  gboolean result;

  g_return_val_if_fail (GST_IS_RTSP_MEDIA_FACTORY (factory), FALSE);

  g_mutex_lock (factory->lock);
  result = factory->shared;
  g_mutex_unlock (factory->lock);

  return result;
}

static gboolean
compare_media (gpointer key, GstRTSPMedia *media1, GstRTSPMedia *media2)
{
  return (media1 == media2);
}

static void
media_unprepared (GstRTSPMedia *media, GstRTSPMediaFactory *factory)
{
  g_mutex_lock (factory->medias_lock);
  g_hash_table_foreach_remove (factory->medias, (GHRFunc) compare_media,
       media);
  g_mutex_unlock (factory->medias_lock);
}

/**
 * gst_rtsp_media_factory_construct:
 * @factory: a #GstRTSPMediaFactory
 * @url: the url used
 *
 * Prepare the media object and create its streams. Implementations
 * should create the needed gstreamer elements and add them to the result
 * object. No state changes should be performed on them yet.
 *
 * One or more GstRTSPMediaStream objects should be added to the result with
 * the srcpad member set to a source pad that produces buffer of type 
 * application/x-rtp.
 *
 * Returns: a new #GstRTSPMedia if the media could be prepared.
 */
GstRTSPMedia *
gst_rtsp_media_factory_construct (GstRTSPMediaFactory *factory, const GstRTSPUrl *url)
{
  gchar *key;
  GstRTSPMedia *media;
  GstRTSPMediaFactoryClass *klass;

  klass = GST_RTSP_MEDIA_FACTORY_GET_CLASS (factory);

  /* convert the url to a key for the hashtable. NULL return or a NULL function
   * will not cache anything for this factory. */
  if (klass->gen_key)
    key = klass->gen_key (factory, url);
  else
    key = NULL;

  g_mutex_lock (factory->medias_lock);
  if (key) {
    /* we have a key, see if we find a cached media */
    media = g_hash_table_lookup (factory->medias, key);
    if (media)
      g_object_ref (media);
  }
  else
    media = NULL;

  if (media == NULL) {
    /* nothing cached found, try to create one */
    if (klass->construct)
      media = klass->construct (factory, url);
    else
      media = NULL;

    if (media) {
      /* configure the media */
      if (klass->configure)
        klass->configure (factory, media);

      /* check if we can cache this media */
      if (gst_rtsp_media_is_shared (media)) {
        /* insert in the hashtable, takes ownership of the key */
        g_object_ref (media);
        g_hash_table_insert (factory->medias, key, media);
        key = NULL;
      }
      if (!gst_rtsp_media_is_reusable (media)) {
	/* when not reusable, connect to the unprepare signal to remove the item
	 * from our cache when it gets unprepared */
	g_signal_connect (media, "unprepared", (GCallback) media_unprepared,
			factory);
      }
    }
  }
  g_mutex_unlock (factory->medias_lock);

  if (key)
    g_free (key);

  g_message ("constructed media %p for url %s", media, url->abspath);

  return media;
}

static gchar *
default_gen_key (GstRTSPMediaFactory *factory, const GstRTSPUrl *url)
{
  gchar *result;

  result = gst_rtsp_url_get_request_uri ((GstRTSPUrl *)url);
//  result = g_strdup_printf("rtsp://%s%d%s", url->host,url->port,url->abspath);
  return result;
}

static GstElement *
default_get_element (GstRTSPMediaFactory *factory, const GstRTSPUrl *url)
{
  GstElement *element;
  GError *error = NULL;
  gchar *url_launch = g_strdup(url->query);

  g_mutex_lock (factory->lock);
  /* we need a parse syntax */
  if (factory->launch == NULL)
    goto no_launch;

  /* we need to get framerate and bitrate here*/
  /* url format sent from client like this rtsp://server:port/test?framerate=25/1&bitrate=2048 */
  /*( v4l2src ! videoscale ! videorate ! video/x-raw-yuv,width=800,height=500,framerate=30/1 ! ffmpegcolorspace ! 
  x264enc bitrate=2048 ! rtph264pay name=pay0 pt=96  alsasrc ! audio/x-raw-int ! faac bitrate=22000 ! 
  rtpmp4apay name=pay1 pt=97 )  */
  if (url_launch) {
    gchar *framerate = NULL;
    gchar *bitrate = NULL;
    gchar *width = NULL;
    gchar *height = NULL;    
    GHashTable *params = NULL;
	 guint param_number = 0;
  	 gchar ** tmp = NULL, **tmp_0 = NULL, ** tmp_1 = NULL;
  	 gchar ** tmp_2 = NULL, ** tmp_3 = NULL;
    gchar *tmp_launch = NULL;   	
    guint array_size =0;
    params = g_hash_table_new (g_str_hash, g_str_equal);         

	 if (strstr(url_launch, "&")) {
  	 	tmp = g_strsplit (url_launch, "&", -1);
  	 	array_size = sizeof (tmp)/sizeof (tmp[0]); 

		/* Check values before insert into hashtable */	 	
		if (tmp[0] != NULL) {	 	
	 		tmp_0 = g_strsplit (tmp[0], "=", 2);
		 	if (tmp_0[1] != NULL) {
				g_hash_table_insert (params, g_strdup(tmp_0[0]), g_strdup(tmp_0[1]));
			}
	 	}
	 	if (array_size > 1 && tmp[1] != NULL) {
	 		tmp_1 = g_strsplit (tmp[1], "=", 2);
			if (tmp_1[1] != NULL) {		
				g_hash_table_insert (params, g_strdup(tmp_1[0]), g_strdup(tmp_1[1]));
			}
	 	}
	 	if (array_size > 2 && tmp[2] != NULL) {	 
	 		tmp_2 = g_strsplit (tmp[2], "=", 2);
	      g_message ("tmp 2 %s",tmp_2[1]);
			if (tmp_2[1] != NULL) {		
				g_hash_table_insert (params, g_strdup(tmp_2[0]), g_strdup(tmp_2[1]));
			}
	 	}
	 	if (array_size > 3 && tmp[3] != NULL) {	
	 		tmp_3 = g_strsplit (tmp[3], "=", 2);
			if (tmp_3[1] != NULL) {		
				g_hash_table_insert (params, g_strdup(tmp_3[0]), g_strdup(tmp_3[1]));
			}
	 	}	
	   g_strfreev (tmp_0);
	   g_strfreev (tmp_1);
	   g_strfreev (tmp_2);
	   g_strfreev (tmp_3);
	 } else {
		tmp = g_strsplit (url_launch, "=", 2);
		g_hash_table_insert (params, g_strdup(tmp[0]), g_strdup(tmp[1]));
	 } 
    g_strfreev (tmp);
	 url_launch = g_strdup (factory->launch);
	 param_number = g_hash_table_size (params);
	 if (param_number == 0) 
	 	goto wrong_params; 
    framerate = g_hash_table_lookup (params, "framerate");
	 bitrate = g_hash_table_lookup (params, "bitrate");
	 width = g_hash_table_lookup (params, "width");
	 height = g_hash_table_lookup (params, "height");
	 g_message ("Parameters in url framerate=%s, bitrate=%s, width=%s, height=%s ", framerate, bitrate, width, height);
	 
	 if (param_number >= 2) {
	 	tmp = g_strsplit (url_launch, "framerate=", 2);
		if (width != NULL) {
			if (height == NULL) {
				height = "320";			
			}
			tmp_1 = g_strsplit (tmp[0], "width=", 2);		
		} 	 	
	 	tmp_0 = g_strsplit (tmp[1], "bitrate=", 2);
	 	g_message ("Temp launch 1 %s: ", tmp_0[0]);
	 	g_message ("Temp launch 2 %s: ", tmp_0[1]);
 		gchar * tmp0 = strstr (tmp_0[0], "!") ;
 		gchar * tmp1 = strstr (tmp_0[1], "!") ;  	 
	 	if (tmp0 && tmp1) {
	 		if (width != NULL) {
				tmp_launch = g_strdup_printf("%swidth=%s,height=%s,framerate=%s %s bitrate=%s %s", tmp_1[0], width, height, framerate, tmp0, bitrate, tmp1);	 		
	 		} else {
		   	tmp_launch = g_strdup_printf("%sframerate=%s %s bitrate=%s %s", tmp[0], framerate, tmp0, bitrate, tmp1);
	 		}
   	  	g_message ("New launch string %s: ", tmp_launch);
   	} else {
			tmp_launch = g_strdup_printf("%swidth=%s,height=%s,framerate=%s %s", tmp_1[0], width, height, framerate, tmp[1]);	 		
   	  	g_message ("New launch string %s: ", tmp_launch);
   	} 
   	g_strfreev (tmp_0);
   	g_strfreev (tmp_1);
    } else {
		if (framerate != NULL) {
			tmp = g_strsplit (url_launch, "framerate=", 2);
			if (tmp[1]) {
	      	gchar * tmp0 = strstr (tmp[1], "!") ;
				tmp_launch = g_strdup_printf("%sframerate=%s %s", tmp[0], framerate, tmp0);
			}		
		} else {
			tmp = g_strsplit (url_launch, "bitrate=", 2);		
			if (tmp[1]) {
	      	gchar * tmp0 = strstr (tmp[1], "!") ;
				tmp_launch = g_strdup_printf("%sbitrate=%s %s", tmp[0], bitrate, tmp0);
			}				
		}	     
   	g_strfreev (tmp);	
		g_message ("New launch string %s: ", tmp_launch);
	 }
	 if (tmp_launch != NULL) {	 
       g_free (factory->launch);
    	 factory->launch = g_strdup(tmp_launch);
 	 	 g_free (tmp_launch);
	 }
	 
wrong_params:
 	 g_free (url_launch);
	 g_hash_table_destroy (params);
  }	
 
  /* parse the user provided launch line */
  element = gst_parse_launch (factory->launch, &error);

  if (element == NULL)
    goto parse_error;

  g_mutex_unlock (factory->lock);

  if (error != NULL) {
    /* a recoverable error was encountered */
    g_warning ("recoverable parsing error: %s", error->message);
    g_error_free (error);
  }
  return element;

  /* ERRORS */
no_launch:
  {
    g_mutex_unlock (factory->lock);
    g_critical ("no launch line specified");
    return NULL;
  }
parse_error:
  {
    g_mutex_unlock (factory->lock);
    g_critical ("could not parse launch syntax (%s): %s", factory->launch, 
         (error ? error->message : "unknown reason"));
    if (error)
      g_error_free (error);
    return NULL;
  }
}

/* try to find all the payloader elements, they should be named 'pay%d'. for
 * each of the payloaders we will create a stream and collect the source pad. */
void
gst_rtsp_media_factory_collect_streams (GstRTSPMediaFactory *factory, const GstRTSPUrl *url,
    GstRTSPMedia *media)
{
  GstElement *element, *elem;
  GstPad * pad;
  gint i;
  GstRTSPMediaStream *stream;
  gboolean have_elem;

  element = media->element;

  have_elem = TRUE;
  for (i = 0; have_elem ; i++) {
    gchar *name;

    have_elem = FALSE;

    name = g_strdup_printf ("pay%d", i);
    if ((elem = gst_bin_get_by_name (GST_BIN (element), name))) {
      /* create the stream */
      stream = g_new0 (GstRTSPMediaStream, 1);
      stream->payloader = elem;

      g_message ("found stream %d with payloader %p", i, elem);

      pad = gst_element_get_static_pad (elem, "src");

      /* ghost the pad of the payloader to the element */
      stream->srcpad = gst_ghost_pad_new (name, pad);
      gst_element_add_pad (media->element, stream->srcpad);
      gst_object_unref (elem);

      /* add stream now */
      g_array_append_val (media->streams, stream);
      have_elem = TRUE;
    }
    g_free (name);

    name = g_strdup_printf ("dynpay%d", i);
    if ((elem = gst_bin_get_by_name (GST_BIN (element), name))) {
      /* a stream that will dynamically create pads to provide RTP packets */

      g_message ("found dynamic element %d, %p", i, elem);

      media->dynamic = g_list_prepend (media->dynamic, elem);

      have_elem = TRUE;
    }
    g_free (name);
  }
}

static GstRTSPMedia *
default_construct (GstRTSPMediaFactory *factory, const GstRTSPUrl *url)
{
  GstRTSPMedia *media;
  GstElement *element;
  GstRTSPMediaFactoryClass *klass;

  klass = GST_RTSP_MEDIA_FACTORY_GET_CLASS (factory);

  if (klass->get_element)
    element = klass->get_element (factory, url);
  else
    element = NULL;
  if (element == NULL)
    goto no_element;

  /* create a new empty media */
  media = gst_rtsp_media_new ();
  media->element = element;
  g_message ("Create MEDIA"); 
  if (!klass->create_pipeline)
    goto no_pipeline;

  media->pipeline = klass->create_pipeline (factory, media);

  gst_rtsp_media_factory_collect_streams (factory, url, media);

  return media;

  /* ERRORS */
no_element:
  {
    g_critical ("could not create element");
    return NULL;
  }
no_pipeline:
  {
    g_critical ("could not create pipeline");
    return FALSE;
  }
}

static GstElement*
default_create_pipeline (GstRTSPMediaFactory *factory, GstRTSPMedia *media) {
  GstElement *pipeline;

  pipeline = gst_pipeline_new ("media-pipeline");
  gst_bin_add (GST_BIN_CAST (pipeline), media->element);

  return pipeline;
}

static void
default_configure (GstRTSPMediaFactory *factory, GstRTSPMedia *media)
{
  gboolean shared;

  /* configure the sharedness */
  g_mutex_lock (factory->lock);
  shared = factory->shared;
  g_mutex_unlock (factory->lock);

  gst_rtsp_media_set_shared (media, shared);
}