CXX = c++
INCLUDES = -I$(XPRESSDIR)/include -Iugraph/grafo/ugraph/include -Iugraph/grafo/myutil/include
LIBS = -L$(XPRESSDIR)/lib -Lugraph/grafo/ugraph -Lugraph/grafo/myutil
CFLAGS += -Wall -g -D_REENTRANT $(INCLUDES)
LDFLAGS = $(LIBS) -lxprs -lm -lugraph -lmyutil

headers = instance.H integer_program.H definitions.H cutting.H stab.H
objects = stab_instance.o bnc.o stab.o cutting.o
sources = $(objects:.o=.cc)

.PHONY: all
all: bnc

.cc.o:
	$(CXX) $(CFLAGS) $< -c -o $@

bnc: $(sources) $(objects) $(headers) ugraph
	$(CXX) $(CFLAGS) $(objects) -o $@ $(LDFLAGS)

prebnc: $(objects) $(headers)
	$(CXX) $(CFLAGS) $(objects) $(LIBS) -o $@ -lugraph -lmyutil

ugraph:
	(cd ugraph ; ./build)

.PHONY: clean
clean: 
	rm -Rf $(objects) bnc prebnc
	(cd ugraph ; ./clean)
