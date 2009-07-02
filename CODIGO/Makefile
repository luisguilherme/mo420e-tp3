CXX = c++
INCLUDES = -I$(XPRESSDIR)/include -Iugraph/grafo/ugraph/include -Iugraph/grafo/myutil/include
LIBS = -L$(XPRESSDIR)/lib -Lugraph/grafo/ugraph -Lugraph/grafo/myutil
CFLAGS += -Wall -g -D_REENTRANT $(INCLUDES)
LDFLAGS = $(LIBS) -lxprs -lm -lugraph -lmyutil

headers = instance.H integer_program.H definitions.H
objects = stab_instance.o bnc.o
sources = $(objects:.o=.cc)

.PHONY: all
all: $(sources) $(headers) $(objects)

.cc.o:
	$(CXX) $(CFLAGS) $< -c -o $@

bnc: $(objects) $(headers)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(objects) -o $@

prebnc: $(objects) $(headers)
	$(CXX) $(CFLAGS) $(objects) $(LIBS) -lugraph -lmyutil -o $@

.PHONY: clean
clean: 
	rm -Rf $(objects) bnc prebnc


