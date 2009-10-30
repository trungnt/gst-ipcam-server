# plugindir is set in configure

plugin_LTLIBRARIES = libgstfpsbinplugin.la

# sources used to compile this plug-in
libgstfpsbinplugin_la_SOURCES = gstfpsbin.c

# flags used to compile this plugin
# add other _CFLAGS and _LIBS as needed
libgstfpsbinplugin_la_CFLAGS = $(GST_CFLAGS)
libgstfpsbinplugin_la_LIBADD = $(GST_LIBS) $(GST_BASE_LIBS) $(GSTCTRL_LIBS)
libgstfpsbinplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstfpsbinplugin_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstfpsbin.h
