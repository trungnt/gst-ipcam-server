# plugindir is set in configure

plugin_LTLIBRARIES = libgstbitrateplugin.la

# sources used to compile this plug-in
libgstbitrateplugin_la_SOURCES = gstbitrate.c

# flags used to compile this plugin
# add other _CFLAGS and _LIBS as needed
libgstbitrateplugin_la_CFLAGS = $(GST_CFLAGS)
libgstbitrateplugin_la_LIBADD = $(GST_LIBS) $(GST_BASE_LIBS) $(GSTCTRL_LIBS)
libgstbitrateplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstbitrateplugin_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstbitrate.h
