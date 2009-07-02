#ifndef  GRAPH_CIRCUIT
#define  GRAPH_CIRCUIT

#define GR_DFS_CIRCUIT_BRANCO 0
#define GR_DFS_CIRCUIT_CINZA 1
#define GR_DFS_CIRCUIT_PRETO 2
boolean gr_get_circuit(graphtype *g,int *nseq,int *seq,boolean *edgesetmask);
#endif
