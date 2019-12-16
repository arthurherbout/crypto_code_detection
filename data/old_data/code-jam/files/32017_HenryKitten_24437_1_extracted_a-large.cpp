#include <iostream>

using namespace std;

struct Point{
	int x,y;
};

Point p[1000000];

int main(){
	int ncas,ti=0;
	for(cin >> ncas;ncas--;){
		int n;
		long long A,B,C,D,x0,y0,M;

		cin >> n >> A >> B >> C >> D >> x0 >> y0 >> M;

		//gen tree
		long long X = x0, Y = y0;
		p[0].x = X % 3; 
		p[0].y = Y % 3;
		for(int i=1;i<n;i++){
			X = (A * X + B) % M;
			Y = (C * Y + D) % M;
			p[i].x = X % 3; 
			p[i].y = Y % 3;
		}

		long long npxy[9] = {0};

		for(int i=0;i<n;i++) npxy[p[i].x*3+p[i].y]++;

//		for(int i=0;i<n;i++) cout << i << ":" << npxy[p[i].x][p[i].y] << " ";
//		cout << endl;

		int a,b,c;
		long long nans = 0;
		for(a=0;a<9;a++)
			for(b=a;b<9;b++)
				for(c=b;c<9;c++)
				{
					if((((a/3+b/3+c/3) % 3) == 0) && (((a+b+c)%3)==0)){
						if(a==b && b==c){
							if(npxy[a] < 3) nans += 0;
							else nans += (npxy[a] * (npxy[a]-1) * (npxy[a]-2)) / 6;
//							printf("a\n");
						}else if(a==b){
							if(npxy[a] < 2) nans += 0;
							else nans += (npxy[a] * (npxy[b]-1) / 2) * npxy[c];
//							printf("b\n");
						}else if(b==c){
							if(npxy[b] < 2) nans += 0;
							else nans += (npxy[c] * (npxy[b]-1) / 2) * npxy[a];
//							printf("c\n");
						}else if(a==c){
							if(npxy[a] < 2) nans += 0;
							else nans += (npxy[a] * (npxy[c]-1) / 2) * npxy[b];
//							printf("d\n");
						}else{
							nans += npxy[a] * npxy[b] * npxy[c];
//							printf("e\n");
						}
					}
				}
		printf("Case #%d: ",++ti);
		cout << nans << endl;
	}
	return 0;
}
