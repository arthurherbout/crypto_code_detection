#include <iostream>

using namespace std;

struct Point{
	long long x,y;
};

int main(){
	int ncas,ti=0;
	for(cin >> ncas;ncas--;){
		Point p[128];
		int n;
		long long A,B,C,D,x0,y0,M;

		cin >> n >> A >> B >> C >> D >> x0 >> y0 >> M;

		//gen tree
		long long X = x0, Y = y0;
		p[0].x = X; 
		p[0].y = Y;
		for(int i=1;i<n;i++){
			X = (A * X + B) % M;
			Y = (C * Y + D) % M;
			p[i].x = X; 
			p[i].y = Y;
		}


		int nans = 0;
		for(int i=0;i<n;i++){
			for(int j=i+1;j<n;j++){
				for(int k=j+1;k<n;k++){
					if(i!=j && j!=k && i!=k){
						if(((p[i].x + p[j].x + p[k].x) % 3) == 0 && ((p[i].y + p[j].y + p[k].y)% 3) == 0) nans++;
					}
				}
			}
		}
		printf("Case #%d: ",++ti);
		cout << nans << endl;
	}
	return 0;
}
