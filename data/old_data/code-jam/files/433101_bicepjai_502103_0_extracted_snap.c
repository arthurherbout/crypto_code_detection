#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct condition {
bool pwr;
bool on_off;
} cnd;

bool alltrue(cnd* bits, long N);
bool toggle( bool on_off);
void printme(cnd* bits,long N);
void copyme(cnd* bits_src, cnd* bits_dest, long N);

cnd *cur_bits, *old_bits;
bool check;

int main(int argc, char* argv[]){
	long i,j,k,l,m,cases;
	long N;
	long K;
	
	FILE *infp = fopen("input1.in","r");
    FILE *outfp = fopen("output1.out","w");
	
	fscanf(infp,"%ld",&cases);
	//printf("%ld\n",cases);
	for(m=0;m<cases;m++){
		fscanf(infp,"%ld %ld",&N,&K);
		old_bits = (cnd *) malloc(N * sizeof(cnd));
		cur_bits = (cnd *) malloc(N * sizeof(cnd));

		old_bits[0].pwr = true;
		old_bits[0].on_off = false;
		
		for(i=1;i<N;i++){
			old_bits[i].pwr = false;
			old_bits[i].on_off = false;
		}
		copyme(old_bits, cur_bits,N);

		for(l=0;l<K;l++){
			
			for(i=0;i<N;i++){
				if(i==0){
					cur_bits[i].pwr = true;
					check = toggle(old_bits[i].on_off);
					cur_bits[i].on_off = check;
					cur_bits[i+1].pwr = check;
				}else{
					if(old_bits[i].pwr==true){
						check = toggle(old_bits[i].on_off);
							cur_bits[i].on_off = check;
							if(alltrue(cur_bits,i+1)){
								cur_bits[i+1].pwr = true;
							} else
								cur_bits[i+1].pwr = false;
					} else {
						if(alltrue(cur_bits,i+1))
							cur_bits[i+1].pwr = true;
						else
							cur_bits[i+1].pwr = false;
						break;
					}
				}
			}
			//printf("%ld =>",l);
			//printf("(%ld)",j);
			//printme(cur_bits,N);
			for(j=0;j<N;j++){
				if(cur_bits[j].pwr == true && cur_bits[j].on_off == true){
					cur_bits[j+1].pwr = true;
				} else
					break;
			}			
			//printf("%ld =>",l);
			//printf("(%ld)",j);
			//printme(cur_bits,N);
			copyme(cur_bits,old_bits,N);
		}
		
		if(alltrue(cur_bits,N)) fprintf(outfp,"Case #%ld: ON\n",m+1);
		else fprintf(outfp,"Case #%ld: OFF\n",m+1);
		
		free(old_bits);
		free(cur_bits);
	}
	close(infp);
	close(outfp);
	return 0;
}

void copyme(cnd* bits_src, cnd* bits_dest, long N){
	long i;
	for(i=0;i<N;i++){
		bits_dest[i].pwr 		= 	bits_src[i].pwr;
		bits_dest[i].on_off 	= 	bits_src[i].on_off;
	}
}

bool alltrue(cnd* bits, long N){
	long i;
	for(i=0;i<N;i++)
		if(bits[i].pwr == false || bits[i].on_off == false)
			return false;
	return true;
}

bool toggle( bool on_off){
	if(on_off)
		return false;
	else
		return true;
}

void printme(cnd* bits,long N){
	long i;
 	for(i=0;i<N;i++){
		if(bits[i].pwr)
		printf("1");
		else
		printf("0");
		
		if(bits[i].on_off)
		printf("1");
		else
		printf("0");
		
		printf(" ");
	}
		printf("\n");
}