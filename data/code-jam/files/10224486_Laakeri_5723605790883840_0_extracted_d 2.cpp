#include <bits/stdc++.h>
#define F first
#define S second
#define X real()
#define Y imag()
using namespace std;
typedef long long ll;
typedef long double ld;

int lv[111];
int r,c,n;

int lol[111][111];

vector<int> fo;

int git=1;

int u[22][22][6];

void dfs(int y, int x, int d){
	if (y<0||y>=r||x<0||x>=c) return;
	if (u[y][x][d]==git) return;
	//cout<<git<<" "<<y<<" "<<x<<" "<<d<<endl;
	u[y][x][d]=git;
	if (y==0&&d==1){
		//cout<<"fo "<<x+1<<endl;
		fo.push_back(x+1);
	}
	if (x==c-1&&d==2){
		//cout<<"fo "<<c+y+1<<endl;
		fo.push_back(c+y+1);
	}
	if (y==r-1&&d==3){
		//cout<<"fo "<<c+r+c-x<<endl;
		fo.push_back(c+r+c-x);
	}
	if (x==0&&d==4){
		//cout<<"fo "<<c+r+c+r-y<<endl;
		fo.push_back(c+r+c+r-y);
	}
	if (d==1){
		dfs(y-1, x, 3);
		if (lol[y][x]){
			dfs(y, x, 2);
		}
		else{
			dfs(y, x, 4);
		}
	}
	if (d==2){
		dfs(y, x+1, 4);
		if (lol[y][x]){
			dfs(y, x, 1);
		}
		else{
			dfs(y, x, 3);
		}
	}
	if (d==3){
		dfs(y+1, x, 1);
		if (lol[y][x]){
			dfs(y, x, 4);
		}
		else{
			dfs(y, x, 2);
		}
	}
	if (d==4){
		dfs(y, x-1, 2);
		if (lol[y][x]){
			dfs(y, x, 3);
		}
		else{
			dfs(y, x, 1);
		}
	}
	
}

int test(){
	for (int i=1;i<=n;i++){
		//cout<<"test "<<i<<endl;
		git++;
		fo.clear();
		if (i<=c){
			dfs(0, i-1, 1);
		}
		else if(i<=c+r){
			dfs(i-c-1, c-1, 2);
		}
		else if(i<=c+r+c){
			dfs(r-1, c-(i-(c+r)-1)-1, 3);
		}
		else {
			dfs(r-(i-(c+r+c)-1)-1, 0, 4);
		}
		if ((int)fo.size()!=2) return 0;
		if (fo[0]!=lv[i]&&fo[1]!=lv[i]) return 0;
	}
	return 1;
}

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int tcs;
	cin>>tcs;
	for (int tc=1;tc<=tcs;tc++){
		cout<<"Case #"<<tc<<":"<<endl;
		cin>>r>>c;
		n=2*(r+c);
		int e=0;
		for (int i=1;i<=n;i++){
			int a;
			cin>>a;
			if (i%2==0){
				lv[e]=a;
				lv[a]=e;
			}
			e=a;
		}
		int f=0;
		for (int b=0;b<(1<<(r*c));b++){
			int i2=0;
			for (int i=0;i<r;i++){
				for (int j=0;j<c;j++){
					if (b&(1<<i2)){
						lol[i][j]=1;
					}
					else{
						lol[i][j]=0;
					}
					i2++;
				}
			}
			if (test()){
				i2=0;
				for (int i=0;i<r;i++){
					for (int j=0;j<c;j++){
						if (b&(1<<i2)){
							cout<<'\\';
						}
						else{
							cout<<'/';
						}
						i2++;
					}
					cout<<endl;
				}
				f=1;
				break;
			}
		}
		if (!f){
			cout<<"IMPOSSIBLE"<<endl;
		}
	}
}