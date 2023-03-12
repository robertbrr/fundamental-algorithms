/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Evaluarea DFS
 *
 * Complexitatea algoritmului DFS este O(V + E), unde V este numarul varfurilor si E este numarul muchiilor.
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
Profiler h("tema_10");
typedef struct df_node {
	int desc;
	int fin;
	struct df_node * prnt;
	struct df_node** adjL;
	int adjNo;
	int clr;
	int val;
	int idx;
	int onSt;
	int LL;
	int compon;
}DFNode;

typedef struct stack_node {
	DFNode* nd;
	struct stack_node* next;
}STNode;

typedef struct {
	int vf_no;
	DFNode** vf;
}DFGrf;
enum { CLR_WHITE, CLR_GRAY, CLR_BLACK };


void push_st(STNode** stackTop, DFNode* toPush) {
	STNode* tmp = (STNode*)malloc(sizeof(STNode));
	tmp->nd = toPush;
	tmp->next = NULL;
	if (*stackTop == NULL) {
		*stackTop = tmp;
	} 
	else {
		tmp->next = *stackTop;
		*stackTop = tmp;
	}
}

DFNode* pop_st(STNode** stackTop) {
	if (*stackTop == NULL) return NULL;
	DFNode* aux = (*stackTop)->nd;
	*stackTop = (*stackTop)->next;
	return aux;
}

int switch_ = 0;
STNode* sTop = NULL;

void DFSv(DFGrf* grf, DFNode* curr_n,int* tm,Operation oper){
	(*tm)++;
	curr_n->desc = *tm;
	curr_n->clr = CLR_GRAY;
	oper.count(2);
	for (int i = 0; i < curr_n->adjNo; i++) {
		oper.count(1);
		if (curr_n->adjL[i]->clr == CLR_WHITE)
		{	
			oper.count(1);
			curr_n->adjL[i]->prnt = curr_n;
			DFSv(grf, curr_n->adjL[i], tm,oper);
		}
	}
	curr_n->clr = CLR_BLACK;
	(*tm)++;
	curr_n->fin = *tm;
	if (switch_ == 1) push_st(&sTop, curr_n);//topological sort
	oper.count(2);
}

void DFS(DFGrf* grf,Operation oper) {
	for (int i = 0; i < grf->vf_no; i++) {
		grf->vf[i]->clr = CLR_WHITE;
		oper.count(1);
	}

	int tm = 0;

	for (int l = 0; l < grf->vf_no; l++) {
		oper.count(1);
		if (grf->vf[l]->clr == CLR_WHITE) {
			DFSv(grf, grf->vf[l], &tm,oper);
		}
	}
}


void print_rprz_R1(int* s, int s_size, int lookingFor, int mt_spaces) {
	for (int i = 0; i < s_size; i++)
		if (s[i] == lookingFor) {
			for (int i = 0; i < mt_spaces; i++)
				printf("\t");
			printf("%d\n", i);
			print_rprz_R1(s, s_size, i, mt_spaces + 1);
		}
}


int minFind(int x1, int y1) {
	if (x1 > y1) return y1;
	else return x1;
}

void ST_CON(DFGrf* grf, DFNode* curr_n, int* idx, STNode** TJStack, int* nrCo) {
	curr_n->idx = *idx;
	curr_n->LL = *idx;
	(*idx)++;
	push_st(TJStack, curr_n);
	curr_n->onSt = 1;
	for (int j = 0; j < curr_n->adjNo; j++)
	{
		if (curr_n->adjL[j]->idx == -1)
		{
			ST_CON(grf, curr_n->adjL[j], idx, TJStack, nrCo);
			curr_n->LL = minFind(curr_n->LL, curr_n->adjL[j]->LL);
		}
		else if (curr_n->adjL[j]->onSt == 1) {
			curr_n->LL = minFind(curr_n->LL, curr_n->adjL[j]->idx);
		}
	}
	DFNode* aux_n;
	if (curr_n->LL == curr_n->idx) {
		(*nrCo)++;
		do {
			aux_n = pop_st(TJStack);
			aux_n->onSt = 0;
			aux_n->compon = *nrCo;
		} while (curr_n != aux_n);
	}
}


void T_A_R_J_A_N(DFGrf* grf) {
	int idx = 0;
	STNode* TJStack = NULL;
	int nrCo = 0;
	for (int i = 0; i < grf->vf_no; i++) {
		grf->vf[i]->idx = -1;
		grf->vf[i]->LL = -1;
		grf->vf[i]->onSt = 0;
		grf->vf[i]->compon = 0;
	}
	for (int i = 0; i < grf->vf_no; i++) {
		if (grf->vf[i]->idx == -1)
			ST_CON(grf, grf->vf[i], &idx, &TJStack, &nrCo);
	}

}


int checkr(DFGrf* gr, int first, int last) {
	for (int i = 0; i < gr->vf[first]->adjNo; i++) {
		if (gr->vf[first]->adjL[i] == gr->vf[last]) {
			return 0;
		}
	}
	return 1;
}


void gen_rnd_graph(DFGrf* gr, int nr_edg) {
	switch_ = 0;
	int* s = (int*)malloc(gr->vf_no * sizeof(int));
	FillRandomArray(s, gr->vf_no, 0, gr->vf_no - 1, true);
	for (int i = 0; i < gr->vf_no; i++) {
		gr->vf[i]->adjNo = 0;
		gr->vf[i]->adjL = (DFNode**)malloc(sizeof(DFNode*) * gr->vf_no);
	}
	int first, last;
	for (int i = 0; i < nr_edg; i++) {
		first = rand() % gr->vf_no;
		last = rand() % gr->vf_no;
		if (first != last && checkr(gr, first, last)) {

			gr->vf[first]->adjL[gr->vf[first]->adjNo] = gr->vf[last];
			(gr->vf[first]->adjNo)++;
		}
		else
			i--;

	}
}
void test_corect()
{
	DFGrf g;
	g.vf_no = 9;
	g.vf = (DFNode**)malloc(sizeof(DFNode*) * g.vf_no);
	for (int i = 0; i < g.vf_no; i++) {
		g.vf[i] = (DFNode*)malloc(sizeof(DFNode));
		g.vf[i]->val = i;
		g.vf[i]->prnt = NULL;
	}
	g.vf[0]->adjNo = 1;
	g.vf[0]->adjL = (DFNode**)malloc(g.vf[0]->adjNo * sizeof(DFNode*));
	g.vf[0]->adjL[0] = g.vf[1];

	g.vf[1]->adjNo = 1;
	g.vf[1]->adjL = (DFNode**)malloc(g.vf[1]->adjNo * sizeof(DFNode*));
	g.vf[1]->adjL[0] = g.vf[2];

	g.vf[2]->adjNo = 2;
	g.vf[2]->adjL = (DFNode**)malloc(g.vf[2]->adjNo * sizeof(DFNode*));
	g.vf[2]->adjL[0] = g.vf[0];
	g.vf[2]->adjL[1] = g.vf[3];

	g.vf[3]->adjNo = 2;
	g.vf[3]->adjL = (DFNode**)malloc(g.vf[3]->adjNo * sizeof(DFNode*));
	g.vf[3]->adjL[0] = g.vf[4];
	g.vf[3]->adjL[1] = g.vf[8];

	g.vf[4]->adjNo = 1;
	g.vf[4]->adjL = (DFNode**)malloc(g.vf[4]->adjNo * sizeof(DFNode*));
	g.vf[4]->adjL[0] = g.vf[5];

	g.vf[5]->adjNo = 3;
	g.vf[5]->adjL = (DFNode**)malloc(g.vf[5]->adjNo * sizeof(DFNode*));
	g.vf[5]->adjL[0] = g.vf[3];
	g.vf[5]->adjL[1] = g.vf[6];
	g.vf[5]->adjL[2] = g.vf[7];

	g.vf[6]->adjNo = 0;
	g.vf[7]->adjNo = 0;

	g.vf[8]->adjNo = 1;
	g.vf[8]->adjL = (DFNode**)malloc(g.vf[8]->adjNo * sizeof(DFNode*));
	g.vf[8]->adjL[0] = g.vf[3];


	Operation useless = h.createOperation("no", 1);
	switch_ = 1;
	DFS(&g, useless);
	switch_ = 0;
	int* s = (int*)malloc(sizeof(int) * g.vf_no);

	for (int i = 0; i < g.vf_no; i++)
		if (g.vf[i]->prnt == NULL)
			s[g.vf[i]->val] = -1;
		else
			s[g.vf[i]->val] = g.vf[i]->prnt->val;
	puts("Arborele DFS");
	print_rprz_R1(s, g.vf_no, -1, 0);
	printf("\n");
	printf("Varf || Parinte || Timp descoperire ||  Timp finalizare\n");
	for (int i = 0; i < g.vf_no; i++) {
		int x;
		if (g.vf[i]->prnt == NULL)
			x = -1;
		else
			x = g.vf[i]->prnt->val;
		printf("%3d%9d%15d%20d\n", g.vf[i]->val, x, g.vf[i]->desc, g.vf[i]->fin);
	}

	puts("\nSORTARE TOPOLOGICA");

	for (STNode* j = sTop; j != NULL; j = j->next) {
		printf("%d ", j->nd->val);
	}
	printf("\n");
	T_A_R_J_A_N(&g);
	printf("\nVarf || LowLink || Componenta \n");
	for (int i = 0; i < g.vf_no; i++) {
		int x;
		printf("%3d%9d%12d\n", g.vf[i]->val, g.vf[i]->LL,g.vf[i]->compon);
	}

}


void test() {
	for (int n = 1000; n <= 4500; n += 100) {
		Operation op_edge_var = h.createOperation("dfs_edge_var", n);
		DFGrf grf;
		grf.vf_no = 100;
		grf.vf = (DFNode**)malloc((grf.vf_no) * sizeof(DFNode*));
		DFNode** helper = grf.vf;
		for (int l = 0; l < grf.vf_no; l++) {
			grf.vf[l] = (DFNode*)malloc(sizeof(DFNode));
			grf.vf[l]->val = l;
		}
		gen_rnd_graph(&grf, n);
		DFS(&grf, op_edge_var);
	}

	for (int n = 100; n <= 200; n += 10) {
		Operation op_vertices_var = h.createOperation("dfs_vertices_var", n);
		DFGrf grf;
		grf.vf_no = n;
		grf.vf = (DFNode**)malloc((grf.vf_no) * sizeof(DFNode*));
		DFNode** helper = grf.vf;
		for (int l = 0; l < grf.vf_no; l++) {
			grf.vf[l] = (DFNode*)malloc(sizeof(DFNode));
			grf.vf[l]->val = l;
		}
		gen_rnd_graph(&grf, 4500);
		DFS(&grf, op_vertices_var);
	}

	h.showReport();
}


int main() {
	test_corect();
	//test();
	return 0;
}