CXX = c++
INCLUDES = -I$(XPRESSDIR)/include -Iugraph/grafo/ugraph/include
LIBS = -L$(XPRESSDIR)/lib -Lugraph/grafo/ugraph/
CFLAGS += -Wall -g -D_REENTRANT $(INCLUDES)
LDFLAGS = $(LIBS) -lxprs -lm  

headers = instance.H integer_program.H definitions.H
objects = 
sources = $(objects:.o=.cc)

.PHONY: all
all: $(sources) $(headers) $(objects)

.cc.o:
	$(CXX) $(CFLAGS) $< -c -o $@

milsp: $(objects) $(headers)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(objects) -o $@

.PHONY: clean
clean: 
	rm -Rf $(objects) milsp


