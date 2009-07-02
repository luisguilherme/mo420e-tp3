CC = gcc

includedir       = ./grafo/ugraph/include
myutilincludedir = ./grafo/myutil/include
INCLUDES         = -I$(includedir) -I$(myutilincludedir)

ex-ugraph: ./grafo/ugraph/libugraph.a  ./grafo/myutil/libmyutil.a
	$(CC) -o gh gh.c ./grafo/ugraph/libugraph.a ./grafo/myutil/libmyutil.a $(INCLUDES) -lm