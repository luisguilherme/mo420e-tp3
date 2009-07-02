CC = gcc
CFLAGS = -Wall -O3 -g
LDFLAGS = 

prefix = .
includedir = ${prefix}/include
sourcedir = ${prefix}/source

SOURCES=source/color.c source/mymemory.c source/mydate.c source/mydouble.c source/tempo.c source/unionfin.c source/logfile.c

OBJS=source/color.o source/mymemory.o source/mydate.o source/mydouble.o source/tempo.o source/unionfin.o  source/logfile.o

INCLUDES = -I$(includedir)

.SUFFIXES:
.SUFFIXES: .c .o

all: libmyutil.a tempo-ex

libmyutil.a: $(OBJS)
	$(AR) cru libmyutil.a $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) libmyutil.a tempo-ex *~ core

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

tempo-ex: libmyutil.a tempo-ex.c
	$(CC) -o tempo-ex tempo-ex.c libmyutil.a $(CFLAGS) $(INCLUDES) 