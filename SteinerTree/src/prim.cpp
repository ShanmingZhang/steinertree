//============================================================================
// Name        : Prim.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "bfs-dfs.h"
#include "bool.h"
#include "graph.h"

int parent[MAXV + 1]; /* discovery relation */
/************************************************************/
int branch_node[MAXV + 1];

void prim(graph *g, int start) {
	int i; /* counter */
	edgenode *p; /* temporary pointer */
	boolean intree[MAXV + 1]; /* is the vertex in the tree yet? */
	double distance[MAXV + 1]; /* cost of adding to tree */
	int v; /* current vertex to process */
	int w; /* candidate next vertex */
	double weight; /* edge weight */
	double dist; /* best current distance from start */

	for (i = 1; i <= g->nvertices; i++) {
		intree[i] = FALSE;
		distance[i] = INT_MAX;
		parent[i] = -1;
	}
	for (i = 1; i <= MAXV + 1; i++) {
		parent[i] = -1;
	}

	distance[start] = 0;
	v = start;

	while (intree[v] == FALSE) {
		intree[v] = TRUE;
		p = g->edges[v];
		while (p != NULL) {
			w = p->y;
			weight = p->weight;
			if ((distance[w] > weight) && (intree[w] == FALSE)) {
				distance[w] = weight;
				parent[w] = v;
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
}

void print_prim(graph *g, int start, int end) {
//	printf("Out of Prim's branch from %d to %d \n", start, end);
	find_path(start, end, parent);
//	printf("\n");
}

void print_prim(graph *g, int start) {
	printf("Out of Prim\n");

	for (int i = 1; i <= g->nvertices; i++) {
		//printf(" %d parent=%d\n",i,parent[i]);
		find_path(start, i, parent);
	}
//	printf("\n");
}

void get_prim_tree(graph *g, int start, int tree[][MAXV+1]) {
	int index = 1;
	printf("\n Out of Dijkstra Path\n");
	for (int k = 0; k <= MAXV + 1; k++) {
		branch_node[k] = -1;
	}

	for (int i = 1; i <= g->nvertices; i++) {
		get_path(branch_node, start, i, parent);
		for (int j = MAXV + 1; j >= 1; j--) {
			if (branch_node[j] != -1) {
				printf(" %d ", branch_node[j]);
				tree[i][index] = branch_node[j];
				branch_node[j] = -1;
				index++;
			}
		}
		printf("\n");
	}

	printf(" location change \n");
	index = 1;
	for (int i = 1; i <= g->nvertices; i++) {
		for (int j = 1; j <= MAXV + 1; j++) {
			if (tree[i][j] != -1) {
				tree[i][index] = tree[i][j];
				index++;
				if (i != 1 && j != 1) {
					tree[i][j] = -1;
				}
				printf(" %d ", tree[i][index-1]);
			}
		}
		index = 1;
		printf("\n");
	}

	index = 1;
}
/**
 int main() {
 graph g;
 int i;

 read_graph(&g, FALSE);

 prim(&g, 6);

 printf("Out of Prim\n");

 for (i = 1; i <= g.nvertices; i++) {
 //printf(" %d parent=%d\n",i,parent[i]);
 find_path(6, i, parent);
 }
 printf("\n");
 return 0;
 }
 */

