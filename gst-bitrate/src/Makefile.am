# plugindir is set in configure

plugin_LTLIBRARIES = libgstbitrate.la

# sources used to compile this plug-in
libgstbitrate_la_SOURCES = gstbitrate.c

# flags used to compile this plugin
# add other _CFLAGS and _LIBS as needed
libgstbitrate_la_CFLAGS = $(GST_CFLAGS)
libgstbitrate_la_LIBADD = $(GST_LIBS) $(GST_BASE_LIBS) $(GSTCTRL_LIBS)
libgstbitrate_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstbitrate_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstbitrate.h
