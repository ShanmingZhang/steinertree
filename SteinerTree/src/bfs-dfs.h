/*
 * bfs-dfs.h
 *
 *  Created on: 2013/01/31
 *      Author: Shanming Zhang
 */

#include "graph.h"

#ifndef BFS_DFS_H_
#define BFS_DFS_H_




#endif /* BFS_DFS_H_ */

void initialize_search(graph *g);
void bfs(graph *g, int start);
int edge_classification(int x, int y);
void dfs(graph *g, int v);
void find_path(int start, int end, int parents[]);
int* get_path(int start, int end, int parents[]);
void get_path(int path[], int start, int end, int parents[]);
