#include "break_password.h"

// Process rank
int my_rank;

// Hashed password
char hashed[33];

// Password length
int size;

// True password
char* true_password;

// Number of processes
int num_proc;

int main(int argc, char** argv) {

	// Input hashed string
	if (argc < 2) {
		printf("Please enter hashed string.\n");
		return 0;
	}

	strcpy(hashed, argv[1]);
	size = atoi(argv[2]);

	// Allocate
	true_password = (char*)malloc(sizeof(char) * (size + 1));

	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get number of process
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	// printf("%d\n", num_proc);

	// Rank of process
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Handle
	if (my_rank == 0) {
		// Rank 0
		time_t t0 = time(0);
		rank_0();
		float x = time(0) - t0;
		printf("Time %f\n", x);
	} else {
		// Rank k
		rank_k();
	}

	// Finalize the MPI environment
	MPI_Finalize();

	// Free
	free(true_password);

	return 0;
}

// Rank 0 handler
int rank_0() {
	int i, j, sm;
	
	// Receive finish signal
	MPI_Request recv_finish_request[num_proc - 1];
	int finish_buff[num_proc - 1][1]; 

	for (i = 1; i < num_proc; i++) {
		MPI_Irecv(finish_buff[i - 1], 1, MPI_INT, i, TAG_COMPUTE_FINISH,
				MPI_COMM_WORLD, recv_finish_request + i - 1);
	}

	printf("rank 0 init receive finish signal\n");

	// Receive true password
	MPI_Request recv_password_request;

	MPI_Irecv(true_password, size + 1, MPI_CHAR, MPI_ANY_SOURCE, TAG_COMPUTE_SEND_TRUE_PASSWORD,
			MPI_COMM_WORLD, &recv_password_request);

	printf("rank 0 init receive true password\n");

	// Devide password for compute processes
	int* prefix[num_proc];
	printf("size = %d\n", size);
	
	// password prefix array for each process
	int m_0 = N % num_proc;
	int m = N / num_proc;

	printf("Number of character in alphabet: %d\n", N);
	printf("m = %d\n", m);
	printf("m0 = %d\n", m_0);
	printf("rank 0: num process = %d\n", num_proc);

	// Prefixes
	int marr[num_proc];

	sm = 0;
	for (i = 0; i < num_proc; i++) {
		// allocate 
		if (m_0 > 0) {
			marr[i] = m + 1;
			m_0--;
		} else {
			marr[i] = m;
		}

		prefix[i] = (int*)malloc(sizeof(int) * marr[i]);

		printf("prefix rank %d: ", i);
		for (j = 0; j < marr[i]; j++) {
			prefix[i][j] = sm + j;
			printf("%d ", prefix[i][j]);
		}
		printf("\n");

		sm += marr[i];
	}

	// Send prefix arrays
	MPI_Status status;

	// rank k > 0
	for (i = 1 ; i < num_proc; i++) {
		// send prefix array length
		MPI_Send(marr + i, 1, MPI_INT, i, TAG_MASTER_SEND_PREFIX_ARRAY_LENGTH,
				MPI_COMM_WORLD);

		// send prefix array
		MPI_Send(prefix[i], m, MPI_INT, i, TAG_MASTER_SEND_PREFIX_ARRAY,
				MPI_COMM_WORLD);
		printf("send prefix array to process rank %d\n", i);
	}

	printf("rank 0 send done no error\n");
	// return 0;

	// Handle prefix array for rank 0
	int curr[size];
	int end[size];

	// end
	end[0] = prefix[0][marr[0] - 1];
	for (i = 1; i < size; i++) {
		end[i] = N - 1;
	}
	printf("rank 0 end:");
	for (i = 0; i < size; i++) {
		printf("%c ", ALPHABET[end[i]]);
	}
	printf("\n");

	// current
	curr[0] = prefix[0][0];
	for (i = 1; i < size; i++) {
		curr[i] = 0;
	}

	printf("rank 0 curr:");
	for (i = 0; i < size; i++) {
		printf("%c ", ALPHABET[curr[i]]);
	}
	printf("\n");
	

	// Free
	for (i = 0; i < num_proc; i++) {
		free(prefix[i]);
	}

	// Finished processes
	int finished[num_proc];
	int num_finished = 0;

	for (i = 0; i < num_proc; i++) {
		finished[i] = 0;
	} 

	int count = 0;
	while (1) {
		count++;
		
		// printf("count = %d\n", count);

		// listen to send true password signal from compute
		int received_password = 0;
		int from;

		MPI_Test(&recv_password_request, &received_password, &status);
		if (received_password) {
			from = status.MPI_SOURCE;
		}

		// if received true password
		if (received_password) {
			printf("True password found from %d: %s\n", from, true_password);
			break;
		}

		// listen to finish signal from compute
		for (i = 1; i < num_proc; i++) {
			MPI_Test(recv_finish_request + i - 1, finished + i, &status);
			num_finished += finished[i];
			if (finished[i]) {
				printf("finished %d\n", num_finished);
			}
		}

		

		// generate candidate, hash then compare
		int found = generate_hash_compare(curr, end);
		
		// printf("found = %d\n", found);
		
		if (!(count % 100000)) {
		// if (1) {
			printf("rank %d: ", my_rank);
			for (i = 0; i < size; i++) {
				printf("%c ", ALPHABET[curr[i]]);
			}
			printf("\n");
		}

		if (found == -1) {
			// reach end
			if (num_finished == num_proc - 1) {
				printf("rank 0: Not found\n");
				return 0;
			} 
		}

		// If found true password, return
		if (found == 1) {
			printf("True password found: %s\n", true_password);
			break;
		}
	}

	// Stop all compute
	stop_all_computes();	
	
}

// Rank k handler
int rank_k() {
	// Init receive stop signal
	int stop_buff[1];
	MPI_Request recv_stop_request;

	MPI_Irecv(stop_buff, 1, MPI_INT, 0, TAG_MASTER_SEND_STOP_SIGNAL,
			MPI_COMM_WORLD, &recv_stop_request);

	printf("rank %d init receive stop signal\n", my_rank);

	MPI_Status status; // MPI status

	// Length of prefix array at process rank k > 0
	int m;

	// Receive prefix array length
	MPI_Recv(&m, 1, MPI_INT, 0, TAG_MASTER_SEND_PREFIX_ARRAY_LENGTH, 
			MPI_COMM_WORLD, &status);

	int prefix[m]; // prefix array

	// Receive prefix array
	MPI_Recv(prefix, m, MPI_INT, 0, TAG_MASTER_SEND_PREFIX_ARRAY, 
			MPI_COMM_WORLD, &status);
	printf("rank %d received prefix array\n", my_rank);

	printf("prefix rank %d: ", my_rank);
	int i;
	for (i = 0; i < m; i++) {
		printf("%c ", ALPHABET[prefix[i]]);
	}
	printf("\n");

	// First and last password candidate
	int curr[size]; // current candidate
	int end[size]; // end candidate

	// end
	end[0] = prefix[m - 1];
	for (i = 1; i < size; i++) {
		end[i] = N - 1;
	}
	printf("rank %d end:", my_rank);
	for (i = 0; i < size; i++) {
		printf("%c ", ALPHABET[end[i]]);
	}
	printf("\n");

	// current
	curr[0] = prefix[0];
	for (i = 1; i < size; i++) {
		curr[i] = 0;
	}
	printf("rank %d curr:", my_rank);
	for (i = 0; i < size; i++) {
		printf("%c ", ALPHABET[curr[i]]);
	}
	printf("\n");

	int count = 0;
	while (1) {
		count++;
		// listen to stop signal 

		int stop;
		MPI_Test(&recv_stop_request, &stop, &status);
		if (stop) {
			printf("Rank %d: Received stop signal from master\n", my_rank);
			return 0;
		}

		// generate, hash then compare
		int found = generate_hash_compare(curr, end);
		
		if (!(count % 100000)) {
			printf("rank %d: ", my_rank);
			for (i = 0; i < size; i++) {
				printf("%c ", ALPHABET[curr[i]]);
			}
			printf("\n");
		}

		if (found == -1) {
			// reach endc
			break;
		}

		// if found then send true password to master
		if (found) {
			compute_send_password();

			return 0;
		}
	}

	// Send finish signal
	compute_finish();
	return 0;
}

// Generate, hash and compare
int generate_hash_compare(int curr[], int end[]) {
	int next[size]; // next candidate
	char candidate[size + 1]; // candidate string
	char hashed_candidate[33]; // md5 hashed candidate string 

	// to string
	indices_to_string(size, curr, candidate);
	
	// printf("candidate %s\n", candidate);

	// hash
	md5(candidate, size, hashed_candidate);
	
	// printf("len hashed %d\n", (int)strlen(hashed_candidate));
	// printf("hashed %s\n", hashed_candidate);
	
	// compare
	int cmp = strcmp(hashed, hashed_candidate);

	if (!cmp) {
		// true password
		strcpy(true_password, candidate);
		return 1;
	}

	// next indices
	int result = generate_next_indices(size, end, curr, next);
	if (result == -1) {
		// reach end
		return -1;
	}

	// update current
	int i;
	for (i = 0; i < size; i++) {
		curr[i] = next[i];
	}

	return 0;
}

// Compute process send finish signal
int compute_finish() {
	// Buffer
	int buff[1];
	buff[0] = my_rank;

	// Send finish signal
	MPI_Send(buff, 1, MPI_INT, 0, TAG_COMPUTE_FINISH, 
			MPI_COMM_WORLD);
	printf("compute %d send finish\n", my_rank);
	return 1;
}

// Compute send true password
int compute_send_password() {
	// Send finish signal
	MPI_Send(true_password, size + 1, MPI_CHAR, 0, TAG_COMPUTE_SEND_TRUE_PASSWORD, 
			MPI_COMM_WORLD);
	printf("Compute rank %d send password: %s\n", my_rank, true_password);
	return 1;
}

// Master send stop all compute
int stop_all_computes() {
	// Buffer
	int buff[] = {1}; 

	// Send stop signals
	int i;
	for (i = 1; i < num_proc; i++) {
		MPI_Send(buff, 1, MPI_INT, i, TAG_MASTER_SEND_STOP_SIGNAL,
				MPI_COMM_WORLD);
	}
}