#include<stdio.h>

int findCommon(int a1[], int a2[]);
int main(){
	int t;
	scanf("%d",&t);
	int results[t];
	
	for(int i = 0; i < t; i++){
		int choice1;
		scanf("%d",&choice1);
		int row_matrix1[4];
		
		// read the first matrix
		for(int row = 1; row <= 4; row++){
			for(int col = 0; col<4; col++){
				int temp;
				scanf("%d",&temp);
				if(row == choice1)				
					row_matrix1[col] = temp;
			}
		}
		
		int choice2;
		scanf("%d",&choice2);
		int row_matrix2[4];

		// read the first matrix
		for(int row = 1; row <= 4; row++){
			for(int col = 0; col<4; col++){
				int temp;
				scanf("%d",&temp);
				if(row == choice2)				
					row_matrix2[col] = temp;
			}
		}
		results[i] = findCommon(row_matrix1, row_matrix2);		
	}
	
	// print the Output
	for(int i = 0; i<t; i++){
		if(results[i] == -1)
			printf("Case #%d: Bad magician!\n",i+1);
		else if(results[i] == 0)
			printf("Case #%d: Volunteer cheated!\n",i+1);
			else
				printf("Case #%d: %d\n",i+1,results[i]);
	}
	return 0;
}

int findCommon(int arr1[], int arr2[]){
	int matched_num = 0;
	int num_of_matches = 0;
	for(int i=0; i<4; i++){
		for(int j = 0; j<4; j++){
			if(arr1[i] == arr2[j]){
				matched_num = arr1[i];
				num_of_matches++;
			}
		}
	}
	if(num_of_matches == 1)
			return matched_num;
	else{
	if(num_of_matches == 0)
		return 0;
	else
		return -1;
	}

}
