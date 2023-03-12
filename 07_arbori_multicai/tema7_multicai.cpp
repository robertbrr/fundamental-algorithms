/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Modalitati de reprezentare a unui arbore 
 * Pentru R2 am folosit o structura pentru un nod care contine cheia nodului, cati copii are si o un vector de copii care sunt tot de tip nod
 * Pentru R3 am folosit o structura de tip nod care cuprinde cheia nodului, copilul sau din stanga si fratele din dreapta
 * In prima transformare se parcurge vectorul de parinti de 4 ori deci complexitatea este O(4n) = O(n)
 * In a doua transformare se va apela functia recursiv pentru fiecare nod, complexitatea fiind tot liniara.
 *
 **/


#include <stdio.h>
#include <stdlib.h>

typedef struct nod {
	int value;
	int kid_cnt;
	struct nod** kids;
}NodeRep2;

typedef struct nod2 {
	int value;
	struct nod2* lft_kid;
	struct nod2* rgth_bro;
}NodeRep3;

NodeRep2* init_NodeRep2(int value) {
	NodeRep2* p = (NodeRep2*)calloc(1, sizeof(NodeRep2));
	p->value = value;
	p->kid_cnt = 0;
	p->kids = NULL;
	return p;
}

NodeRep3* init_NodeRep3(int value) {
	NodeRep3* p = (NodeRep3*)malloc(sizeof(NodeRep3));
	p->value = value;
	p->lft_kid = NULL;
	p->rgth_bro = NULL;
	return p;
}

NodeRep2* turn_r1_to_r2(int* s, int s_size) {
	NodeRep2** auxArray = (NodeRep2**)calloc(s_size+1, sizeof(NodeRep2));
	NodeRep2* root = NULL;
	for (int i = 1; i < s_size + 1; i++) {
		auxArray[i] = init_NodeRep2(i);
	}
	for (int i = 1; i < s_size+1; i++) {
		if (s[i] != -1)
			(auxArray[s[i]]->kid_cnt)++;//adunam "de cate ori este parinte" (cati copii are)
		else
			root = auxArray[i];//daca nu are parinti e radacina
	}
	for (int i = 1; i < s_size+1; i++) {
		auxArray[i]->kids = (NodeRep2**)calloc(auxArray[i]->kid_cnt, sizeof(auxArray[i]));
		auxArray[i]->kid_cnt = 0;
	}
	for (int i = 1; i < s_size + 1; i++)
		if (s[i] != -1)
			auxArray[s[i]]->kids[(auxArray[s[i]]->kid_cnt)++] = auxArray[i];
	return root;

}

NodeRep3* turn_r2_to_r3(NodeRep2* rootRep2) {
	if (rootRep2 == NULL) return NULL;
	NodeRep3* root = init_NodeRep3(rootRep2->value);
	if (rootRep2->kid_cnt > 0) {
		root->lft_kid = turn_r2_to_r3(rootRep2->kids[0]);
		if (rootRep2->kid_cnt > 1) {
			root->lft_kid->rgth_bro = turn_r2_to_r3(rootRep2->kids[1]);
			NodeRep3* aux_nd = root->lft_kid;
			for (int i = 2; i < rootRep2->kid_cnt; i++) {
				aux_nd = aux_nd->rgth_bro;
				aux_nd->rgth_bro = turn_r2_to_r3(rootRep2->kids[i]);
			}
		}
		else root->lft_kid->rgth_bro = NULL;
	}
	return root;
}

void print_rprz_R1(int* s, int s_size, int lookingFor,int mt_spaces) {
	for (int i = 1; i < s_size+1; i++)
		if (s[i] == lookingFor) {
			for (int i = 0; i < mt_spaces; i++)
				printf("\t");
			printf("%d\n", i);
			print_rprz_R1(s, s_size, i, mt_spaces +1);
		}
}

void print_rprz_R2(NodeRep2* root,int mt_spaces) {
	if (root == NULL) return;
	printf("%d\n",root->value);
	for (int i = 0; i < root->kid_cnt; i++) {
		for (int j = 0; j <= mt_spaces; j++)
			printf("\t");
		print_rprz_R2(root->kids[i], mt_spaces + 1);
	}
}

void print_rprz_R3(NodeRep3* root, int mt_spaces) {
	if (root == NULL) return;
	for (int i = 0; i < mt_spaces; i++)
		printf("\t");
	printf("%d\n", root->value);
	print_rprz_R3(root->lft_kid, mt_spaces + 1);
	print_rprz_R3(root->rgth_bro, mt_spaces);
}


void test(int *a, int n) {
	puts("Rep R1:");
	print_rprz_R1(a, n, -1, 0);
	printf("\n\n");

	puts("Rep R2:");
	NodeRep2* rootRep2 = turn_r1_to_r2(a, n);
	print_rprz_R2(rootRep2, 0);
	printf("\n\n");

	puts("Rep R3:");
	NodeRep3* rootRep3 = turn_r2_to_r3(rootRep2);
	print_rprz_R3(rootRep3,0);

}


int main() {
	int a[] = { 0,-1,1,1,2,2,2,3,3,3,5,5,7,9,9 };
	int na = sizeof(a) / sizeof(a[0]) - 1;
	int b[] = {0,2,7,5,2,7,7,-1,5,2 };
	int nb = sizeof(b) / sizeof(b[0]) - 1;
	puts("----------TEST 1----------");
	test(a, na);
	puts("----------TEST 2----------");
	test(b, nb);
	return 0;
}