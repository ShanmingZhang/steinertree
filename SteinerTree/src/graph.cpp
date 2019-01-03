#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#include "bool.h"
#include "queue.h"
#include "graph.h"
#include "dijkstra.h"

int new_node_no[3][MAXV + 1];
int current_no;
char fname[] = "40.brite";

void initialize_graph(graph *g, bool directed) {
	int i; /* counter */

	g->nvertices = 0;
	g->nedges = 0;
	g->directed = directed;

	for (i = 1; i <= MAXV+1; i++) {
		g->degree[i] = 0;
		g->inDegree[i] = 0;
		g->orig_node[i] = -1;
	}
	for (i = 1; i <= MAXV+1; i++)
		g->edges[i] = NULL;
}

void read_graph(graph *g, bool directed) {

	initialize_graph(g, directed);

	//scanf("%d %d", &(g->nvertices), &m);
	get_topology_infor(g);

	get_topology_edges(g, directed);

}

void insert_edge(graph *g, int x, int y, double w, double d, bool directed) {
	edgenode *p; /* temporary pointer */

	p = (edgenode*) malloc(sizeof(edgenode)); /* allocate storage for edgenode */

	p->weight = w;
	p->delay = d;
	p->y = y;

	for (int i = 1; i < MAXV + 1; i++) {
		p->orig_edges[i] = -1;
	}

	p->orig_edges[1] = x;
	p->orig_edges[2] = y;

	p->next = g->edges[x];

	g->edges[x] = p; /* insert at head of list */

	g->degree[x]++;

	if (directed == FALSE)
		insert_edge(g, y, x, w, d, TRUE);
	else
		g->nedges++;
}

void insert_edge(graph *g, int x, int y, double w, double d, int org_edges[],
		bool directed) {
	edgenode *p; /* temporary pointer */

	p = (edgenode*) malloc(sizeof(edgenode)); /* allocate storage for edgenode */

	p->weight = w;
	p->delay = d;
	p->y = y;

	for (int i = 1; i < MAXV + 1; i++) {
		p->orig_edges[i] = -1;
	}

	for (int i = 1; i <= MAXV + 1; i++) {
		//printf(" i --- = %d ", i);
		if (org_edges[i] != -1) {
			p->orig_edges[i] = org_edges[i];
			//printf(" %d (%d, %d) - ", p->orig_edges[i], x, y);
			//org_edges[i] = -1;

		} else {
			break;
		}
	}
	//printf("\n");
	p->next = g->edges[x];

	g->edges[x] = p; /* insert at head of list */

	g->degree[x]++;
	g->inDegree[x]++;

	if (directed == FALSE)
		insert_edge(g, y, x, w, d, org_edges, TRUE);
	else
		g->nedges++;

}

void delete_edge(graph *g, int x, int y, bool directed) {

	edgenode *p, *p_back; /* temporary pointers */

	p = g->edges[x];
	p_back = NULL;

	while (p != NULL)
		if (p->y == y) {
			g->degree[x]--;
			if (p_back != NULL)
				p_back->next = p->next;
			else
				g->edges[x] = p->next;

			free(p);

			if (directed == FALSE)
				delete_edge(g, y, x, TRUE);
			else
				g->nedges--;

			return;
		} else
			p = p->next;

	//printf("Warning: deletion(%d,%d) not found in g.\n", x, y);
}

void print_graph(graph *g) {
	int i; /* counter */
	edgenode *p; /* temporary pointer */

	for (i = 1; i <= g->nvertices; i++) {
		printf("No %d ( O_No %d ): ", i, g->orig_node[i]);
		p = g->edges[i];
		while (p != NULL) {

			//printf(" %d ( weight: %.13f, delay: %.13f ) ", p->y, p->weight,
			//		p->delay);

			printf(" %d ( weight: %.6f delay: %.6f ) ", p->y, p->weight, p->delay);
			p = p->next;
		}
		printf("\n");
	}
}
///

void get_topology_infor(graph *g) {
	//file path
	//char fname[] = "10.brite";

	int topl_node_num;
	int b_node_pos;
	int e_node_pos;

	int topl_edge_num;
	int b_edge_pos;
	int e_edge_pos;

	//read file by line
	string line;

	ifstream ifs(fname);
	if (!ifs) {
		printf("Error.Can't open input data file. \n");
		exit(1);
	}

	//read first line to get the numbers of topology nodes and edges
	getline(ifs, line);

	//'Topology: ( '
	b_node_pos = line.find("( ") + 2;

	//' Nodes'
	e_node_pos = line.find(" Node");

	string node_num = line.substr(b_node_pos, e_node_pos - b_node_pos);

	//', '
	b_edge_pos = line.find(", ") + 2;

	//' Edges'
	e_edge_pos = line.find(" Edges");

	string edge_num = line.substr(b_edge_pos, e_edge_pos - b_edge_pos);

	stringstream nstream;
	nstream << node_num;
	nstream >> topl_node_num;

	g->nvertices = topl_node_num;
	printf("Topology Nodes : %d \n", topl_node_num);
	stringstream estream;
	estream << edge_num;
	estream >> topl_edge_num;

	g->nedges = topl_edge_num;
	printf("Topology Edges : %d \n", topl_edge_num);
	ifs.close();
}

void get_topology_edges(graph *g, bool directed) {

	//file path
	//char fname[] = "10.brite";

	int linkId;
	int from;
	int to;
	int fromDomain;
	int toDomain;

	double weight;
	double delay;
	double bandwidth;
	//read file by line
	string line;
	string edges("Edges:");

	ifstream ifs(fname);
	if (!ifs) {
		printf("Error.Can't open input data file. \n");
		exit(1);
	}

	// search line contains "Edges:" string
	while (getline(ifs, line) && line.find(edges) == string::npos) {
	}

	while (getline(ifs, line) && line.size() != 0) {
		istringstream is(line);
		is >> linkId >> from >> to >> weight >> bandwidth >> delay
				>> fromDomain >> toDomain;
		//printf("weight : %.6f  deley : %.6f ", weight, delay);
		// node No is from 1
		insert_edge(g, from + 1, to + 1, weight, delay, directed);
	}

	ifs.close();
}

void build_mesh_topology(graph *g, graph *new_g, int new_nodes[], int num,
		bool directed) {
	initialize_graph(new_g, directed);

	new_g->nvertices = num;

	int nodes[MAXV + 1];

	for (int i = 1; i < MAXV + 1; i++) {
		nodes[i] = -1;
	}

	double weight = 0;

	for (int i = 0; i < num; i++) {
		new_g->orig_node[i + 1] = new_nodes[i];
		for (int j = i + 1; j < num; j++) {
			weight = get_dijkstra(g, new_nodes[i], new_nodes[j]);
			get_dijkstra_path(g, new_nodes[i], new_nodes[j], nodes);
			//			printf(" \n ");
			insert_edge(new_g, i + 1, j + 1, weight, 0, nodes, directed);
			for (int i = 1; i < MAXV + 1; i++) {
				nodes[i] = -1;
			}
		}
	}
}

void get_orig_tree(graph* g, graph* orig_g, int tree[MAXV + 1][MAXV + 1],
		int orig_tree[MAXV + 1][MAXV + 1]) {

	//edgenode *p;
	//p = (edgenode*) malloc(sizeof(edgenode));

	printf("get_orig_tree 1 \n");
	for (int i = 1; i <= g->nvertices; i++) {
		for (int j = 1; j <= MAXV + 1; j++) {
			if (tree[i][j] != -1) {
				tree[i][j] = g->orig_node[tree[i][j]];
				printf(" %d  ( %d )", tree[i][j], tree[i][j]);
			}
		}
		printf("\n");
	}

	printf("get_orig_tree 2 \n");

	int orig_nodes[MAXV + 1];
	int tmp_nodes[MAXV + 1];

	for (int i = 1; i <= g->nvertices; i++) {

		for (int m = 0; m <= MAXV + 1; m++) {
			orig_nodes[m] = -1;
		}
		int index = 1;
		for (int j = 1; j <= MAXV + 1; j++) {

			if (tree[i][j] != -1 && tree[i][j + 1] != -1) {
				for (int k = 0; k <= MAXV + 1; k++) {
					tmp_nodes[k] = -1;
				}
				get_dijkstra(orig_g, tree[i][j], tree[i][j + 1]);
				get_dijkstra_path(orig_g, tree[i][j], tree[i][j + 1], tmp_nodes);
				for (int k = 1; k <= MAXV + 1; k++) {
					if (tmp_nodes[k - 1] != -1) {
						orig_nodes[index] = tmp_nodes[k - 1];
						//printf(" (%d) ", orig_nodes[index]);
						index++;
					}
				}
			} else {
				if (j == 1) {
					orig_nodes[index] = tree[i][j];
					//printf(" (%d) \n", orig_tree[i][index]);
					break;
				}
			}
			index--;
		}
		for (index = 1; index <= MAXV + 1; index++) {
			if (orig_nodes[index] != -1) {
				orig_tree[i][index] = orig_nodes[index];
				printf(" (%d) ", orig_tree[i][index]);
			}
		}
		printf(" \n");
	}

}

void make_graph(graph* g, graph* last_g, int branch_num,
		int orig_tree[MAXV + 1][MAXV + 1], bool directed) {

	double wd[3] = { -1, -1, -1 };

	make_new_no(branch_num, orig_tree);

	initialize_graph(last_g, directed);
	last_g->nvertices = current_no - 1;

	for (int m = 1; m < current_no; m++) {
		for (int n = 1; n <= MAXV + 1; n++) {
			if (new_node_no[2][n] == m) {
				last_g->orig_node[m] = new_node_no[1][n];
				printf(" lsat _g : %d(%d) ", last_g->orig_node[m],
						new_node_no[2][n]);
				break;
			}
		}

	}

	for (int i = 1; i <= branch_num; i++) {
		for (int j = 1; j <= MAXV + 1; j++) {
			if (orig_tree[i][j + 1] == -1) {
				break;
			}
			///get weight and delay according to nodes from g
			get_edge_infor(g, orig_tree[i][j], orig_tree[i][j + 1], wd);
			//instert into last_g

			insert_tree_edge(last_g, new_node_no[2][orig_tree[i][j]],
					new_node_no[2][orig_tree[i][j + 1]], orig_tree[i][j],
					orig_tree[i][j + 1], wd[1], wd[2], directed);
			///
		}
	}
}

void initialize_node_no() {
	for (int i = 1; i <= MAXV + 1; i++) {
		new_node_no[1][i] = i;
	}
	for (int j = 1; j <= MAXV + 1; j++) {
		new_node_no[2][j] = -1;
	}
	current_no = 1;
}

void make_new_no(int nodes, int orig_tree[MAXV + 1][MAXV + 1]) {
	initialize_node_no();

	for (int i = 1; i <= nodes; i++) {
		for (int j = 1; j <= MAXV + 1; j++) {
			if (orig_tree[i][j] != -1) {
				if (new_node_no[1][orig_tree[i][j]] == orig_tree[i][j]
						&& new_node_no[2][orig_tree[i][j]] == -1) {
					new_node_no[2][orig_tree[i][j]] = current_no;
					current_no++;
					printf(" %d(%d) ", new_node_no[1][orig_tree[i][j]],
							new_node_no[2][orig_tree[i][j]]);
				}
			}
		}
	}
}

void get_edge_infor(graph *g, int f_node, int t_node, double wd[3]) {

	edgenode *p; /* temporary pointer */
	p = g->edges[f_node];

	while (p != NULL) {

		if (p->y == t_node) {
			wd[1] = p->weight;
			wd[2] = p->delay;
			//printf(" (%d -> %d weight: %d (delay: %d )   \n", f_node, t_node,
			//		wd[1], wd[2]);
			//free(p);
			break;
		}

		p = p->next;
	}
}

void insert_tree_edge(graph *g, int x, int y, int orgi_x, int orgi_y, double w,
		double d, bool directed) {
	if (orgi_x == orgi_y) {
		return;
	}
	bool flag = FALSE;
	edgenode *p_0, *p;

	p_0 = (edgenode*) malloc(sizeof(edgenode));

	p_0 = g->edges[x];

	while (p_0 != NULL) {
		if (p_0->y == y) {
			//			printf("\n TRUE %d(%d), %d(%d) %d %d \n", x, orgi_x, y, orgi_y, w, d);
			flag = TRUE;
			break;
		}
//		if (g->inDegree[p_0->y] == 1) {
//			flag = TRUE;
//			break;
//		}
		p_0 = p_0->next;
	}
	//free(p_0);
	if (flag == FALSE) {
		printf("\n FLASE %d(%d), %d(%d) %.13f %.13f \n", x, orgi_x, y, orgi_y, w, d);
		p = (edgenode*) malloc(sizeof(edgenode));
		p->weight = w;
		p->delay = d;
		p->y = y;

		for (int i = 1; i < MAXV + 1; i++) {
			p->orig_edges[i] = -1;
		}

		p->orig_edges[1] = orgi_x;
		p->orig_edges[2] = orgi_y;
		p->next = g->edges[x];

		g->edges[x] = p;

		g->degree[x]++;
		//g->inDegree[y]++;

		if (directed == FALSE) {
			insert_tree_edge(g, y, x, orgi_y, orgi_x, w, d, TRUE);
		} else {
			g->nedges++;
		}
	}

}

