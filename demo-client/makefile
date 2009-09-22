#simple makefile for client demo
CC=gcc
CFLAGS=`pkg-config --cflags gstreamer-0.10 gtk+-2.0` -lgstinterfaces-0.10
LDFLAGS=`pkg-config --libs gstreamer-0.10 gtk+-2.0` -lgstinterfaces-0.10
OBJS= main.o demo-client-interface.o demo-client-callbacks.o demo-client-support.o demo-client-backend.o
TARGET= demo-client

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c demo-client-interface.h demo-client-interface.c demo-client-support.h demo-client-support.c demo-client-callbacks.h demo-client-callbacks.c demo-client-backend.c demo-client-backend.h

demo-client-interface.o: demo-client-interface.c demo-client-interface.h demo-client-support.h demo-client-support.c demo-client-callbacks.h demo-client-callbacks.c demo-client-backend.c demo-client-backend.h

demo-client-callbacks.o: demo-client-callbacks.c demo-client-callbacks.h demo-client-interface.h demo-client-interface.c demo-client-support.h demo-client-support.c demo-client-backend.c demo-client-backend.h

demo-client-support.o: demo-client-support.c demo-client-support.h

demo-client-backend.o: demo-client-backend.c demo-client-backend.h

clean:
	rm -rf $(OBJS) $(TARGET)
