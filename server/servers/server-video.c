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


static gboolean
timeout (GstRTSPServer *server, gboolean ignored)
{
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool (server);
  gst_rtsp_session_pool_cleanup (pool);
  g_object_unref (pool);

  return TRUE;
}

int
main (int argc, char *argv[])
{
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMediaMapping *mapping;
  GstRTSPMediaFactory *factorymp4, *factoryjpg, *factoryh264;

  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);

  /* create a server instance */
  server = gst_rtsp_server_new ();

  /* get the mapping for this server, every server has a default mapper object
   * that be used to map uri mount points to media factories */
  mapping = gst_rtsp_server_get_media_mapping (server);

  /*----------------Mpeg 4 video only stream server start----------------------------*/
  /* make a media factory for a mpeg 4 video stream. The default media factory can use
   * gst-launch syntax to create pipelines. 
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factorymp4 = gst_rtsp_media_factory_new ();

  gst_rtsp_media_factory_set_launch (factorymp4, "( "
    "v4l2src ! video/x-raw-yuv,width=352,height=288,framerate=25/1 ! ffmpegcolorspace ! "
    "ffenc_mpeg4 ! rtpmp4vpay name=pay0 pt=96 "
    ")");

  /* share the pipeline with multiple clients */
  gst_rtsp_media_factory_set_shared(factorymp4, TRUE);

  /* attach the test factory to the /mp4 url */
  gst_rtsp_media_mapping_add_factory (mapping, "/mp4", factorymp4);
  /*----------------Mpeg 4 video only stream server end------------------------------*/

  /*----------------Jpeg only stream server start------------------------------------*/
  /* make a media factory for a jpeg stream. The default media factory can use
   * gst-launch syntax to create pipelines. 
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factoryjpg = gst_rtsp_media_factory_new ();

  gst_rtsp_media_factory_set_launch (factoryjpg, "( "
    "v4l2src ! video/x-raw-yuv,width=352,height=288,framerate=25/1 ! "
    "jpegenc ! rtpjpegpay name=pay0 pt=96 "
    ")");

  /* share the pipeline with multiple clients */
  gst_rtsp_media_factory_set_shared(factoryjpg, TRUE);

  /* attach the test factory to the /jpg url */
  gst_rtsp_media_mapping_add_factory (mapping, "/jpg", factoryjpg);
  /*----------------Jpeg only stream server end--------------------------------------*/

  /*----------------Jpeg h264 video only stream server start-------------------------*/
  /* make a media factory for a h264 video stream. The default media factory can use
   * gst-launch syntax to create pipelines. 
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factoryh264 = gst_rtsp_media_factory_new ();

  gst_rtsp_media_factory_set_launch (factoryh264, "( "
    "v4l2src ! video/x-raw-yuv,width=352,height=288,framerate=25/1 ! "
    "ffenc_libx264 ! rtph264pay name=pay0 pt=96 "
    ")");

  /* share the pipeline with multiple clients */
  gst_rtsp_media_factory_set_shared(factoryh264, TRUE);

  /* attach the test factory to the /h264 url */
  gst_rtsp_media_mapping_add_factory (mapping, "/h264", factoryh264);
  /*----------------Jpeg h264 video only stream server end---------------------------*/

  /* don't need the ref to the mapper anymore */
  g_object_unref (mapping);

  /* attach the server to the default maincontext */
  gst_rtsp_server_attach (server, NULL);

  g_timeout_add_seconds (2, (GSourceFunc) timeout, server); 

  /* start serving */
  g_main_loop_run (loop);

  return 0;
}
