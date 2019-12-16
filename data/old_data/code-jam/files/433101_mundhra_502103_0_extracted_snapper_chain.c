#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ON 1
#define OFF 0
#define DUMMY 0

#define MAX_DEV 30

struct snapper{
	int state;
	int output;
};

void init_snappers(struct snapper *dev, int n)
{
	int i;

	/* All sanppers are in OFF state to begin with*/
	for (i = 0; i <= n ; i++)  {
		dev[i].state = OFF;
		dev[i].output = OFF;
	}
	
	/*Dummy Snapper has power ON */
	dev[0].output = ON;
}

static void toggle_state(struct snapper *dev, int devid)
{
	int last_state = dev[devid].state;
	if (devid == DUMMY) {
		return;
	}
	
	dev[devid].state = ((last_state == ON) ? OFF : ON);
}

static void update_output(struct snapper *dev, int devid)
{
	if ((dev[devid - 1].output == ON) && (dev[devid].state == ON)) {
		dev[devid].output = ON;
	} else {
		dev[devid].output = OFF;
	}
}

static void do_one_snap(struct snapper *dev, int n)
{
	int devid;

	for (devid = 1; devid <= n; devid++) {
		if (dev[devid-1].output == ON) {
			toggle_state(dev, devid);
		}
	}

	for (devid = 1; devid <= n; devid++) {
		update_output(dev, devid);
	}
}
#if 0
void dump_state(struct snapper *dev, int n, char *str)
{
	int i;
	printf("\n\n %s :\n", str);

	for (i = 0; i <= n ; i++) {
		printf("Dev[%d] S:%d O:%d\n", i, dev[i].state, dev[i].output);
	}
}
#endif
	 
int compute_light_state(int n, int k)
{
	int snap;
	int light_output;
	struct snapper dev[MAX_DEV];
	init_snappers(dev, n);
	
	//dump_state(dev, n, "Init");
	for (snap = 0; snap < k; snap++) {
		//char str[10];
		//snprintf(str, 9, "%s%d", "Snap", snap);
		do_one_snap(dev, n);
		//dump_state(dev, n , str);
	}

	light_output = dev[n].output;
	return light_output;	
}

static int process_each_input(FILE *fp, int num_inputs)
{
	int index;
	int ret = 0;

	for (index = 0; index < num_inputs; index++) {
		int n;
		int k;
		int final_output;
		ret = fscanf(fp, "%d %d", &n, &k);

		if (ret != 2) {
			printf("Error while readinf i/p file\n");
			return -EIO;
		}
		
		final_output = compute_light_state(n, k);	

		printf("Case #%d: %s\n", index + 1, (final_output == ON) ? "ON" : "OFF");
	}
	
	return ret;
}

static int process_inputs(int argc, char **argv)
{
	char *fname;
	FILE *fp;
	int ret = 0;
	int num_inputs;

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

	ret = process_each_input(fp, num_inputs);
	
	return ret;
}


int main (int argc, char **argv)
{
	int ret;

	ret = process_inputs(argc, argv);
	
	return ret;
}
