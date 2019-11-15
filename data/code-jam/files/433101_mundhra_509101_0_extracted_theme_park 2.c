#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_GRPS 100000000

static long long money_made(int num_trips, int num_seats, int *grps, int num_grps)
{
	long long money = 0;
	int next = 0, num;
	int trip;

	for (trip = 0; trip < num_trips; trip++) {
		num = 0;
		if (num_grps != 1) {	
			while (num + grps[next] <= num_seats) { 	
				num += grps[next];
				next++;
				if (next >= num_grps) {
					next = next % num_grps;
				}
			}
		} else {
			num = grps[next];
		}
		money += num;
	}

	return money;
}

static int process_each_input(FILE *fp, int num_inputs, int *grps)
{
	int i;
	int ret = 0;


	for (i = 0; i < num_inputs; i++) {
		int num_trips;
		int num_seats;
		int num_grps;
		int j;
		
		fscanf(fp, "%d %d %d", &num_trips, &num_seats, &num_grps);
		for (j = 0; j < num_grps; j++) {
			fscanf(fp, "%d ", &grps[j]);
		}

		printf("Case #%d: %lld\n", i+1, money_made(num_trips, num_seats, grps, num_grps));
	}
	
	return 0;
}	

static int process_inputs(int argc, char **argv)
{
	char *fname;
	FILE *fp;
	int ret = 0;
	int num_inputs;
	int *grps;

	/* Check if we have right number of arguments*/
	if (argc != 2) {
		printf("Insufficent arguments\n");
		return -EINVAL;
	}

	/* Open the file, get fp */
	fname = argv[1];
	fp = fopen(fname, "r");
	
	if (NULL == fp) {
		perror("open");
		return -ENODATA;
	} 
	
	/*Get no of inputs, and process each one of them*/	
	ret = fscanf(fp, "%d", &num_inputs);
	if (ret != 1) {
		printf("Error while readinf from file %s\n", fname);
		return -EIO;
	}
	
	grps = (int *) malloc (sizeof(int) * MAX_GRPS);
	if (!grps) {
		printf("No memory");
		return -ENOMEM;
	}

	ret = process_each_input(fp, num_inputs, grps);
	
	free(grps);
	return ret;
}


int main (int argc, char **argv)
{
	int ret;

	ret = process_inputs(argc, argv);
	
	return ret;
}
