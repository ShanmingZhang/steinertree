#define	MAXV		40	/* maximum number of vertices */

/*	DFS edge types		*/

#define TREE		0		/* tree edge */
#define BACK		1		/* back edge */
#define CROSS		2		/* cross edge */
#define FORWARD		3		/* forward edge */

extern int parent[MAXV + 1];
#ifndef PRIM_H_
#define PRIM_H_

typedef struct edgenode {
	int y; /* adjancency info */
	double weight; /* edge weight, if any */
	double delay;
	struct edgenode *next; /* next edge in list */
	int orig_edges[MAXV + 1];
} edgenode;

typedef struct {
	edgenode *edges[MAXV + 1]; /* adjacency info */
	int degree[MAXV + 1]; /* outdegree of each vertex */
	int inDegree[MAXV + 1]; /* outdegree of each vertex */
	int nvertices; /* number of vertices in the graph */
	int nedges; /* number of edges in the graph */
	int directed; /* is the graph directed? */
	int orig_node[MAXV + 1]; /* node no is original topology node no*/
} graph;
#endif

void initialize_graph(graph *g, bool directed);
void read_graph(graph *g, bool directed);
void insert_edge(graph *g, int x, int y, double w, double d, bool directed);
void insert_edge(graph *g, int x, int y, double w, double d, int org_edges[],
		bool directed);

void delete_edge(graph *g, int x, int y, bool directed);
void print_graph(graph *g);
void get_topology_infor(graph *g);
void get_topology_edges(graph *g, bool directed);
void build_mesh_topology(graph *g, graph *new_g, int new_nodes[], int num,
		bool directed);
void get_orig_tree(graph* g, graph* orig,int org_tree[][MAXV + 1], int tree[][MAXV + 1]);
void make_graph(graph* g, graph* last_g, int branch_num,
		int orig_tree[][MAXV + 1], bool directed);
void insert_tree_edge(graph *g, int x, int y, int orgi_x, int orgi_y, double w,
		double d, bool directed);
void make_new_no(int nodes, int orig_tree[MAXV + 1][MAXV + 1]);
void get_edge_infor(graph *g, int f_node, int t_node, double wd[3]);

