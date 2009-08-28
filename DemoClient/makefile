#simple makefile for client demo
CC=gcc
CFLAGS=`pkg-config --cflags gstreamer-0.10 gtk+-2.0` -lgstinterfaces-0.10
LDFLAGS=`pkg-config --libs gstreamer-0.10 gtk+-2.0` -lgstinterfaces-0.10
OBJS= main.o democlient-interface.o democlient-callbacks.o democlient-support.o democlient-backend.o
TARGET= DemoClient

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c democlient-interface.h democlient-interface.c democlient-support.h democlient-support.c democlient-callbacks.h democlient-callbacks.c democlient-backend.c democlient-backend.h

democlient-interface.o: democlient-interface.c democlient-interface.h democlient-support.h democlient-support.c democlient-callbacks.h democlient-callbacks.c democlient-backend.c democlient-backend.h

democlient-callbacks.o: democlient-callbacks.c democlient-callbacks.h democlient-interface.h democlient-interface.c democlient-support.h democlient-support.c democlient-backend.c democlient-backend.h

democlient-support.o: democlient-support.c democlient-support.h

democlient-backend.o: democlient-backend.c democlient-backend.h

clean:
	rm -rf $(OBJS) $(TARGET)
