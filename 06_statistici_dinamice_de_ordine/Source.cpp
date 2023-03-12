/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Evaluarea operatiilor de management pentru un arbore de statistica de ordine
 *
 * Operatia de build are complexitatea de O(n);
 *
 * Operatiile de select si delete au complexitatea log(n) deoarece de fiecare data se verifica in care parte
 * a arborelui trebuie sa se afle elementul cautat, si se apeleaza recursiv pe jumatate din arbore pana cand se gaseste
 * sau pana cand se iese din range-ul elementelor si ajunge la un NULL
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#include <time.h>
Profiler t("tema_7af");

typedef struct nod_str {
	int val;
	int size;
	struct nod_str* left;
	struct nod_str* right;
}NodeOS;


NodeOS* create_OS_tree(int first_no, int last_no, Operation op_assgn, Operation op_cmp) {
	op_cmp.count();
	if (first_no > last_no) return NULL;
	op_assgn.count();
	int in_between = (first_no + last_no) / 2;
	NodeOS* rootOS = (NodeOS*)malloc(sizeof(NodeOS));
	rootOS->val = in_between;
	rootOS->size = 1;
	rootOS->left = create_OS_tree(first_no, in_between - 1, op_assgn, op_cmp);
	rootOS->right = create_OS_tree(in_between + 1, last_no, op_assgn, op_cmp);
	op_assgn.count(4);
	op_cmp.count(2);
	if (rootOS->left != NULL)
	{
		rootOS->size = rootOS->size + rootOS->left->size;
		op_assgn.count();
	}
	if (rootOS->right != NULL) {
		rootOS->size = rootOS->size + rootOS->right->size;
		op_assgn.count();
	}
	return rootOS;
}

NodeOS* select_OS_tree(NodeOS* rootOS, int i, Operation op_assgn, Operation op_cmp) {
	op_cmp.count();
	if (rootOS == NULL)return NULL;
	int aux_sz = 1;
	op_assgn.count();
	op_cmp.count();
	if (rootOS->left != NULL)
	{
		op_assgn.count();
		aux_sz = aux_sz + rootOS->left->size;
	}
	op_cmp.count();
	if (aux_sz == i)
		return rootOS;
	else if (i < aux_sz) {
		op_cmp.count();
		return select_OS_tree(rootOS->left, i, op_assgn, op_cmp);
	}
	else {
		op_cmp.count();
		return select_OS_tree(rootOS->right, i - aux_sz, op_assgn, op_cmp);
	}

}

NodeOS* min_node_from_right_subtree(NodeOS* rootOS, Operation op_cmp)
{
	op_cmp.count();
	if (rootOS == NULL)
		return NULL;
	op_cmp.count();
	if (rootOS->left != NULL)
		return min_node_from_right_subtree(rootOS->left, op_cmp);
	else
		return rootOS;

}

//functia deleteKey pentru BST preluata de la SDA, adaptata pentru un index si pentru a actualiza campul size in care se sterge un nod
NodeOS* delete_OS_tree(NodeOS* rootOS, int index, int* inRange, Operation op_assgn, Operation op_cmp)
{
	op_cmp.count();
	if (rootOS == NULL) {
		*inRange = 0;
		op_assgn.count();
		return NULL;
	}
	int aux_sz = 1;
	op_assgn.count();
	op_cmp.count();
	if (rootOS->left != NULL)
	{
		op_assgn.count();
		aux_sz = aux_sz + rootOS->left->size;
	}
	NodeOS* interm_node;
	op_cmp.count();
	if (index < aux_sz) {
		rootOS->left = delete_OS_tree(rootOS->left, index, inRange, op_assgn, op_cmp);
		op_cmp.count();
		if (*inRange) {
			(rootOS->size)--;
			op_assgn.count();
		}
		op_assgn.count();
	}
	else if (index > aux_sz) {
		op_cmp.count();
		rootOS->right = delete_OS_tree(rootOS->right, index - aux_sz, inRange, op_assgn, op_cmp);
		op_cmp.count();
		if (*inRange) {
			(rootOS->size)--;
			op_assgn.count();
		}
		op_assgn.count();
	}
	else
	{
		op_cmp.count();
		op_cmp.count();
		if (rootOS->left == NULL)
		{

			interm_node = rootOS->right;
			free(rootOS);
			op_assgn.count();
			*inRange = 1;
			return interm_node;
		}
		else if (rootOS->right == NULL)
		{
			op_cmp.count();
			interm_node = rootOS->left;
			*inRange = 1;
			op_assgn.count(2);
			free(rootOS);
			return interm_node;
		}
		op_cmp.count();
		interm_node = min_node_from_right_subtree(rootOS->right, op_cmp);
		rootOS->val = interm_node->val;
		(rootOS->size)--;
		int aux_interm_node = 1;
		op_cmp.count();
		if (interm_node->left != NULL) {
			aux_interm_node = aux_interm_node + interm_node->left->size;
			op_assgn.count();
		}
		rootOS->right = delete_OS_tree(rootOS->right, aux_interm_node, inRange, op_assgn, op_cmp);
		op_assgn.count(5);
	}

	return rootOS;
}


void pretty_print_OS_tree(NodeOS* rootOS, int mt_spaces) {
	if (rootOS == NULL) return;
	for (int i = 0; i < mt_spaces; i++)
		printf("\t");
	printf("%d|%d|\n", rootOS->val, rootOS->size);
	pretty_print_OS_tree(rootOS->left, mt_spaces + 1);
	pretty_print_OS_tree(rootOS->right, mt_spaces + 1);

}

void test_average() {
	for (int i = 100; i <= 10000; i += 100) {
		Operation op_assgn_bld = t.createOperation("atr_bld", i);
		Operation op_cmp_bld = t.createOperation("cmp_bld", i);
		Operation op_assgn_slct = t.createOperation("atr_slct", i);
		Operation op_cmp_slct = t.createOperation("cmp_slct", i);
		Operation op_assgn_del = t.createOperation("atr_del", i);
		Operation op_cmp_del = t.createOperation("cmp_del", i);
		for (int r = 0; r < 5; r++) {
			NodeOS* rootOS = create_OS_tree(1, i, op_assgn_bld, op_cmp_bld);
			int saved_size = i;
			for (int l = 0; l < i; l++) {
				int elem = rand() % saved_size + 1;
				int* inRange = (int*)malloc(sizeof(int));
				NodeOS* selected = select_OS_tree(rootOS, elem, op_assgn_slct, op_cmp_slct);
				if (selected == NULL) printf("BUBA");
				rootOS = delete_OS_tree(rootOS, elem, inRange, op_assgn_del, op_cmp_del);
				saved_size--;
			}
		}
	}
	t.divideValues("atr_slct", 5);
	t.divideValues("cmp_slct", 5);
	t.divideValues("atr_del", 5);
	t.divideValues("cmp_del", 5);
	t.divideValues("atr_bld", 5);
	t.divideValues("cmp_bld", 5);
	t.addSeries("total_op_slct", "atr_slct", "cmp_slct");
	t.addSeries("total_op_del", "atr_del", "cmp_del");
	t.addSeries("total_op_bld", "atr_bld", "cmp_bld");
	t.createGroup("Operatii totale build", "total_op_bld", "atr_bld", "cmp_bld");
	t.createGroup("Operatii totale select", "total_op_slct", "atr_slct", "cmp_slct");
	t.createGroup("Operatii totale delete", "total_op_del", "atr_del", "cmp_del");
	t.showReport();
}


void test_corect_createOS() {
	Operation useless = t.createOperation("no", 1);
	int osT_size = 11;
	NodeOS* rootOS = create_OS_tree(1, osT_size, useless, useless);
	puts("Arborele initial");
	pretty_print_OS_tree(rootOS, 0);
	NodeOS* test = (NodeOS*)malloc(sizeof(test));
	int* inRange = (int*)malloc(sizeof(int));
	//FIND AND DELETE ELEM
	for (int j = 0; j < 3; j++) {
		int toFind = rand() % osT_size + 1;
		test = select_OS_tree(rootOS, toFind, useless, useless);
		if (test == NULL)
			printf("\nNu exista nodul al %d-lea\n", toFind);
		else {
			printf("\nAl %d-lea element este %d\n", toFind, test->val);
			osT_size--;
			rootOS = delete_OS_tree(rootOS, toFind, inRange, useless, useless);
			printf("\nDupa ce am sters elementul al %d-lea:\n", toFind);
			pretty_print_OS_tree(rootOS, 0);
		}
	}
	//OUT OF RANGE SELECT
	int toFind = osT_size + 15;
	test = select_OS_tree(rootOS, toFind, useless, useless);
	if (test == NULL)
		printf("\nNu exista nodul al %d-lea\n", toFind);
	else {
		printf("\nAl %d-lea element este %d\n", toFind, test->val);
		osT_size--;
	}
	//OUT OF RANGE DELETE
	rootOS = delete_OS_tree(rootOS, toFind, inRange, useless, useless);
	printf("\nDupa ce am incercat stergerea unui element care nu se afla in arbore (al %d-lea):\n", toFind);
	pretty_print_OS_tree(rootOS, 0);

}

int main() {
	srand(time(NULL));
	//test_corect_createOS();
	test_average();
	return 0;
}