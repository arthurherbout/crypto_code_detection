#include<iostream>

using namespace std;


void showOutput(int A[4][4], int testcase){
	int sum1,sum2;
	int sum3 = 0 , sum4 = 0;
	bool vaccant = false;
	bool traversedDiagonal = false;
	for (int i = 0 ; i < 4 ; i++){
		sum1 = 0;
		sum2 = 0;
		for (int j = 0 ; j < 4 ; j++){
			sum1 += A[i][j];
			sum2 += A[j][i];
			if(!traversedDiagonal){
				sum3 += A[j][j];
				sum4 += A[3-j][j];
			}
		}
		traversedDiagonal = true;

//cout << sum1 << "  " << sum2 << "  "<< sum3 << "  " << sum4  << "\n" ;
			if(sum1 >= 17 || sum2 >= 17 ) vaccant = true;
			if(sum1 == 3 || sum1 == 4 || sum2 == 3 || sum2 == 4 || sum3 == 3 || sum3 == 4 || sum4 == 3 ||sum4 == 4) {
				cout << "Case #" << testcase << ": " << "X won\n" ;
				return;
			}
			if(sum1 == -3 || sum1 == -4  || sum2 == -3 || sum2 == -4 || sum3 == -3 || sum3 == -4 || sum4 == -3 ||sum4 == -4)  {
				cout << "Case #" << testcase << ": " << "O won\n" ;
				return;
			}
			if(i == 3 && vaccant ) {
				cout << "Case #" << testcase << ": " << "Game has not completed\n" ; 
				return;
			}
			if(i == 3 && !vaccant ) {
				cout << "Case #" << testcase << ": " << "Draw\n" ;
				return;
			}
	
	}
	return;		
}

int main()
{
	int T;
	cin >> T;

	int A[4][4];
	char temp;
	int testcase = 0;
	
	while( ++testcase <= T){
		for (int i = 0 ; i < 4 ; i++){
			for (int j = 0 ; j < 4 ; j++){
				cin >> temp;
				if(temp == 'X')A[i][j]= 1;
				if(temp == 'O')A[i][j]= -1;
				if(temp == '.')A[i][j]= 20;
				if(temp == 'T')A[i][j]= 0;

			}
		}
		showOutput(A,testcase);
	}
	return 0;
}

