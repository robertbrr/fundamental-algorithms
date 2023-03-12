/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Compararea algoritmilor de sortare Bubble Sort, Insertion Sort si Selection Sort
 *
 * Insertion si Bubble sort sunt algoritmi stabili.
 * Selection Sort nu este stabil.
 * 
 * In cazul mediu, toti cei 3 algoritmi au complexitate O(n^2). Din grafice rezulta faptul ca BubbleSort este cel mai putin 
 * eficient, efectuand mai mult decat dublul numarului de operatii in raport cu selection si insertion, care ambele efectueaza
 * un numar similar de operatii in total
 * 
 * In cazurile nefavorabile, complexitatea algoritmilor ramane identica cu cea din cazul mediu
 * Numarul de operatii efectuate de SelectionSort este cel mai mic, similar cu cel din cazurile average, in timp ce 
 * BubbleSort este in continuare cel mai putin eficient.
 * 
 * In cel mai bun caz (sir deja sortat), atat BubbleSort cat si SelectionSort nu efectueaza nicio asignare
 * Bubble Sort efectueaza (n-1) comparatii, deci are complexitate O(n)
 * Insertion Sort face 3(n-1) operatii in total, deci are complexitatea O(n)
 * Selection Sort are complexitatea O(n^2)
 * 
 **/



#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler p("lab_sort");

void insertionsort(int* s, int n) {
	Operation compare = p.createOperation("insertion_compare", n);
	Operation assign = p.createOperation("insertion_assign", n);
	int val;
	for (int i = 1; i < n; i++) {
		val = s[i];
		assign.count();
		int j = i - 1;
		compare.count();
		while (j >= 0 && s[j] > val) {
			compare.count();
			assign.count();
			s[j + 1] = s[j];
			j--;
		}
		s[j + 1] = val;
		assign.count();
	}
}

void selectionsort(int* s, int n) {
	Operation compare = p.createOperation("selection_compare", n);
	Operation assign = p.createOperation("selection_assign", n);

	for (int i = 0; i < n - 1;i++) {
		int min_idx = i;
		for (int j = i + 1; j < n; j++) {
			compare.count();
			if (s[j] < s[min_idx]) {
				min_idx = j;

			}
		}
		
		if (i != min_idx) {
			int temp = s[min_idx];
			s[min_idx] = s[i];
			s[i] = temp;
			assign.count(3);
		}

	}
}

void bubblesort(int* s, int n) {
	int ok;
	int lim = 0;

	Operation compare = p.createOperation("bubblesort_compare",n);
	Operation assign = p.createOperation("bubblesort_assign", n);

	do {
		 ok = 1;
		for (int i = 0; i < n-1-lim; i++) {

			compare.count();
			if (s[i] > s[i + 1]) {
				int temp = s[i];
				s[i] = s[i + 1];
				s[i + 1] = temp;
				ok = 0;
				assign.count(3);
			}
		}
		lim++;
	} while (ok == 0);
}


void afis(int* s, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", s[i]);
	}
	printf("\n");
}

void check_bubblesort() {
	int s[] = { 10, 2, 7, 9, 11, 29, 1 };
	int n = sizeof(s) / sizeof(int);
	puts("Inainte de sortare:");
	afis(s, n);
	bubblesort(s,n);
	puts("Dupa sortare (bubble-sort):");
	afis(s, n);
}

void check_insertionsort() {
	int s[] = { 100, 31, -7, 21, 139, 64, 11, 23 };
	int n = sizeof(s) / sizeof(int);
	puts("Inainte de sortare:");
	afis(s, n);
	insertionsort(s, n);
	puts("Dupa sortare (insertion-sort):");
	afis(s, n);
}

void check_selectionsort() {
	int s[] = { -12, 23, 54, -11, 99, 3, 11};
	int n = sizeof(s) / sizeof(int);
	puts("Inainte de sortare:");
	afis(s, n);
	selectionsort(s, n);
	puts("Dupa sortare (selection-sort):");
	afis(s, n);
}



void average_test() {
	for (int k = 0; k < 5; k++) {
		for (int i = 100; i <= 10000; i =i + 100)
		{
			int* s = (int*)malloc(i*sizeof(int));
			int* s_saved = (int*)malloc(i*sizeof(int));
			FillRandomArray(s, i);
			memmove(s_saved, s, i*sizeof(int));
			bubblesort(s, i);
			memmove(s, s_saved, i*sizeof(int));
			selectionsort(s, i);
			memmove(s, s_saved, i*sizeof(int));
			insertionsort(s, i);
			free(s_saved);
			free(s);
		}
	}
	p.divideValues("bubblesort_compare", 5);
	p.divideValues("bubblesort_assign", 5);
	p.addSeries("operatii_bubblesort", "bubblesort_compare", "bubblesort_assign");

	p.divideValues("selection_compare", 5);
	p.divideValues("selection_assign", 5);
	p.addSeries("operatii_selection", "selection_compare", "selection_assign");

	p.divideValues("insertion_compare", 5);
	p.divideValues("insertion_assign", 5);
	p.addSeries("operatii_insertion", "insertion_compare", "insertion_assign");

	p.createGroup("total_comparatii", "bubblesort_compare", "selection_compare", "insertion_compare");
	p.createGroup("total_atribuiri", "bubblesort_assign", "selection_assign", "insertion_assign");
	p.createGroup("total", "operatii_bubblesort", "operatii_selection", "operatii_insertion");
	p.createGroup("atribuiri_selection", "selection_assign");
	p.showReport();
}

void best_test() {
	
		for (int i = 100; i <= 10000; i = i + 100)
		{
			int* s = (int*)malloc(i * sizeof(int));
			int* s_saved = (int*)malloc(i * sizeof(int));
			FillRandomArray(s, i,0,50000,false,ASCENDING);
			memmove(s_saved, s, i * sizeof(int));
			bubblesort(s, i);
			memmove(s, s_saved, i * sizeof(int));
			selectionsort(s, i);
			memmove(s, s_saved, i * sizeof(int));
			insertionsort(s, i);
			free(s_saved);
			free(s);
		}
		p.addSeries("operatii_bubblesort", "bubblesort_compare", "bubblesort_assign");
		p.addSeries("operatii_selection", "selection_compare", "selection_assign");
		p.addSeries("operatii_insertion", "insertion_compare", "insertion_assign");

		p.createGroup("comparatii_insert_bubblesrt", "insertion_compare","bubblesort_compare");
		p.createGroup("total_insert_bubble", "operatii_bubblesort", "operatii_insertion");
		p.createGroup("total_comparatii", "bubblesort_compare", "selection_compare", "insertion_compare");
		p.createGroup("total_atribuiri", "bubblesort_assign", "selection_assign", "insertion_assign");
		p.createGroup("total", "operatii_bubblesort", "operatii_selection", "operatii_insertion");
		p.showReport();
	
}

void worst_test() {

	for (int i = 100; i <= 10000; i = i + 100)
	{
		int* s = (int*)malloc(i * sizeof(int));
		int* s_saved = (int*)malloc(i * sizeof(int));
		FillRandomArray(s, i, 0, 50000, false, DESCENDING);
		memmove(s_saved, s, i * sizeof(int));
		bubblesort(s, i);
		memmove(s, s_saved, i * sizeof(int));
		insertionsort(s, i);

		FillRandomArray(s, i, 0, 50000, false, ASCENDING);
		int tmp = s[0];
		memmove(s, s+1, (i - 1) * sizeof(int));
		s[i - 1] = tmp;
		selectionsort(s, i);
		free(s_saved);
		free(s);
	}
	p.addSeries("operatii_bubblesort", "bubblesort_compare", "bubblesort_assign");
	p.addSeries("operatii_selection", "selection_compare", "selection_assign");
	p.addSeries("operatii_insertion", "insertion_compare", "insertion_assign");
	p.createGroup("asignari_selection", "selection_assign");
	p.createGroup("total_comparatii", "bubblesort_compare", "selection_compare", "insertion_compare");
	p.createGroup("total_atribuiri", "bubblesort_assign", "selection_assign", "insertion_assign");
	p.createGroup("total", "operatii_bubblesort", "operatii_selection", "operatii_insertion");
	p.showReport();

}

int main() {
	check_bubblesort();
	check_insertionsort();
	check_selectionsort();
	//worst_test();
	//average_test();
	//best_test();
	return 0;
}