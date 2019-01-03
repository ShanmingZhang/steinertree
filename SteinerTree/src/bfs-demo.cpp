
#include <stdlib.h>
#include <stdio.h>
#include "bool.h"
#include "graph.h"


void process_vertex_early(int v)
{
	printf("processed vertex %d\n",v);
}

void process_vertex_late(int v)
{
}


void process_edge(int x, int y)
{
        printf("processed edge (%d,%d)\n",x,y);
}


/*
int main()
{
	graph g;
	int i;

	read_graph(&g,FALSE);
	print_graph(&g);
	initialize_search(&g);
	bfs(&g,1);
	for (i=1; i<=g.nvertices; i++)
		printf(" %d",parent[i]);
	printf("\n");

	for (i=1; i<=g.nvertices; i++) 
		find_path(1,i,parent);
	printf("\n");
	return 0;
}
*/

