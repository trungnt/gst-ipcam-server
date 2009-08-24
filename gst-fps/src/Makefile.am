# plugindir is set in configure

plugin_LTLIBRARIES = libvideofpsplugin.la

# sources used to compile this plug-in
libvideofpsplugin_la_SOURCES = gstvideofps.c

# flags used to compile this plugin
# add other _CFLAGS and _LIBS as needed
libvideofpsplugin_la_CFLAGS = $(GST_CFLAGS)
libvideofpsplugin_la_LIBADD = $(GST_LIBS) $(GST_BASE_LIBS) $(GSTCTRL_LIBS)
libvideofpsplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libvideofpsplugin_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstvideofps.h
