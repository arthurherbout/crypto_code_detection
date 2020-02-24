#include <stdio.h> 

#define FNAME_SIZE 30
#define ST_A 0
#define ST_B 1
#define INP_LEN 5

struct train {
	int start;
	int end;
	int station;
	struct train *next;
	struct train *prev;
};

void print_list(struct train *list)
{
	struct train *pos;

	for (pos = list->next; pos != list; pos = pos->next)
		printf("St:%d end :%d station: %d\n", pos->start, pos->end, pos->station);
}
void free_list(struct train *list)
{
	struct train *pos, *next_pos;
	for (pos = list->next; pos != list; pos = next_pos) {
		next_pos = pos->next;
		pos->next = pos->prev = NULL;
		free(pos);
	}
	list->next = list->prev = list;
}

int get_time (char *str)
{
	return ((((str[0] - '0') * 10) + (str[1] - '0')) * 60) + ((str[3] - '0') * 10) + (str[4] - '0');
}

void init_header (struct train **trainQ)
{
	*trainQ = (struct train *) malloc(sizeof(struct train));
	(*trainQ)->prev = *trainQ;
	(*trainQ)->next = *trainQ;
}

void free_header (struct train * list)
{
	list->prev = list->next = NULL;
	free(list);
}

int train_avail(struct train *list, int cur_start_time, int turn_time)
{
	struct train *pos;

	for (pos = list->next; pos != list; pos = pos->next) {
		if (cur_start_time >= ((pos->end + turn_time)%1440)) {
			//This train will go to next station, remove it from this station
			pos->prev->next = pos->next;
			pos->next->prev = pos->prev;
			pos->prev = pos->next = NULL;
			free(pos);
			return 1;
		}
	}
	return 0;
}
void insert_in_trainQ(struct train *trainQ , int start, int end, int station)
{
	struct train *new_train, *pos;
	new_train = (struct train *) malloc (sizeof(struct train));
	new_train->start = start;
	new_train->end = end;
	new_train->station = station;

	//Find the position for the new train :)
	for (pos = trainQ->next; ((pos != trainQ) && (pos->start < start)); pos = pos->next);
	new_train->next = pos;
	new_train->prev = pos->prev;
	pos->prev->next = new_train;
	pos->prev = new_train;
}

int main(int argc, char **argv)
{
	char fname[FNAME_SIZE + 1];
	FILE *fp;

	
	struct train *trainQ = NULL;
	struct train *tr_arr[2];

	struct train *tr, *next_tr;

	int inputs, tcase;
	char start_str[INP_LEN + 1], end_str[INP_LEN + 1];

	int num_tr[] = {0, 0};

	int turn_time, inpA, inpB, i;

	if (argc != 2) {
		printf("Invalid arguments specified\n");
		exit(1);
	}

	//getfname
	strncpy(fname, argv[1], FNAME_SIZE);
	fname[FNAME_SIZE]= '\0';

	fp = fopen(fname,"r");
	if (!fp) {
		perror("open");
		exit(1);
	} 
	init_header(&trainQ);
	init_header(&tr_arr[ST_A]);
	init_header(&tr_arr[ST_B]);

	fscanf(fp, "%d", &inputs);
	for (tcase = 0 ; tcase < inputs; tcase++) {
		num_tr[0] = num_tr[1] = 0;
		if(fscanf(fp, "%d%d%d", &turn_time, &inpA, &inpB) != 3) {
			perror("Empty file");
			exit(1);
		}

		for (i = 0; i < inpA; i++) {
			fscanf(fp, "%s%s", start_str, end_str);
			insert_in_trainQ(trainQ, get_time(start_str), get_time(end_str), ST_A);
		}

		for (; i < (inpA+ inpB); i++) {
			fscanf(fp, "%s%s", start_str, end_str);
			insert_in_trainQ(trainQ, get_time(start_str), get_time(end_str), ST_B);
		}
		
		for (tr = trainQ->next; tr != trainQ; tr = next_tr) {
			next_tr = tr->next;
			if (train_avail(tr_arr[tr->station], tr->start, turn_time) == 0) {
				num_tr[tr->station]++;		
			}
			//Insert train into arriving stationQ
			tr->station = ((tr->station) ^ 1);
	
			tr->prev->next = tr->next;
			tr->next->prev = tr->prev;

			tr->prev = tr_arr[tr->station]->prev;
			tr->next = tr_arr[tr->station];

			tr_arr[tr->station]->prev->next = tr;
			tr_arr[tr->station]->prev = tr;
		} 
		printf ("Case #%d: %d %d\n", tcase+1, num_tr[0], num_tr[1]);
		
		free_list(trainQ);
		free_list(tr_arr[ST_A]);
		free_list(tr_arr[ST_B]);
		
	}
	free_header(trainQ);
	free_header(tr_arr[ST_A]);
	free_header(tr_arr[ST_B]);
	
	return 0;
}
