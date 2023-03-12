/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Comparatie Quicksort si Heapsort
 * 
 * Desi ambii algorittmi au complexitate O(nlogn),asa cum reiese din grafice, in cazul mediu 
 * Quicksort efectueaza mai putine atribuiri si comparatii, deci facand mai putine operatii decat HeapSort.
 * 
 * Analiza best si worst pentru Quicksort
 * Cazul worst este atunci cand vectorul este sortat, iar unul dintre capetele acestuia este luat ca pivot, si dupa partitionare, toate elementele
 * raman ori toate in stanga, ori toate in dreapta pivotului.
 * Complexitatea este O(n^2)
 * 
 * Cazul best este un vector sortat, la care se alege ca si pivot elementul din mijloc.
 * Complexitatea este O(nlogn).
 *	
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

Profiler p("lab3_quick");

int best_sw = 0;

int part(int* s, int i1, int i2,int s_size) {
	Operation attr_quick = p.createOperation("attr_quickSort",s_size);
	Operation comp_quick = p.createOperation("comp_quickSort", s_size);
	if (best_sw == 1) {
		int aux = s[(i1 + i2) / 2];
		s[(i1 + i2) / 2] = s[i2];
		s[i2] = aux;
		attr_quick.count(3);
	}
	int pvt = s[i2];
	attr_quick.count();
	int i = i1 - 1;
	for (int j = i1; j <= i2 - 1; j++) {
		comp_quick.count();
		if (s[j] <= pvt) {

			i++;
			int tmp = s[j];
			s[j] = s[i];
			s[i] = tmp;
			attr_quick.count(3);
		}
	}
	int tmp = s[i2];
	s[i2] = s[i+1];
	s[i+1] = tmp;
	attr_quick.count(3);
	return i + 1;
}

void quick_sort(int* s, int i1, int i2,int s_size) {
	if (i1 < i2) {
		int new_idx= part(s, i1, i2,s_size);
		quick_sort(s, i1, new_idx - 1,s_size);
		quick_sort(s, new_idx + 1, i2,s_size);
	}
}

void afis(int* a, int size) {
	for (int i = 0; i < size; i++)
		printf("%d ", a[i]);
	printf("\n");
}

void check_qs() {
	int s[] = { 4,5,-12,88,19,30,-34,71 };
	int s_size = sizeof(s) / sizeof(s[0]);
	puts("Sirul inainte de sortare:");
	afis(s, s_size);
	puts("Sirul dupa Quick-Sort");
	quick_sort(s, 0, s_size-1,s_size);
	afis(s,s_size);
}

int partRand(int* s, int i1, int i2,int s_size) {
	srand(time(NULL));
	int i = rand() % (i2 - i1) + i1;
	int tmp = s[i];
	s[i] = s[i2];
	s[i2] = tmp;
	return part(s, i1, i2,s_size);
}

int rand_select(int* s, int i1,int i2,int i,int s_size) {
	if (i1 == i2)
		return s[i1];
	int piv = partRand(s, i1, i2,s_size);
	int k = piv - i1 + 1;
	if (i == k) {
		return s[piv];
	}
	else if (i < k)
		return rand_select(s, i1, piv - 1, i,s_size);
	else
		return rand_select(s, piv + 1, i2, i - k,s_size);
}

void check_rs() {
	int s[] = { 4,5,-12,88,19,30,-34,71 };
	int s_size = sizeof(s) / sizeof(s[0]);
	afis(s, s_size);
	int k = 4;
	printf("Elementul %d din sirul sortat este: % d\n",k,rand_select(s, 0, s_size - 1, k,s_size));
	
}

void maxheapify(int* s, int i, int size,int s_size) {

	Operation attr_heap = p.createOperation("attr_heap", s_size);
	Operation comp_heap = p.createOperation("comp_heap", s_size);
	int L = 2 * i + 1;
	int R = 2 * i + 2;
	int lrg = i;
	if (L < size && s[L] > s[lrg])
		lrg = L;
	if (R < size && s[R] > s[lrg])
		lrg = R;
	comp_heap.count(2);
	if (lrg != i) {
		int temp;
		temp = s[lrg];
		s[lrg] = s[i];
		s[i] = temp;
		attr_heap.count(3);
		maxheapify(s, lrg, size,s_size);
	}

}

void create_heap_bttm_up(int* s, int size) {
	for (int i = size / 2 - 1; i >= 0; i--)
		maxheapify(s, i, size,size);

}
void heapsort(int* s, int size) {
	create_heap_bttm_up(s, size);
	Operation attr_heap = p.createOperation("attr_heap", size);
	Operation comp_heap = p.createOperation("comp_heap", size);
	int heap_size = size;
	for (int i = size - 1; i > 0; i--) {
		int tmp = s[i];
		s[i] = s[0];
		s[0] = tmp;
		attr_heap.count(3);
		heap_size--;
		maxheapify(s, 0, heap_size,size);
	}
}

void average_test() {
	for (int k = 0; k < 5; k++) {
		for (int i = 100; i <= 10000; i = i + 100) {
			int* s = (int*)malloc(i * sizeof(int));
			int* s_saved = (int*)malloc(sizeof(int) * i);
			FillRandomArray(s, i);
			memmove(s_saved, s, i * sizeof(int));
			quick_sort(s_saved, 0, i - 1, i);
			heapsort(s, i);
			free(s);
			free(s_saved);
		}
	}
	p.divideValues("attr_heap", 5);
	p.divideValues("comp_heap", 5);
	p.addSeries("op_heap", "attr_heap", "comp_heap");

	p.divideValues("attr_quickSort", 5);
	p.divideValues("comp_quickSort", 5);
	p.addSeries("op_quickSort", "attr_quickSort", "comp_quickSort");

	p.createGroup("ATRIBUIRI_total", "attr_heap", "attr_quickSort");
	p.createGroup("COMPERATII_total", "comp_heap", "comp_quickSort");
	p.createGroup("OPERATII_total", "op_heap", "op_quickSort");

	p.showReport();
}

void worst_test() {
		for (int i = 100; i <= 10000; i = i + 100) {
			int* s = (int*)malloc(i * sizeof(int));
		
			FillRandomArray(s, i,0,50000,false,ASCENDING);
			
			quick_sort(s, 0, i - 1, i);
			free(s);
		}
	
	p.addSeries("op_quickSort", "attr_quickSort", "comp_quickSort");
	p.showReport();
}


void best_test() {
	best_sw = 1;
	for (int i = 100; i <= 10000; i = i + 100) {
		int* s = (int*)malloc(i * sizeof(int));
		FillRandomArray(s, i, 0, 50000, false, ASCENDING);
		quick_sort(s, 0, i - 1, i);
		free(s);
	}

	p.addSeries("op_quickSort", "attr_quickSort", "comp_quickSort");
	p.showReport();
	best_sw = 0;
}


int main() {
	check_qs();
	printf("\n");
	check_rs();
	//average_test();
	//worst_test();
	//best_test();
	return 0;

}