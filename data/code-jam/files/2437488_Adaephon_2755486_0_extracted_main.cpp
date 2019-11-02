#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

using namespace std;



class tribe{
public: tribe(){
		}
		~tribe(){
		}

		int delta_d;
		int delta_p;
		int delta_s;

		int s;
		int n;
		int w;
		int e;
		int d;

		int attacks;
		int success;
};

void Output(int status, int case_num){
	FILE *f1;
	f1 = fopen("output_small.out", "a");
	fprintf(f1, "Case #%d: %d\n",case_num, status);
	fclose(f1);
}

int Numbers(tribe *t, int N){
	int num_attacks = 0;
	int all_attacks = 0;
	int current_day = 0;

	int *update_tribes;
	int num_update = 0;
	update_tribes = new int[N];

	int wall[1000];
	for(int i=0; i<1000; i++){
		wall[i] = 0;
	}
	int zero = 500;

	current_day = t[0].d;
	t[0].attacks = 0;
	for(int i=1; i<N; i++){
		t[i].attacks = 0;
		if(t[i].d < current_day){
			current_day = t[i].d;
		}
	}

	while(all_attacks==0){
		all_attacks = 1;
		for(int i=0; i<N; i++){
			if(t[i].attacks<t[i].n){
				if(t[i].d == current_day){
					//process attacks
					t[i].success = 0;
				
					for(int j=zero+t[i].w; j<zero+t[i].e; ++j){
						if(t[i].s>wall[j]){
							++num_attacks;
							t[i].success = 1;
							j = zero+t[i].e;
						}
					}
					update_tribes[num_update] = i;
					++num_update;
				}
				all_attacks = 0;
			}
		}
		for(int i=0; i<num_update; i++){
			int ind = update_tribes[i];
			if(t[ind].success == 1){
				for(int j=zero+t[ind].w; j<zero+t[ind].e; ++j){
					printf("%d ", j);
					if(t[ind].s>wall[j]){
						wall[j] = t[ind].s;
					}
				}
			}
			t[ind].w += t[ind].delta_p;
			t[ind].e += t[ind].delta_p;
			t[ind].s += t[ind].delta_s;
			t[ind].d += t[ind].delta_d;
			t[ind].attacks++;
		}
		num_update = 0;
		current_day = 1000000000;
		for(int i=0; i<N; i++){
			if(t[i].attacks<t[i].n){
				if(t[i].d < current_day){
					current_day = t[i].d;
				}
			}
		}
	}


	delete[] update_tribes;
	return num_attacks;
}

int main(){
	int num_cases;
	int N;	//num tribes
	int di, ni, wi, ei, si, delta_d, delta_p, delta_s;
	FILE *f1;
	tribe *tribes;

	f1 = fopen("C-small-attempt0.in", "r");
	fscanf(f1, "%d\n", &num_cases);
	for(int i=0; i<num_cases; i++){
		fscanf(f1, "%d\n", &N);
		tribes = new tribe[N];
		for(int j=0; j<N; j++){
			fscanf(f1, "%d %d %d %d %d %d %d %d\n", &tribes[j].d, &tribes[j].n, &tribes[j].w, &tribes[j].e, &tribes[j].s, &tribes[j].delta_d, &tribes[j].delta_p, &tribes[j].delta_s);
		}
		Output(Numbers(tribes, N),i+1);
		delete[] tribes;
	}
	fclose(f1);



	return 0;
}