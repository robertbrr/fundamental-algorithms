/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Evaluarea operayiilor pe multimi disjuncte cu ajutorul algoritmului lui Kruskal
 *
 * Operatia make_set are rolul de a creea o multime cu un singur element, efectuand 3 atribuiri si deci avand complexitatea O(1)
 * Functia de link la fel se realizeaza in O(1), iar operatia de uniune depinde astfel de complexitatea operatiei find
 * Rank-ul unui nod dintr-un set este cel mult log(n), iar functia de find are complexitatea log(n), dar aceasta returneaza pentru
 * fiecare nod parcurs parintele set-ului, efectuand astfel path compression, iar la urmatoarea apelare a acestei functii, rezultatul
 * este obtinut in O(1)
 * 
 **/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler G("tema08_af");
typedef struct node_set {
	struct node_set* pr;
	int rank;
	int val;
}NodeSt;

typedef struct {
	int a1;
	int a2;
	int val;
}MucSt;

typedef struct {
	int v_no;
	int m_no;
	MucSt* muchii;
}GrfSt;



NodeSt* makeSt(int value, Operation oper) {
	NodeSt* t = (NodeSt*)malloc(sizeof(NodeSt));
	t->pr = t;
	t->rank = 0;
	t->val = value;
	oper.count(3);
	return t;
}

void linkSt(NodeSt* t1, NodeSt* t2, Operation oper) {
	oper.count();
	if (t1->rank > t2->rank) {
		t2->pr = t1;
		oper.count();
	}
	else {
		t1->pr = t2;
		oper.count(2);
		if (t1->rank == t2->rank) {
			(t2->rank)++;
			oper.count();
		}
	}
}

NodeSt* findSt(NodeSt* t, Operation oper) {
	oper.count();
	if (t != t->pr) {
		oper.count();
		t->pr = findSt(t->pr, oper);
	}
	return t->pr;
}

void unionSt(NodeSt* t1, NodeSt* t2, Operation oper) {
	linkSt(findSt(t1,oper), findSt(t2,oper),oper);
}

int part(MucSt* s, int i1, int i2) {
	MucSt pvt = s[i2];
	int i = i1 - 1;
	for (int j = i1; j <= i2 - 1; j++) {
		if (s[j].val <= pvt.val) {

			i++;
			MucSt tmp = s[j];
			s[j] = s[i];
			s[i] = tmp;
		}
	}
	MucSt tmp = s[i2];
	s[i2] = s[i + 1];
	s[i + 1] = tmp;
	return i + 1;
}
void quick_sort(MucSt* s, int i1, int i2) {
	if (i1 < i2) {
		int new_idx = part(s, i1, i2);
		quick_sort(s, i1, new_idx - 1);
		quick_sort(s, new_idx + 1, i2);
	}
}


GrfSt Kruskal(GrfSt h, Operation op_make, Operation op_uni, Operation op_find) {
	GrfSt res_t;
	res_t.v_no = h.v_no;
	res_t.muchii = (MucSt*)malloc(h.m_no * sizeof(MucSt));
	NodeSt** aux = (NodeSt**)malloc((h.v_no + 1) * sizeof(NodeSt*));
	for (int i = 1; i <= h.v_no; i++)
		aux[i] = makeSt(i,op_make);
	int current_idx = 0;
	quick_sort(h.muchii, 0, h.m_no - 1);
	for (int i = 0; i < h.m_no; i++) {
		NodeSt* vf1 = findSt(aux[h.muchii[i].a1],op_find);
		NodeSt* vf2 = findSt(aux[h.muchii[i].a2],op_find);
		if (vf1 != vf2)
		{
			res_t.muchii[current_idx].a1 = aux[h.muchii[i].a1]->val;
			res_t.muchii[current_idx].a2 = aux[h.muchii[i].a2]->val;
			res_t.muchii[current_idx].val = h.muchii[i].val;
			unionSt(aux[h.muchii[i].a1], aux[h.muchii[i].a2],op_uni);
			current_idx++;
		}
	}
	res_t.m_no = current_idx;
	return res_t;
}

void afis(NodeSt* t) {
	Operation tmp = G.createOperation("tmp", 1);
	printf("Nod %d || Rank %d || Set %d\n", t->val, t->rank,findSt(t,tmp)->val);
}

void afis_arr(NodeSt** t, int t_size) {
	for (int i = 1; i <= t_size; i++)
		afis(t[i]);
	printf("\n");
}
void afis_tree(GrfSt h) {
	printf("VARFURI: %d\nMUCHII: %d\n", h.v_no, h.m_no);
	for (int i = 0; i < h.m_no; i++)
		printf("[%d, %d]   ", h.muchii[i].a1, h.muchii[i].a2);
	printf("\n");
}

void test_corect() {
	Operation useless = G.createOperation("useless", 1);
	int n = 6;
	NodeSt** t = (NodeSt**)malloc((n+1) * sizeof(NodeSt*));
	for (int i = 1; i <= n; i++)
		t[i] = makeSt(i,useless);
	afis_arr(t, n);
	unionSt(t[1], t[2],useless);
	afis_arr(t, n);
	unionSt(t[3], t[4],useless);
	afis_arr(t, n);
	unionSt(t[5], t[6],useless);
	afis_arr(t, n);
	unionSt(t[1], t[4],useless);
	afis_arr(t, n);
	unionSt(t[4], t[5], useless);
	afis_arr(t, n);
}

void test_corect_K() {
	Operation tmp = G.createOperation("tmp", 2);
	GrfSt h;
	h.v_no = 6;
	h.m_no = 9;
	h.muchii = (MucSt*)malloc(h.m_no * sizeof(MucSt));
	h.muchii[0] = { 1,2,1 };
	h.muchii[1] = { 2,3,5 };
	h.muchii[2] = { 3,4,3 };
	h.muchii[3] = { 1,3,2 };
	h.muchii[4] = { 1,4,6 };
	h.muchii[5] = { 3,5,4 };
	h.muchii[6] = { 3,6,8 };
	h.muchii[7] = { 4,5,7 };
	h.muchii[8] = { 5,6,9 };
	GrfSt res_t = Kruskal(h,tmp,tmp,tmp);
	afis_tree(res_t);
}

void test() {
	for (int n = 100; n <= 10000; n += 100) {
		GrfSt h;
		h.v_no = n;
		h.m_no = 4*n;
		int crt_idx = 0;
		h.muchii = (MucSt*)malloc(h.m_no * sizeof(MucSt));
		for (int j = 0; j < 3 * n+1; j++) {
			int rnd1 = rand() % n + 1;
			int rnd2 = rand() % n + 1;
			while (rnd1 == rnd2)
				rnd2 = rand() % n + 1;
			h.muchii[crt_idx] = { rnd1, rnd2, rand() % 77 + 1};
			crt_idx++;
		}
		int* s = (int*)malloc(n * sizeof(int));
		FillRandomArray(s, n, 1, n, true);
		for (int i = 0; i < n - 1; i++) {
			h.muchii[crt_idx] = {s[i], s[i + 1],rand() % 77 + 1 };
			crt_idx++;
		}
		Operation op_make = G.createOperation("make_s", n);
		Operation op_uni = G.createOperation("union_s", n);
		Operation op_find = G.createOperation("find_s", n);
		Kruskal(h,op_make,op_uni,op_find);
		G.addSeries("make+union", "make_s", "union_s");
		G.addSeries("total_op_disj_sets", "make+union", "find_s");
		G.createGroup("DISJOINT SETS", "total_op_disj_sets", "make_s", "union_s", "find_s");
	}
	G.showReport();
}

int main() {
	//test_corect();
	test_corect_K();
	//test();
	return 0;
}