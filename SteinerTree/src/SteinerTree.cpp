

#include "graph.h"
#include "bool.h"
#include "prim.h"
#include "dijkstra.h"
#include "bfs-dfs.h"

#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

#define tplog_nodes_num 30
#define src_num 3
#define des_num 8
#define case_num  ((int)pow(src_num,des_num))

FILE *fp;

int range_random(int start, int end) {
	return (start + rand() % (end - start + 1));
}

void get_src_dest_infor(int s_num, int src[], int d_num, int dest[]) {

	//time(NULL);
	srand((unsigned) time(NULL));
	//int index;
	fprintf(fp,
			" \n------------------------Topology Nodes (%d)------------------------",
			tplog_nodes_num);
	fprintf(fp,
			" \n------------------------Server Node (%d)------------------------",
			s_num);
	int index = 0;
	while (true) {
		src[index] = range_random(1, tplog_nodes_num);
		int i = 0;
		for (i = 0; i < index; i++) {
			if (src[i] == src[index]) {
				break;
			}
		}
		if (i == index) {

			fprintf(fp, " \n sour_no(%d)  ", src[index]);
			printf(" \n sour_no(%d)  ", src[index]);
			if (index + 1 == s_num) {
				break;
			}
			index++;
		}
	}
	printf("\n");
	fprintf(fp, " \n------------------------Users (%d)------------------------",
			d_num);

	index = 0;
	while (true) {
		dest[index] = range_random(1, tplog_nodes_num);

		int i = 0;
		for (i = 0; i < index; i++) {
			if (dest[i] == dest[index]) {
				break;
			}
		}
		if (i == index) {
			for (i = 0; i < s_num; i++) {
				if (src[i] == dest[index]) {
					break;
				}
			}
			if (i == s_num) {

				fprintf(fp, " \n  dest_no(%d)  ", dest[index]);
				printf(" \n dest_no(%d)  ", dest[index]);
				if (index + 1 != des_num) {
					index++;
				} else {
					break;
				}

			}
		}
	}
	printf("\n");
}

int check_edges(int f, int t, int edges[MAXV + 1][3]) {
	int flag;
	for (int i = 1; i < MAXV + 1; i++) {
		if (-1 == edges[i][1] && -1 == edges[i][2]) {
			edges[i][1] = f;
			edges[i][2] = t;
			//printf(" insert edges : %d  <-> %d \n", edges[i][1], edges[i][2]);
			flag = TRUE;
			break;
		} else {
			if (f == edges[i][1] && t == edges[i][2]) {
				//printf(" the edges existed : %d  <->  %d \n", edges[i][1],
				//		edges[i][2]);
				flag = FALSE;
				break;
			}
		}
	}
	return flag;
}

double multicast_tree_cost(graph* g, int m_tree[MAXV + 1][MAXV + 1],
		int node_num) {

	double cost = 0;

	int edge[MAXV + 1][3];

	for (int i = 0; i < MAXV + 1; i++) {
		for (int j = 0; j < 3; j++) {
			edge[i][j] = -1;
		}
	}

	for (int i = 1; i <= node_num; i++) {
		for (int j = 1; j <= MAXV + 1; j++) {
			if (m_tree[i][j + 1] == -1) {
				break;
			}
			if (TRUE == check_edges(m_tree[i][j], m_tree[i][j + 1], edge)) {
				//			printf("\n edges  %d <-> %d \n", m_tree[i][j], m_tree[i][j + 1]);

				edgenode *p_0;
				p_0 = g->edges[m_tree[i][j]];
				while (p_0 != NULL) {
					if (p_0->orig_edges[2] == m_tree[i][j + 1]) {
						double cost_1 = p_0->weight;
						//					printf("\n edges TRUE %d <-> %d (cost: %d )\n",
						//							m_tree[i][j], p_0->orig_edges[2], cost_1);
						cost = cost_1 + cost;
						//					printf(" cost : %d \n", cost);
						break;
					}
					p_0 = p_0->next;
				}
				//free(p_0);
			}
		}
	}

	return cost;
}

void multicast_tree_delay(graph* g, int m_tree[MAXV + 1][MAXV + 1],
		double delay[], int node_num) {

	double delay_tmp = 0;
	for (int i = 1; i <= MAXV + 1; i++) {
		delay[i] = -1;
	}

	for (int i = 1; i <= node_num; i++) {
		for (int j = 1; j <= MAXV + 1; j++) {
			if (m_tree[i][j + 1] == -1) {
				break;
			}

			edgenode *p_0;
			p_0 = g->edges[m_tree[i][j]];
			while (p_0 != NULL) {
				if (p_0->orig_edges[2] == m_tree[i][j + 1]) {

					//				printf("\n edges TRUE %d <-> %d (cost: %d )\n",
					//					m_tree[i][j], p_0->orig_edges[2], p_0->delay);
					delay_tmp += p_0->delay;
					//		printf(" delay_tmp : %d \n", delay_tmp);
					break;
				}
				p_0 = p_0->next;
			}
		}
		delay[m_tree[i][1]] = delay_tmp;
		delay_tmp = 0;
	}
}
void muticast_tree(int nodes[], int node_num, double wd[2]) {
	graph g;
	graph g_cost;
	graph g_delay;

	graph new_g;
	graph last_g;

	int source = 1;
	double delay[MAXV + 1];
	int p_nodes[MAXV + 1];
	int tree[MAXV + 1][MAXV + 1];
	int orig_tree[MAXV + 1][MAXV + 1];
	int final_tree[MAXV + 1][MAXV + 1];

	for (int i = 0; i <= MAXV + 1; i++) {
		for (int j = 0; j <= MAXV + 1; j++) {
			tree[i][j] = -1;
			orig_tree[i][j] = -1;
			final_tree[i][j] = -1;
		}
	}

	read_graph(&g, FALSE);
	//print_graph(&g);

	build_mesh_topology(&g, &new_g, nodes, node_num, FALSE);

	//print_graph(&new_g);

	prim(&new_g, source);
	//print_prim(&new_g, source);

	get_prim_tree(&new_g, source, tree);

	//	for (int i = 1; i <= new_g.nvertices; i++) {
	//		for (int j = 1; j <= MAXV + 1; j++) {
	//			if (tree[i][j] != -1) {
	//				printf(" %d( i:%d, j:%d ) ", tree[i][j], i, j);
	//			}
	//		}
	//		printf("\n");
	//	}

	printf(" Original Topology nodes ( loop )  \n");
	get_orig_tree(&new_g, &g, tree, orig_tree);
	//print_graph(&g);

	//	for (int i = 1; i <= new_g.nvertices; i++) {
	//		for (int j = 1; j <= MAXV + 1; j++) {
	//			if (orig_tree[i][j] != -1) {
	//				printf(" %d( i:%d, j:%d ) ", orig_tree[i][j], i, j);
	//			} else {
	//				break;
	//			}
	//		}
	//		printf("\n");
	//	}

	printf("aaa");
	make_graph(&g, &last_g, new_g.nvertices, orig_tree, FALSE);
	//print_graph(&last_g);
	prim(&last_g, source);
	//print_prim(&last_g, source);

	for (int l = 1; l <= last_g.nvertices; l++) {
		for (int z = 0; z <= MAXV + 1; z++) {
			p_nodes[z] = -1;
		}

		get_path(p_nodes, 1, l, parent);
		for (int i = 1; i <= node_num; i++) {
			if (last_g.orig_node[p_nodes[1]] == nodes[i]) {
				for (int m = 1; m <= MAXV + 1; m++) {
					if (p_nodes[m] != -1) {
						final_tree[i][m] = last_g.orig_node[p_nodes[m]];
						//						printf(" %d (%d)", p_nodes[m], final_tree[i][m]);
					}
				}
			}
			printf("\n");
		}

	}
	printf("\n");
	for (int i = 1; i <= node_num; i++) {
		for (int j = 1; j <= MAXV + 1; j++) {
			if (final_tree[i][j] != -1) {
				printf(" %d ", final_tree[i][j]);
			}
		}
		printf("\n");
	}
	//	print_graph(&last_g);

	read_graph(&g_cost, FALSE);
	double cost;
	cost = multicast_tree_cost(&g_cost, final_tree, node_num);
	wd[0] = cost;
	//	print_graph(&g_cost);

	read_graph(&g_delay, FALSE);
	multicast_tree_delay(&g_delay, final_tree, delay, node_num);
	double delay_tmp = 0;
	for (int i = 1; i <= MAXV + 1; i++) {
		if (delay[i] != -1) {
			//			printf(" \n node: %d, delay: %d \n ", i, delay[i]);
			if (delay_tmp < delay[i]) {
				delay_tmp = delay[i];
			}
		}
	}
	wd[1] = delay_tmp;
	printf("\n The Multicast Tree Cost :  %.13f  ", wd[0]);
	printf(" \n The Multicast Tree Max delay: %.13f  ", wd[1]);
	printf("\n\n");

}

void get_multicast_group_tree(int src[], int des[]) {
	//////////////////////////////////////////////////
	clock_t start_1, finish_1;
	start_1 = clock();
	int case_set[des_num];
	int user_case[des_num][src_num][case_num];
	int group_case[case_num][src_num][des_num + 1];

	for (int i = 0; i < des_num; i++) {
		for (int j = 0; j < src_num; j++) {
			for (int k = 0; k < case_num; k++) {
				user_case[i][j][k] = -1;
			}
			//			printf(" \n");
		}
		//		printf(" \n");
	}

	//	for (int i = 0; i < des_num; i++) {
	//		for (int j = 0; j < src_num; j++) {
	//			for (int k = 0; k < case_num; k++) {
	//				printf(" %d ", user_case[i][j][k]);
	//			}
	//			printf(" \n");
	//		}
	//		printf(" \n");
	//	}

	for (int i = 0; i < des_num; i++) {
		int move_index = (int) (pow(src_num, (des_num - 1 - i)));
		int times = 0;
		for (int m = 1; m <= (int) (pow(src_num, i + 1)); m++) {

			for (int j = 0; j < src_num; j++) {
				for (int k = 0; k < case_num; k++) {
					if (times <= k && k < times + move_index) {
						user_case[i][j][k] = des[i];
					}
					if (k == times + move_index) {
						break;
					}
				}
				times += move_index;
			}
		}
	}

	//	for (int i = 0; i < des_num; i++) {
	//
	//		for (int j = 0; j < src_num; j++) {
	//
	//			for (int k = 0; k < case_num; k++) {
	//				printf(" %d ", user_case[i][j][k]);
	//			}
	//			printf(" \n");
	//		}
	//		printf(" \n");
	//	}

	for (int i = 0; i < case_num; i++) {
		for (int j = 0; j < src_num; j++) {
			for (int k = 0; k < des_num + 1; k++) {
				group_case[i][j][k] = -1;
				if (k == 0) {
					group_case[i][j][k] = src[j];
				}
			}
			//			printf(" \n");
		}
	}

	//	for (int i = 0; i < case_num; i++) {
	//		for (int j = 0; j < src_num; j++) {
	//			for (int k = 0; k < des_num + 1; k++) {
	//				printf(" %d ", group_case[i][j][k]);
	//			}
	//			printf(" \n");
	//		}
	//		printf(" \n");
	//	}

	for (int i = 0; i < case_num; i++) {
		for (int j = 0; j < src_num; j++) {
			for (int k = 1; k < des_num + 1; k++) {
				if (user_case[k - 1][j][i] != -1) {
					group_case[i][j][k] = user_case[k - 1][j][i];
				}
			}
			//			printf(" \n");
		}
		//		printf(" \n");
	}
	//printf(" \n");
	//	for (int i = 0; i < case_num; i++) {
	//		for (int j = 0; j < src_num; j++) {
	//			for (int k = 0; k < des_num + 1; k++) {
	//				printf(" %d ", group_case[i][j][k]);
	//			}
	//			printf(" \n");
	//		}
	//		printf(" \n");
	//	}

	for (int i = 0; i < case_num; i++) {
		for (int j = 0; j < src_num; j++) {
			int index = 0;
			for (int k = 0; k < des_num + 1; k++) {
				if (group_case[i][j][k] != -1) {
					case_set[index] = group_case[i][j][k];
					index++;
				}
			}

			for (int k = 0; k < des_num + 1; k++) {
				group_case[i][j][k] = case_set[k];
				case_set[k] = -1;
			}
			//		printf(" \n");
		}
		//		printf(" \n");
	}
	//	printf(" \n ------------------------------------- \n");
	//	for (int i = 0; i < case_num; i++) {
	//		for (int j = 0; j < src_num; j++) {
	//			for (int k = 0; k < des_num + 1; k++) {
	//				printf(" %d ", group_case[i][j][k]);
	//			}
	//			printf(" \n");
	//		}
	//		printf(" \n");
	//	}

	int nodes[des_num + 1];

	double wd[case_num][2];
	double wd_tmp[2];
	//	int test[3] = {2,5,4};
	//	muticast_tree(test, 3, wd_tmp);

	for (int i = 0; i < case_num; i++) {
		wd[i][0] = 0;
		wd[i][1] = 0;
	}

	for (int i = 0; i < case_num; i++) {
		//printf(
		//		" \n -----------------Case %d  Start------------------------- \n ",
		//		i);
		for (int j = 0; j < src_num; j++) {

			for (int k = 0; k < des_num + 1; k++) {
				nodes[k] = -1;
			}

			int index = 0;
			for (int k = 0; k < des_num + 1; k++) {
				if (group_case[i][j][k] != -1) {
					nodes[index] = group_case[i][j][k];
					index++;
				}
			}

			int node_tmp[index];
			for (int h = 0; h < index; h++) {
				node_tmp[h] = nodes[h];
				//		printf("  %d ", node_tmp[h]);
			}
			printf("\n");
			printf(" Case %d  Group %d : ", i, j);
			for (int k = 0; k < des_num + 1; k++) {
				printf(" %d ", nodes[k]);
			}

			printf(" \n ");
			if (index != 1) {
				muticast_tree(node_tmp, index, wd_tmp);
				wd[i][0] += wd_tmp[0];
				if (wd[i][1] < wd_tmp[1]) {
					wd[i][1] = wd_tmp[1];
				}
			} else {
				wd[i][0] += wd_tmp[0];
				wd[i][1] += wd_tmp[1];
			}
			wd_tmp[0] = 0;
			wd_tmp[1] = 0;
		}
	}
	fprintf(fp,
			" \n -----------------Case1 Permutation and Combination(M:N) Start------------------------- \n ");
	fprintf(fp, " \n ----------------- Detail ------------------------- \n");
	for (int i = 0; i < case_num; i++) {
		fprintf(fp,
				"       The Case: %d  Total Cast : %.13f  Minimum Delay: %.13f \n",
				i + 1, wd[i][0], wd[i][1]);
	}

	int case_no = -1;
	double min_cost = INT_MAX;
	double max_cost = 0;
	double min_delay = INT_MAX;
	double max_delay = 0;
	fprintf(fp, " \n ----------------- Min Cost ------------------------- \n ");
	for (int i = 0; i < case_num; i++) {
		if (min_cost > wd[i][0] && wd[i][0] != 0) {
			min_cost = wd[i][0];
			case_no = i;
		}
	}
	fprintf(fp, "       The Case: %d  Min Cost : %.13f   Delay : %.13f \n",
			case_no + 1, min_cost, wd[case_no][1]);
	fprintf(fp,
			" \n ----------------- Min Delay ------------------------- \n ");
	for (int i = 0; i < case_num; i++) {
		if (min_delay > wd[i][1] && wd[i][1] != 0) {
			min_delay = wd[i][1];
			case_no = i;
		}
	}
	fprintf(fp, "       The Case: %d   Min Delay : %.13f  Cost : %.13f  \n",
			case_no + 1, min_delay, wd[case_no][0]);
	fprintf(fp, " \n ----------------- Max Cost ------------------------- \n ");
	for (int i = 0; i < case_num; i++) {
		if (max_cost < wd[i][0] && wd[i][0] != 0) {
			max_cost = wd[i][0];
			case_no = i;
		}
	}
	fprintf(fp, "       The Case: %d   Max Cost: %.13f  Delay : %.13f  \n",
			case_no + 1, max_cost, wd[case_no][1]);

	fprintf(fp,
			" \n ----------------- Max Delay ------------------------- \n ");
	for (int i = 0; i < case_num; i++) {
		if (max_delay < wd[i][1] && wd[i][0] != 0) {
			max_delay = wd[i][1];
			case_no = i;
		}
	}
	fprintf(fp, "       The Case: %d   Max Delay: %.13f   Cost : %.13f  \n",
			case_no + 1, max_delay, wd[case_no][0]);

	fprintf(fp,
			" \n -----------------Case1 Permutation and Combination(M:N) End------------------------- \n ");
	finish_1 = clock();
	double duration_1 = (double) (finish_1 - start_1) / 1000000l;
	fprintf(fp,
			" \n     The used time of Case1 Permutation and Combination(M:N)   is %.13f seconds \n",
			duration_1);
	fprintf(fp,
			" -------------------------------------------------------------------------------------- \n");

}

void get_multicast_one_tree(int src[src_num], int des[des_num]) {

	clock_t start_3, finish_3;
	start_3 = clock();
	int nodes[src_num][des_num + 1];

	double wd[src_num][2];
	double wd_tmp[2];

	for (int i = 0; i < src_num; i++) {
		wd[i][0] = 0;
		wd[i][1] = 0;
	}
	for (int i = 0; i < src_num; i++) {
		nodes[i][0] = src[i];
		//printf("\n %d ", nodes[i][0]);
		for (int j = 1; j < des_num + 1; j++) {
			nodes[i][j] = des[j - 1];
			//printf(" %d ", nodes[i][j]);
		}
		printf("\n  ");
		muticast_tree(nodes[i], des_num + 1, wd_tmp);
		wd[i][0] += wd_tmp[0];
		if (wd[i][1] < wd_tmp[1]) {
			wd[i][1] = wd_tmp[1];
		}
	}
	fprintf(fp,
			" \n -----------------Case3 (1:M) Start------------------------- \n ");
	fprintf(fp, " \n ----------------- Detail ------------------------- \n");
	for (int i = 0; i < src_num; i++) {
		fprintf(fp, "       The Case: %d  Total Cast : %.13f  Delay: %.13f \n",
				i + 1, wd[i][0], wd[i][1]);
	}
	int case_no = -1;
	double min_cost = INT_MAX;
	double max_cost = 0;
	double min_delay = INT_MAX;
	double max_delay = 0;
	fprintf(fp, " \n ----------------- Min Cost ------------------------- \n ");
	for (int i = 0; i < src_num; i++) {
		if (min_cost > wd[i][0] && wd[i][0] != 0) {
			min_cost = wd[i][0];
			case_no = i;
		}
	}
	fprintf(fp, "       The Case: %d  Min Cost : %.13f   Delay : %.13f \n",
			case_no + 1, min_cost, wd[case_no][1]);

	fprintf(fp,
			" \n ----------------- Min Delay ------------------------- \n ");
	case_no = 0;
	for (int i = 0; i < src_num; i++) {
		if (min_delay > wd[i][1] && wd[i][1] != 0) {
			min_delay = wd[i][1];
			case_no = i;
		}
	}
	fprintf(fp, "       The Case: %d   Min Delay : %.13f  Cost : %.13f  \n",
			case_no + 1, min_delay, wd[case_no][0]);
	fprintf(fp, " \n ----------------- Max Cost ------------------------- \n ");
	case_no = 0;
	for (int i = 0; i < src_num; i++) {
		if (max_cost < wd[i][0] && wd[i][0] != 0) {
			max_cost = wd[i][0];
			case_no = i;
		}
	}
	fprintf(fp, "       The Case: %d   Max Cost: %.13f   Delay : %.13f  \n",
			case_no + 1, max_cost, wd[case_no][1]);

	fprintf(fp,
			" \n ----------------- Max Delay ------------------------- \n ");
	case_no = 0;
	for (int i = 0; i < src_num; i++) {
		if (max_delay < wd[i][1] && wd[i][1] != 0) {
			max_delay = wd[i][1];
			case_no = i;
		}
	}
	fprintf(fp, "       The Case: %d   Max Delay: %.13f   Cost : %.13f  \n",
			case_no + 1, max_delay, wd[case_no][0]);
	fprintf(fp,
			" \n -----------------Case3 (1:M) End------------------------- \n ");

	finish_3 = clock();
	double duration_3 = (double) (finish_3 - start_3) / 1000000l;
	fprintf(fp, " \n     The used time of Case3 (1:M)  is %.13f seconds \n",
			duration_3);
	fprintf(fp,
			" -------------------------------------------------------------------------------------- \n");
}

void get_multicast_tree_proposal(int src[src_num], int des[des_num]) {
	clock_t start_2, finish_2;
	start_2 = clock();

	graph g;
	int group_list[des_num];

	int nodes[des_num + 1];
	double wd[src_num][2];
	double wd_tmp[2];
	read_graph(&g, FALSE);
	//printf(" \n");

	for (int i = 0; i < des_num; i++) {
		int cost_tmp = INT_MAX;
		for (int j = 0; j < src_num; j++) {
			int cost_t = get_dijkstra(&g, des[i], src[j]);
			if (cost_tmp > cost_t) {
				group_list[i] = src[j];
				cost_tmp = cost_t;
				//printf(" dijkstra : %d \n ", group_list[i]);
			}
		}
		//printf(" the pair is %d %d %d: \n", des[i], group_list[i], i);
	}
	//printf(" \n ");

	for (int i = 0; i < src_num; i++) {

		wd[i][0] = -1;
		wd[i][1] = -1;

		int index = 0;
		for (int j = 0; j < des_num + 1; j++) {
			nodes[j] = -1;
		}
		for (int k = 0; k < des_num + 1; k++) {
			if (k == 0) {
				nodes[0] = src[i];
				index++;
				break;
			}
			nodes[k] = -1;
		}

		printf(" \n ");
		for (int j = 1; j < des_num + 1; j++) {
			if (src[i] == group_list[j - 1]) {
				nodes[index] = des[j - 1];
				index++;
			}
		}
		printf(" \n ");

		if (index != 1) {
			muticast_tree(nodes, index, wd_tmp);
			wd[i][0] = wd_tmp[0];
			wd[i][1] = wd_tmp[1];
		}

	}
	fprintf(fp, " \n -----------------Case2 (M:N) Start  -----------------\n ");
	double cost = 0;
	double delay = 0;

	for (int i = 0; i < src_num; i++) {
		if (wd[i][0] != -1 && wd[i][1] != -1 && wd[i][1] != 0
				&& wd[i][0] != 0) {
			cost += wd[i][0];
			if (delay < wd[i][1] && wd[i][1] != 0) {
				delay = wd[i][1];
			}
		}
	}

	fprintf(fp, "       Total Cast : %.13f   Max Delay: %.13f \n", cost, delay);

	fprintf(fp, " \n -----------------Case2 (M:N) End  -----------------\n ");

	finish_2 = clock();
	double duration_2 = (double) (finish_2 - start_2) / 1000000l;
	//double duration_2 = (double) (finish_2 - start_2) ;
	fprintf(fp, " \n     The used time of Case2 (M:N)   is %.13f seconds \n",
			duration_2);
	fprintf(fp,
			" -------------------------------------------------------------------------------------- \n");

}

int main() {
	//srand(1);

	srand((unsigned) time(NULL));
	stringstream fileName;
	fileName << "Result_" << time(NULL) << ".data";
	fp = fopen(fileName.str().c_str(), "w");
	int src[src_num];
	int des[des_num];
	//int src[4] = { 1, 3, 5 };
	//int wd[2] = { 0, 0 };
	//muticast_tree(src, 3, wd);

	get_src_dest_infor(src_num, src, des_num, des);
	printf("OK");
	//case1
	get_multicast_group_tree(src, des);
	//case 2
	//usleep(10000);
	get_multicast_tree_proposal(src, des);
	//case 3
	//usleep(10000);
	get_multicast_one_tree(src, des);
	return 0;
}
