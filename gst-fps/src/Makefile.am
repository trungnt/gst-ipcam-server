# plugindir is set in configure

plugin_LTLIBRARIES = libfpsbinplugin.la

# sources used to compile this plug-in
libfpsbinplugin_la_SOURCES = gstfpsbin.c

# flags used to compile this plugin
# add other _CFLAGS and _LIBS as needed
libfpsbinplugin_la_CFLAGS = $(GST_CFLAGS)
libfpsbinplugin_la_LIBADD = $(GST_LIBS) $(GST_BASE_LIBS) $(GSTCTRL_LIBS)
libfpsbinplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libfpsbinplugin_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstfpsbin.h
