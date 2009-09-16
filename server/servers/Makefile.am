noinst_PROGRAMS = onesrc-h264 onesrc-jpeg onesrc-mpeg4
onesrc_jpeg_SOURCES = server-profile.c onesrc-jpeg.c
onesrc_h264_SOURCES = server-profile.c onesrc-h264.c
onesrc_mpeg4_SOURCES = server-profile.c onesrc-mpeg4.c

INCLUDES = -I$(top_srcdir) -I$(srcdir) -I$(top_srcdir)/profile

AM_CFLAGS = $(GST_PLUGINS_BASE_CFLAGS) $(GST_CFLAGS)
AM_LDFLAGS = \
	$(GST_LIBS) \
	$(top_builddir)/gst/rtsp-server/libgstrtspserver-@GST_MAJORMINOR@.la

