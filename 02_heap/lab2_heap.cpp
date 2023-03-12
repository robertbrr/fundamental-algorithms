/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Compararea metodelor top down si bottom up ale construirii unui Heap 
 * 
 * Asa cum reiese si din grafice, metoda de construire bottom up a unui heap este mai eficienta decat cea top down.
 * Pentru a construi un Heap bottom up, se apeleaza Max-Heapify incepand de la ultimul nod care nu este frunza, deoarece frunzele sunt deja Heap-uri.
 * Complexitatea pentru bottom up este O(n).
 * 
 * In metoda top down, se porneste de la un heap gol, si elementele se insereaza rand pe rand pe ultima pozitie.
 * Complexitatea pentru top down este O(nlogn).
 * 
 **/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler p("lab_2_heap");


void maxheapify(int* s, int i, int size) {
	Operation bttm_up = p.createOperation("operatii_bttm_up", size);
	int L = 2 * i + 1 ;
	int R = 2 * i + 2;
	int lrg=i;
	if (L < size && s[L] > s[lrg])
		lrg = L;
	if (R < size && s[R] > s[lrg])
		lrg = R;
	bttm_up.count(2);
	if (lrg != i) {
		int temp;
		temp = s[lrg];
		s[lrg] = s[i];
		s[i] = temp;
		bttm_up.count(3);
		maxheapify(s, lrg, size);
	}

}

void create_heap_bttm_up(int* s, int size) {
	for (int i = size/2 - 1; i >= 0; i--)
		maxheapify(s, i, size);

}

void afis(int* a, int size) {
	for (int i = 0; i < size; i++)
		printf("%d ", a[i]);
	printf("\n");
}

void bttm_up_check() {
	int a[] = { 23, 4, 12, 55, 9, 11, 7, 45 };
	int size = sizeof(a)/sizeof(int);
	puts("Inainte de construire (heap - bottom up):");
	afis(a, size);
	puts("Dupa construire: ");
	create_heap_bttm_up(a, size);
	afis(a, size);
	printf("\n");
}

void heapsort(int* s, int size) {
	create_heap_bttm_up(s, size);
	int heap_size = size;
	for (int i = size - 1; i > 0; i--) {
		int tmp = s[i];
		s[i] = s[0];
		s[0] = tmp;
		heap_size--;
		maxheapify(s, 0, heap_size);
	}
}

void heap_sort_check() {
	int a[] = { 4, 12, 55, 9, 11, 7, 45 };
	int size = sizeof(a) / sizeof(int);
	puts("Inainte de sortare: ");
	afis(a, size);
	puts("Dupa sortare (heap-sort):");
	heapsort(a, size);
	afis(a, size);
	printf("\n");
}

int getParent(int i) {
	int p = (i - 1) / 2;
	return p;
}

void heap_increase_key(int*s, int val, int i,int s_size){
	s[i] = val;
	p.countOperation("operatii_tp_dwn", s_size, 2);
	while (i >= 1 && s[getParent(i)] < s[i]) {
		int temp = s[getParent(i)];
		s[getParent(i)] = s[i];
		s[i] = temp;
		p.countOperation("operatii_tp_dwn", s_size, 3);
		i = getParent(i);
		p.countOperation("operatii_tp_dwn", s_size, 1);
	}
	
}
void heap_insert(int* s, int val, int* h_size, int s_size) {
	(*h_size) = (*h_size) + 1;
	//p.countOperation("operatii_tp_dwn", s_size, 1);
	heap_increase_key(s, val, (*h_size) - 1,s_size);
}

void create_heap_top_down(int *s, int s_size) {
	Operation tp_dwn = p.createOperation("operatii_tp_dwn", s_size);
	int h_size = 1;
	//tp_dwn.count();
	for (int i = 1; i < s_size; i++)
		heap_insert(s, s[i], &h_size,s_size);

}

void top_down_check() {
	int a[] = { 23, 4, 12, 55, 9, 11, 7, 45 };
	int size = sizeof(a) / sizeof(int);
	puts("Inainte de construire (heap - top down):");
	afis(a, size);
	puts("Dupa construire: ");
	create_heap_top_down(a, size);
	afis(a, size);
	printf("\n");
}

void average_test() {
	for (int k = 0; k < 5; k++) {
		for (int i = 100; i <= 10000; i = i + 100) {
			int* s = (int*)malloc(i * sizeof(int));
			int* s_saved = (int*)malloc(sizeof(int)*i);
			FillRandomArray(s, i);
			memmove(s_saved, s, i*sizeof(int));
			create_heap_top_down(s, i);
			create_heap_bttm_up(s_saved, i);
			free(s);
			free(s_saved);
		}
	}
	p.divideValues("operatii_bttm_up", 5);
	p.divideValues("operatii_tp_dwn", 5);
	p.createGroup("OPERATII_total", "operatii_bttm_up", "operatii_tp_dwn");
	p.showReport();
}

int main() {
	bttm_up_check();
	heap_sort_check();
	top_down_check();
	//average_test();
}