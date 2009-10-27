noinst_PROGRAMS = onesrc-h264 onesrc-jpeg onesrc-mpeg4 onesrc-jpeg-h264 onesrc-jpeg-mpeg4 onesrc-h264-mpeg4 twosrc-jpeg-h264 twosrc-jpeg-mpeg4 twosrc-h264-mpeg4
onesrc_jpeg_SOURCES = onesrc-jpeg.c
onesrc_h264_SOURCES = onesrc-h264.c
onesrc_mpeg4_SOURCES = onesrc-mpeg4.c
onesrc_jpeg_h264_SOURCES = onesrc-jpeg-h264.c
onesrc_jpeg_mpeg4_SOURCES = onesrc-jpeg-mpeg4.c
onesrc_h264_mpeg4_SOURCES = onesrc-h264-mpeg4.c
twosrc_jpeg_h264_SOURCES = twosrc-jpeg-h264.c
twosrc_jpeg_mpeg4_SOURCES = twosrc-jpeg-mpeg4.c
twosrc_h264_mpeg4_SOURCES = twosrc-h264-mpeg4.c

INCLUDES = -I$(top_srcdir) -I$(srcdir) -I$(top_srcdir)/profile

AM_CFLAGS = $(GST_PLUGINS_BASE_CFLAGS) $(GST_CFLAGS)
AM_LDFLAGS = \
	$(GST_LIBS) \
	$(top_builddir)/gst/rtsp-server/libgstrtspserver-@GST_MAJORMINOR@.la


