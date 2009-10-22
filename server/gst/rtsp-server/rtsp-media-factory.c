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
#include "profile/pipeline-profile-ext.h"


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

/**
 * parse url query to extract variables
 *
 * @param url_query gchar* query to extract
 *
 * @return GHashTable* table of variable and value
 */
static GHashTable * gst_rtsp_media_factory_query_parsing(const gchar * url_query);

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
  factory->server_config = NULL;
  factory->shared = DEFAULT_SHARED;

  factory->lock = g_mutex_new ();
  factory->medias_lock = g_mutex_new ();
  factory->medias = g_hash_table_new_full (g_str_hash, g_str_equal,
		  g_free, g_object_unref);
  factory->v4l2src_port = 0;		  
  factory->two_streams = FALSE;
}

static void
gst_rtsp_media_factory_finalize (GObject * obj)
{
  GstRTSPMediaFactory *factory = GST_RTSP_MEDIA_FACTORY (obj);

  g_hash_table_unref (factory->medias);
  g_mutex_free (factory->medias_lock);
  g_free (factory->launch);
  if (factory->server_config != NULL) {
    gst_rtsp_server_configuration_free(factory->server_config);
  }
  g_mutex_free (factory->lock);

  gst_element_set_state (factory->v4l2src_pipeline, GST_STATE_NULL);
  g_object_unref (factory->v4l2src_pipeline);
  g_object_unref (factory->multiudpsink);
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
  /*if (factory->launch == NULL)*/
  if (factory->server_config == NULL)
    goto no_launch;
  /* we need to get framerate and bitrate here*/
  /* url format sent from client like this rtsp://server:port/test?framerate=25/1&bitrate=2048 */
  /*(  videoscale ! videorate ! video/x-raw-yuv,width=800,height=500,framerate=30/1 ! ffmpegcolorspace ! 
  x264enc bitrate=2048 ! rtph264pay name=pay0 pt=96  alsasrc ! audio/x-raw-int ! faac bitrate=22000 ! 
  rtpmp4apay name=pay1 pt=97 )  */
  if (url_launch) {
    gchar *framerate = NULL;
    gchar *bitrate = NULL;
    gchar *width = NULL;
    gchar *height = NULL;    
    GHashTable *params = NULL;
	 guint param_number = 0;

	/* now parse query */
	params = gst_rtsp_media_factory_query_parsing(url_launch);
	 /*url_launch = g_strdup (factory->launch);*/
	 param_number = g_hash_table_size (params);
	 if (param_number == 0) 
	 	goto wrong_params; 
    framerate = g_hash_table_lookup (params, "framerate");
	 bitrate = g_hash_table_lookup (params, "bitrate");
	 width = g_hash_table_lookup (params, "width");
	 height = g_hash_table_lookup (params, "height");
	 g_message ("Parameters in url framerate=%s, bitrate=%s, width=%s, height=%s ", framerate, bitrate, width, height);

	 /* now, we set variable to our pipeline */
	 {
		 /* get the first video pipeline => this tmp here*/
		 GstRTSPPipelineProfile * profile = NULL;
		 profile = gst_rtsp_server_configuration_get_default_video_pipeline(factory->server_config);

		 if (profile == NULL)
			 goto no_launch;

		 if (framerate != NULL) {
			 gst_rtsp_pipeline_profile_video_set_framerate(profile, framerate);
		 }

		 if (width != NULL) {
			 gst_rtsp_pipeline_profile_video_set_width(profile, width);
		 }

		 if (height != NULL) {
			 gst_rtsp_pipeline_profile_video_set_height(profile, height);
		 }

		 if (bitrate != NULL) {
			 gst_rtsp_pipeline_profile_video_set_bitrate(profile, bitrate);
			 factory->bitrate = g_strdup(bitrate);
		 }
	 }
	 
wrong_params:
 	 g_free (url_launch);
	 g_hash_table_destroy (params);
  }	
  /* we add new client here */

  /**
   * NOTE: we can easily set port using our pipeline profile
   * gst_rtsp_pipeline_profile_set_var(profile, "port", factory->v4l2src_port);
   * then use gst_rtsp_pipeline_profile_build_pipeline(profile) to get launch string.
   * But temporary keep it here. May be I'll update after merge with server-development branch
   */
  factory->launch = gst_rtsp_server_configuration_build_pipeline(factory->server_config);
  gchar *tmp, *tmp1=NULL, **tmp_0=NULL;
  /* in case one source 2 streams */
  if (factory->two_streams) {
    factory->v4l2src_port += 2;
  } else {   
    factory->v4l2src_port += 1;
  }  
  g_signal_emit_by_name (factory->multiudpsink, "add", "127.0.0.1",factory->v4l2src_port, NULL);
  tmp =  g_strdup (factory->launch);
  if (strstr(tmp, "(")) {
    if (strstr(tmp, "udpsrc")) {
	   tmp_0 = g_strsplit (tmp, "!", 2);
	 } else {
	   tmp_0 = g_strsplit (tmp, "(", 2);		
	 }    
  }
  tmp1 = g_strdup_printf("( udpsrc port=%d ! %s", factory->v4l2src_port ,tmp_0[1]);
  g_free (tmp);
  g_free (factory->launch);
  factory->launch = g_strdup(tmp1) ;
  g_free (tmp1); 
  g_strfreev (tmp_0);  
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
    g_critical ("no server configuration specified");
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
/*  
  GstElement *pipeline,  *udpsrc;
  gint ret;

  if ((media->client_sock.fd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    g_message ("No udp client socket");
  ret = 1;
  if (setsockopt (media->client_sock.fd, SOL_SOCKET, SO_KEEPALIVE,
          (void *) &ret, sizeof (ret)) < 0)
    g_message ("Failed to create udp client socket");

  memset (&media->client_sin, 0, sizeof (media->client_sin));
  media->client_sin.sin_family = AF_INET;        
  media->client_sin.sin_port = htons (3000);     
  media->client_sin.sin_addr.s_addr = htonl (INADDR_ANY);       
  
  ret = connect(media->client_sock.fd, (struct sockaddr *)&media->client_sin, sizeof (media->client_sin));

  if (ret <0)
  	 g_message ("Can not connect to server");
  udpsrc = gst_element_make_from_uri (GST_URI_SRC, "udp://0.0.0.0", NULL);

  g_object_set (G_OBJECT (udpsrc), "sockfd", media->client_sock.fd, NULL);
  g_object_set (G_OBJECT (udpsrc), "closefd", FALSE, NULL);
  
  pipeline = gst_pipeline_new ("client-pipeline");
  gst_bin_add_many (GST_BIN(pipeline), udpsrc, element, NULL);
  
  
  gst_element_link_many (pipeline, udpsrc, element, NULL);
  media->element = pipeline;
*/
  g_message ("Create MEDIA");
   
  if (!klass->create_pipeline)
    goto no_pipeline;

  media->pipeline = klass->create_pipeline (factory, media);
  media->bitrate = g_strdup(factory -> bitrate) ;
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

/**
 * gst_rtsp_factory_set_device_source:
 * @server: a #GstRTSPServer
 * @v4l2src: v4l2src
 *
 * Configure @factory to use which v4l2 device.
 * @v4l2dev v4l2src device=/dev/video0.
 * @ prop device property, default is /dev/video0
 * @port port to listen
 *
 * This function must be called when using 2 webcamera sources.
 */
void  
gst_rtsp_factory_set_device_source (GstRTSPMediaFactory *factory, gchar *v4l2dev, gchar* prop, gint port)
{
  g_return_if_fail (GST_IS_RTSP_MEDIA_FACTORY (factory));
  g_return_if_fail (v4l2dev);	
 
  /* when do this I will create one server to v4l2src :P */
  GstCaps *caps;
  GstElement *pipeline,  *v4l2src, *ffmpegcolorspace, *jpegenc, *multiudpsink;
  /* setup pipeline */
  pipeline = gst_pipeline_new ("v4l2src-pipeline");
  if (v4l2dev) {
  	 v4l2src = gst_element_factory_make (g_strdup(v4l2dev), "v4l2src");
  } else {
	 v4l2src = gst_element_factory_make ("v4l2src", v4l2dev); 
  }   	 
  if (prop)	  
  	 g_object_set (G_OBJECT (v4l2src), "device", prop, NULL);
  else 
  	 g_object_set (G_OBJECT (v4l2src), "device", "/dev/video0", NULL); 	 
  
  ffmpegcolorspace = gst_element_factory_make ("ffmpegcolorspace", "ffmpegcolorspace");
  jpegenc = gst_element_factory_make ("jpegenc", "jpegenc");
  multiudpsink = gst_element_factory_make ("multiudpsink", "multiudpsink");
  g_object_set (G_OBJECT (multiudpsink), "closefd", FALSE, NULL);
  g_object_set (G_OBJECT (multiudpsink), "sync", FALSE, NULL);
  g_object_set (G_OBJECT (multiudpsink), "async", FALSE, NULL); 
  gst_bin_add_many (GST_BIN(pipeline), v4l2src, ffmpegcolorspace, jpegenc, multiudpsink, NULL);
  caps = gst_caps_new_simple ("video/x-raw-yuv",
      "framerate", GST_TYPE_FRACTION, 20, 1,
      "width", G_TYPE_INT, 640,
      "height", G_TYPE_INT, 480,
      NULL);
  gst_element_link_filtered (v4l2src, ffmpegcolorspace, caps);  
  /* link all */
  gst_element_link_many (ffmpegcolorspace, jpegenc, multiudpsink, NULL);
  gst_element_set_state (pipeline, GST_STATE_PLAYING);	
  factory->v4l2src_pipeline = pipeline ;  
  factory->v4l2src_port = port ;
  factory->multiudpsink = multiudpsink;
}

void
gst_rtsp_media_factory_set_server_configuration(GstRTSPMediaFactory* factory, GstRTSPServerConfiguration* server_config) {
	/* if this's NULL then nothing to do */
	g_return_if_fail(server_config != NULL);

	if (factory->server_config != NULL) {
		/* free the old one */
		gst_rtsp_server_configuration_free(factory->server_config);
	}

	factory->server_config = server_config;
   if (factory->bitrate == NULL)	{  
	 GstRTSPPipelineProfile * profile = NULL;
	 profile = gst_rtsp_server_configuration_get_default_video_pipeline(factory->server_config);
	 if (profile != NULL)
  	 	factory->bitrate = gst_rtsp_pipeline_profile_get_var(profile, "video-bitrate");
  }	 

}

GstRTSPServerConfiguration *
gst_rtsp_media_factory_get_server_configuration(GstRTSPMediaFactory* factory) {
	return factory->server_config;
}

static
GHashTable * gst_rtsp_media_factory_query_parsing(const gchar * url_query) {
	gchar ** parts = g_strsplit(url_query, "&", -1);
	gint no_parts = g_strv_length(parts);
	gint i;
	GHashTable * params;

	if (no_parts == 0) {
		return NULL;
	}

	params = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
	g_return_val_if_fail(params != NULL, NULL);

	/* now, check and insert variables to hash table */
	for ( i=0 ; i<no_parts ; i++ ) {
		gchar ** tmp = g_strsplit(parts[i], "=", -1);
		if (g_strv_length(tmp) != 2) {
			/* wrong param format */
			g_strfreev(tmp);
			continue;
		}
		g_hash_table_insert(params, tmp[0], tmp[1]);
	}

	g_strfreev(parts);
	return params;
}
