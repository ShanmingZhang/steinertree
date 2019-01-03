#include <stdlib.h>
#include <stdio.h>

#include <limits.h>
#include "bool.h"
#include "graph.h"
#include "bfs-dfs.h"

//int parent[MAXV + 1]; /* discovery relation */
int path_node[MAXV + 1];

void dijkstra(graph *g, int start) /* WAS prim(g,start) */
{
	int i; /* counter */
	edgenode *p; /* temporary pointer */
	boolean intree[MAXV + 1]; /* is the vertex in the tree yet? */
	int distance[MAXV + 1]; /* distance vertex is from start */
	int v; /* current vertex to process */
	int w; /* candidate next vertex */
	int weight; /* edge weight */
	int dist; /* best current distance from start */

	for (i = 1; i <= g->nvertices; i++) {
		intree[i] = FALSE;
		distance[i] = INT_MAX;
	}

	for (i = 1; i <= MAXV + 1; i++) {
		parent[i] = -1;
		path_node[i] = -1;
	}

	distance[start] = 0;
	v = start;

	while (intree[v] == FALSE) {
		intree[v] = TRUE;
		p = g->edges[v];
		while (p != NULL) {
			w = p->y;
			weight = p->weight;
			/* CHANGED */
			if (distance[w] > (distance[v] + weight)) {
				/* CHANGED */distance[w] = distance[v] + weight;
				/* CHANGED */parent[w] = v;
			}
			p = p->next;
		}

		v = 1;
		dist = INT_MAX;
		for (i = 1; i <= g->nvertices; i++)
			if ((intree[i] == FALSE) && (dist > distance[i])) {
				dist = distance[i];
				v = i;
			}
	}
	/*for (i=1; i<=g->nvertices; i++) printf("%d %d\n",i,distance[i]);**/

}

double get_dijkstra(graph *g, int start, int end) /* WAS prim(g,start) */
{
	int i; /* counter */
	edgenode *p; /* temporary pointer */
	boolean intree[MAXV + 1]; /* is the vertex in the tree yet? */
	double distance[MAXV + 1]; /* distance vertex is from start */
	int v; /* current vertex to process */
	int w; /* candidate next vertex */
	int weight; /* edge weight */
	int dist; /* best current distance from start */

	for (i = 1; i <= g->nvertices; i++) {
		intree[i] = FALSE;
		distance[i] = INT_MAX;
	}


	for (i = 1; i <= MAXV + 1; i++) {
		parent[i] = -1;
		path_node[i] = -1;
	}

	distance[start] = 0;
	v = start;

	while (intree[v] == FALSE) {
		intree[v] = TRUE;
		p = g->edges[v];
		while (p != NULL) {
			w = p->y;
			weight = p->weight;
			/* CHANGED */
			if (distance[w] > (distance[v] + weight)) {
				/* CHANGED */distance[w] = distance[v] + weight;
				/* CHANGED */parent[w] = v;

			}
			p = p->next;
		}

		v = 1;
		dist = INT_MAX;
		for (i = 1; i <= g->nvertices; i++)
			if ((intree[i] == FALSE) && (dist > distance[i])) {
				dist = distance[i];
				v = i;
			}
	}
	/*for (i=1; i<=g->nvertices; i++) printf("%d %d\n",i,distance[i]);**/
	//printf(" the dijkstra from %d to %d is %d \n", start, end, distance[end]);
	return distance[end];
}

void print_dijkstra(graph *g, int start) {
	//printf("\n Out of Dijkstra\n");

	for (int i = 1; i <= g->nvertices; i++) {
		/*printf(" %d parent=%d\n",i,parent[i]);*/
		find_path(start, i, parent);
	}
	//printf("\n");
}

void print_dijkstra(graph *g, int start, int end) {
	//printf("\n Out of Dijkstra\n");
	find_path(start, end, parent);
	//printf("\n");
}

void get_dijkstra_path(graph *g, int start, int end, int nodes[]) {
	//printf("\n Out of Dijkstra Path\n");
	get_path(path_node, start, end, parent);
	int j = 0;
	for (int i = 0; i < g->nvertices; i++) {
		if (path_node[g->nvertices - i] != -1) {
			//printf(" %d ", path_node[g->nvertices - i]);
			nodes[++j] = path_node[g->nvertices - i];
		}
	}
}

/*
 int main() {
 graph g;
 int i;

 read_graph(&g, FALSE);
 dijkstra(&g, 1);

 for (i = 1; i <= g.nvertices; i++)
 find_path(1, i, parent);
 printf("\n");
 return 0;
 }
 **/
