/*
 * prim.h
 *
 *  Created on: 2013/01/31
 *      Author: Shanming Zhang
 */



void prim(graph *g, int start);
void print_prim(graph *g, int start);
void print_prim(graph *g, int start, int end);
void get_prim_tree(graph *g, int start, int tree[][MAXV+1]);
