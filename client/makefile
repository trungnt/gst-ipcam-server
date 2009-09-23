#simple makefile for client demo
CC=gcc
CFLAGS=`pkg-config --cflags gstreamer-0.10 gtk+-2.0` -lgstinterfaces-0.10
LDFLAGS=`pkg-config --libs gstreamer-0.10 gtk+-2.0` -lgstinterfaces-0.10
OBJS= main.o gst-ipcam-client-interface.o gst-ipcam-client-callbacks.o gst-ipcam-client-support.o gst-ipcam-client-backend.o
TARGET= gst-ipcam-client

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c gst-ipcam-client-interface.h gst-ipcam-client-interface.c gst-ipcam-client-support.h gst-ipcam-client-support.c gst-ipcam-client-callbacks.h gst-ipcam-client-callbacks.c gst-ipcam-client-backend.c gst-ipcam-client-backend.h

gst-ipcam-client-interface.o: gst-ipcam-client-interface.c gst-ipcam-client-interface.h gst-ipcam-client-support.h gst-ipcam-client-support.c gst-ipcam-client-callbacks.h gst-ipcam-client-callbacks.c gst-ipcam-client-backend.c gst-ipcam-client-backend.h

gst-ipcam-client-callbacks.o: gst-ipcam-client-callbacks.c gst-ipcam-client-callbacks.h gst-ipcam-client-interface.h gst-ipcam-client-interface.c gst-ipcam-client-support.h gst-ipcam-client-support.c gst-ipcam-client-backend.c gst-ipcam-client-backend.h

gst-ipcam-client-support.o: gst-ipcam-client-support.c gst-ipcam-client-support.h

gst-ipcam-client-backend.o: gst-ipcam-client-backend.c gst-ipcam-client-backend.h

clean:
	rm -rf $(OBJS) $(TARGET)
