/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Adresare deschisa, quadratic probing, insert si search
 * 
 * Consideram alpha = factorul de umplere = n/m, unde n=nr elemente inserate si m = dimensiunea tabelei de dispersie
 * Numar de cautari pentru un element care va fi gasit este: (1/alpha)*ln(1/(1-alpha))
 * Numar de cautari pentru un element care nu va fi gasit este: 1/(1-alpha)
 * 
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define DM 9973


int qp_func(int k, int i,int dm) {
	return (k % dm+3*i+2*i*i)%dm;
}

int h_isrt(int*hT, int key,int dm) {
	int i = 0;
	do {
		int j = qp_func(key, i,dm);
		if (hT[j] == NULL) {
			hT[j] = key;
			return j;
		}
		else
			i++;
	} while (i != dm);
	return -1;
}


int h_src(int* hT, int key, int* acc, int dm) {
	int i = 0;
	int j;
	do {
		(*acc)=(*acc)+1;
		j = qp_func(key, i,dm);
		if (hT[j] == key)
			return j;
		i++;
	} while (hT[j] != NULL && i != dm);
	return -1;
}

void showht(int* a,int dm) {
	
	for (int i = 0; i < dm; i++) {
		printf("%-5d ", a[i]);
	}
	printf("\n");
}

void testing() {
	int* a;
	int* hT;
	printf("Factor de umplere    Efort mediu gasite    Efort maxim gasite    Efort mediu negasite    Efort maxim negasite\n");
	double fact_umpl[] = { 0.8,0.85,0.90,0.95,0.99 };
	for (int l = 0; l < 5; l++) {
		double fin_found_medium = 0;
		double fin_found_max = 0;
		double fin_notFound_medium = 0;
		double fin_notFound_max = 0;
		for (int z = 0; z < 5; z++) {
			a = (int*)calloc(DM, sizeof(int));
			hT = (int*)calloc(DM, sizeof(int));
			double fact = 0;
			int nr_pos_occupied = 0;
			FillRandomArray(a, DM, 1, 40000, true, 0);
			double temp=0;
			while (temp <= fact_umpl[l]){
				h_isrt(hT, a[nr_pos_occupied],DM);
				nr_pos_occupied++;
				temp = (double)(nr_pos_occupied+1) / DM;
			} 


			int m = 3000;

			//GASITE//

			int* indexes_of_a = (int*)malloc((m / 2) * sizeof(int));
			FillRandomArray(indexes_of_a, m / 2, 0, nr_pos_occupied-1, true, 0);
			double found_medium = 0;//efort mediu gasite
			int found_max = 0;//efort maxim gasite
			for (int i = 0; i < m / 2; i++) {

				int acc = 0;
				int err = 0;
				h_src(hT, a[indexes_of_a[i]], &acc,DM);
				if (found_max < acc) {
					found_max = acc;
				}
				found_medium = found_medium + acc;
			}
			found_medium = found_medium / (m / 2);
			free(a);
			free(indexes_of_a);

			//NEGASITE//

			a = (int*)malloc((m / 2) * sizeof(int));
			FillRandomArray(a, m / 2, 40001, 50000, true, 0);
			double notFound_medium = 0;
			int notFound_max = 0;
			for (int i = 0; i < m / 2; i++) {
				int acc = 0;
				h_src(hT, a[i], &acc,DM);
				if (notFound_max < acc)
					notFound_max = acc;
				notFound_medium = notFound_medium + acc;
			}
			notFound_medium = notFound_medium / (m / 2);
			free(a);
			fin_found_medium += found_medium;
			fin_found_max = fin_found_max + (double)found_max;
			fin_notFound_medium += notFound_medium;
			fin_notFound_max = fin_notFound_max + (double)notFound_max;

		}
		fin_found_medium /=5;
		fin_found_max /= 5;
		fin_notFound_max /= 5;
		fin_notFound_medium /= 5;
		printf("%10.2lf     %18lf      %18lf    %20lf    %20lf\n", fact_umpl[l], fin_found_medium, fin_found_max, fin_notFound_medium, fin_notFound_max);
	}

}

void test_corect() {
	int dm = 17;
	int* hT = (int*)calloc(dm, sizeof(int));
	int vals[] = {18,94,134,175,136,199,123,148,143,174,120,135,54,62,116,29,107,195,200,304};
	for (int i = 0; i < dm; i++) {
		printf("%-5d ", i);
	}
	printf("\n");
	double f = 0.95;
	int sz = 0;
	while((double)(sz+1) / dm <= f)
	{
		int d = h_isrt(hT, vals[sz],dm);
		printf("\nInseram pe %d la pozitia %d\n", vals[sz], d);
		showht(hT,dm);
		if(d!=-1)
			sz++;
	}
	printf("\n");
	for (int k = 0; k < sz; k++) {
		int acc = 0;
		printf("Pozitia lui %d este %d.\n", vals[k], h_src(hT, vals[k], &acc,dm));
		printf("Se fac %d accesari.\n\n", acc);
	}

	int k = 999;
	int acc = 0;
	printf("Pozitia lui %d este %d.\n", k, h_src(hT, k, &acc,dm));
	printf("Se fac %d accesari.\n\n", acc);



}

int main() {
	//testing();
	test_corect();
	return 0;
}