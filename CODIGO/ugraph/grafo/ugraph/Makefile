CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 

prefix = .
ugraphdir = $(prefix)
includedir = $(ugraphdir)/include
myutildir = $(ugraphdir)/../myutil
myutilincludedir = $(myutildir)/include
sourcedir = $(ugraphdir)/source

SOURCES=source/ugraph.c source/ghc_tree.c source/maxflow.c source/mincut.c source/allocrus.c source/circuit.c

OBJS=source/ugraph.o source/ghc_tree.o source/maxflow.o source/mincut.o source/allocrus.o source/circuit.o

INCLUDES = -I$(includedir) -I$(myutilincludedir)

.SUFFIXES:
.SUFFIXES: .c .o

all: libmyutil.a libugraph.a ex-ugraph manual

libmyutil.a:
	cd $(myutildir);make -f Makefile 

libugraph.a: $(OBJS)
	$(AR) cru $(ugraphdir)/libugraph.a $(OBJS) $(LDFLAGS) 

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

ex-ugraph: libugraph.a  libmyutil.a
	$(CC) -o $(ugraphdir)/ex-ugraph/ex-ugraph \
	$(ugraphdir)/ex-ugraph/ex-ugraph.c \
	$(ugraphdir)/libugraph.a \
	$(myutildir)/libmyutil.a \
	$(INCLUDES) -lm


manual: 
	cd doc;makeindex manual.tex;latex manual.tex;makeindex \
	manual;latex manual.tex; dvipdf manual

clean:
	rm -f $(OBJS) libugraph.a \
	ex-ugraph/ex-ugraph \
	doc/manual.pdf doc/manual.aux doc/manual.dvi doc/manual.idx \
	doc/manual.ind doc/manual.log doc/manual.toc doc/manual.ilg \
	*~ core
