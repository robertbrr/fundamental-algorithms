/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Interclasare K liste ordonate care au in total N elemente
 * 
 * Complexitatea este O(n log k) deoarece se va contrui un MIN heap cu K elemente.
 * Complexitatea de a apela min Heapify va fi astfel O(log k).
 * Min Heapify se va apela de n ori deoarece lista finala va fi alcatuita dintr-un numar de n elemente.
 * 
 * In cazul in care K este constant, si n este variabil, asa cum se vede si din graficele rezultate
 * in urma apelarii procedurii test_1, complexitatea devine O(n);
 * 
 * In cazul in care N este constant (test_2), complexitatea devine logaritmica O(log k);
 *
 **/



#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
Profiler p("lab04");
#define LIM_RNG 500

typedef struct nod {
	int key;
	struct nod* next;
}Node;

Node* create_node(int key) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->key = key;
	n->next = NULL;
	return n;
}

void addToListFirst(Node** root, int key) {
	Node* n = create_node(key);
	if (*root == NULL) {
		*root = n;
		
	}
	else
	{
		n->next = *root;
		*root = n;
	}
}

void showList(Node* root) {
	for (Node* p = root; p != NULL; p = p->next)
		printf("%d ", p->key);
	printf("\n");
}

void minheapify(Node* s[], int i, int size, Operation assign_op, Operation comp_op) {
	int L = 2 * i + 1;
	int R = 2 * i + 2;
	int sml = i;
	if (L < size && s[L]->key < s[sml]->key)
		sml = L;
	if (R < size && s[R]->key < s[sml]->key)
		sml = R;
	comp_op.count();
	if (sml != i) {
		Node* temp=NULL;
		temp = s[sml];
		s[sml] = s[i];
		s[i] = temp;
		assign_op.count(3);
		minheapify(s, sml, size,assign_op,comp_op);
	}

}

void create_minHeap(Node* s[], int size, Operation assign_op, Operation comp_op) {
	for (int i = size / 2 - 1; i >= 0; i--)
		minheapify(s, i, size,assign_op,comp_op);

}

Node* interclasare_2liste(Node* rootA, Node* rootB) {
	Node* r = NULL;
	Node* idx = NULL;
	Node* p = rootA;
	Node* q = rootB;
	while (p != NULL && q != NULL) {
		if (p->key < q->key) {
			if (r == NULL) {
				r = p;
				idx = p;
			}
			else {
				idx->next = p;
				idx = idx->next;
			}
			p = p->next;
		}
		else {
			if (r == NULL) {
				r = q;
				idx = q;
			}
			else {
				idx->next = q;
				idx = idx->next;
			}
			q = q->next;
		}
	}
	if (p == NULL)
		idx->next = q;
	else if (q == NULL)
		idx->next = p;
	else idx->next == NULL;
	return r;

}

void merge(Node* f[], int f_size, Node** result_root, Node** result_tail,  Operation assign_op,Operation comp_op) {
	
	int h_size = f_size;
	create_minHeap(f, h_size,assign_op,comp_op);
	while (h_size > 1)
	{	
		comp_op.count();
		if (*result_tail != NULL) {
			(*result_tail)->next = f[0];
			(*result_tail) = f[0];
			assign_op.count(2);
		}
		else
		{
			assign_op.count(2);
			*result_root = f[0];
			*result_tail = f[0];
		}

		f[0] = f[0]->next;
		assign_op.count();
		
		comp_op.count();
		if (f[0] == NULL) {
			f[0] = f[h_size - 1];
			h_size = h_size - 1;
			assign_op.count();
		}
		minheapify(f, 0, h_size,assign_op,comp_op);
	}
	(*result_tail)->next = f[0];
	assign_op.count();
}

Node** genLists(int n, int k) {
	Node** f = (Node**)calloc(k,sizeof(Node*));
	int m = n / k;
	int rem = n - m * k ;
	for (int i = 0; i < rem; i++) {
		int* s = (int*)malloc((m+1) * sizeof(int));
		FillRandomArray(s, m+1, 0, LIM_RNG, false, DESCENDING);
		for (int j = 0; j < m +1; j++)
			addToListFirst(&f[i], s[j]);
		free(s);
	}
	for (int i = rem; i < k; i++) {
		int* s = (int*)malloc(m * sizeof(int));
		FillRandomArray(s, m, 0, LIM_RNG, false, DESCENDING);
		for (int j = 0; j < m; j++)
			addToListFirst(&f[i], s[j]);
		free(s);
	}
	return f;
}


void test_1() {
	for (int i = 100; i <= 10000; i += 100) {
		Operation assign_op5 = p.createOperation("list_assign5", i);
		Operation comp_op5 = p.createOperation("list_comp5", i);
		
		Node** f = genLists(i, 5);
		Node* res_root = NULL;
		Node* res_tail = NULL;
		merge(f, 5, &res_root, &res_tail,assign_op5,comp_op5);
		
		Operation assign_op10 = p.createOperation("list_assign10", i);
		Operation comp_op10 = p.createOperation("list_comp10", i);
		Operation assign_op100 = p.createOperation("list_assign100", i);
		Operation comp_op100 = p.createOperation("list_comp100", i);


		f = genLists(i, 10);
		res_root = NULL;
		res_tail = NULL;
		merge(f, 10, &res_root, &res_tail, assign_op10, comp_op10);

		f = genLists(i, 100);
		res_root = NULL;
		res_tail = NULL;
		merge(f, 100, &res_root, &res_tail, assign_op100, comp_op100);
	}

	p.addSeries("total_op5", "list_assign5", "list_comp5");
	p.addSeries("total_op10", "list_assign10", "list_comp10");
	p.addSeries("total_op100", "list_assign100", "list_comp100");
	p.createGroup("total","total_op5", "total_op10", "total_op100");
	p.createGroup("assign", "list_assign5", "list_assign10", "list_assign100");
	p.createGroup("compare", "list_comp5", "list_comp10", "list_comp100");
	
	p.showReport();

}

void test_2() {
	for (int k = 10; k <= 500; k += 10) {
		Operation assign_op = p.createOperation("list_assign", k);
		Operation comp_op = p.createOperation("list_comp", k);
		

		Node** f = genLists(10000, k);
		Node* res_root = NULL;
		Node* res_tail = NULL;
		merge(f, k, &res_root, &res_tail, assign_op, comp_op);
		free(f);
	}

	p.addSeries("total_op", "list_assign", "list_comp");
	
	p.createGroup("total", "total_op");
	p.createGroup("operatii", "list_assign", "list_comp");
	p.showReport();

}

void test_corect_interclasare2() {
	int n = 20;
	int k = 2;
	Node** f = genLists(n, k);
	for (int i = 0; i < k; i++)
		showList(f[i]);
	//Operation useless = p.createOperation("test", 1);
	//Node* root_rez = NULL;
	//Node* tail_rez = NULL;
	//merge(f, k, &root_rez, &tail_rez, useless, useless);
	puts("Dupa interclasare: ");
	showList(interclasare_2liste(f[0],f[1]));

}

void test_corect() {
	int n = 20;
	int k = 4;
	Node** f = genLists(n, k);
	for (int i = 0; i < k; i++)
		showList(f[i]);
	Operation useless = p.createOperation("test", 1);
	Node* root_rez = NULL;
	Node* tail_rez = NULL;
	merge(f, k, &root_rez, &tail_rez, useless, useless);
	puts("Dupa interclasare: ");
	showList(root_rez);

}

int main() {
	test_corect();
	//test_corect_interclasare2();
	//test_1();
	//test_2();
	return 0;
}