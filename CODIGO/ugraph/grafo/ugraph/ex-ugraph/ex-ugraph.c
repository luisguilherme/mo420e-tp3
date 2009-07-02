#include <stdio.h>
#include "ugraph.h"

int main(int argc,char **argv)
{
  graphtype g,copia, gh;
  
  printf("\nSintaxe: %s %s\n",argv[0], argv[1]);
  printf("\n------ GRAFO DE ENTRADA -------\n\n");
  
  if (argc==2) 
    {
      /* le arquivo inicial */
      if (!gr_readgraph(&g,argv[1]))
	{
	  printf("Erro na leitura do arquivo %s\n\n",argv[1]);
	  return(1);
	}
      printf("Grafo com %d vertices e %d arestas\n",gr_number_vertices(&g), gr_number_edges(&g));
    
      /* imprime arquivo de entrada */
      gr_printgraph(&g);

      /* ----- gera arvore de Gomory-Hu ----- */

      /* re-rotula vertices para rotulos continuos */
      gr_cleangraph(&copia,&g); 

      /* gera arvore */
      gr_generate_ghc_tree(&copia,&gh);
 
      /* imprime arvore de gomory-hu */
      printf("\n------ ARVORE DE GOMORY-HU -------\n\n");
      gr_printgraph(&gh);
      
      /* libera memoria */
      gr_closegraph(&gh);
      gr_closegraph(&copia);
      gr_closegraph(&g);

      return(0);
    }  

}

