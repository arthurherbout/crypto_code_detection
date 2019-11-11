#include <fstream>
#include <iostream>
#include <stdint.h>
using namespace std;
char message[2][4]={"NO","YES"};
int lawn(istream& in,ostream& out){
	int n;
	in>>n;
	for(int i=1;i<=n;i++){
		int res=0;
		int l[100][100];
		int inch[100];
		int incv[100];
		int w,h,mi=100,ma=0;
		in>>w>>h;
		for(int x=0;x<w;x++){
			for(int y=0;y<h;y++){
				in>>l[x][y];
				if(mi>l[x][y]) mi=l[x][y];
				if(ma<l[x][y]) ma=l[x][y];
			}
		}
		res=1;
		for(int j=mi;j<ma;j++){
			for(int x=0;x<w;x++){
				incv[x]=1;
				for(int y=1;y<h;y++){
					if(l[x][y]>j){incv[x]=0;break;}
					if(l[x][y]<j){res=0;goto endlp;}
					if(l[x][y]==l[x][y-1])continue;
					incv[x]=0;
					break;
				}
			}
			for(int x=0;x<h;x++){
				inch[x]=1;
				for(int y=0;y<w;y++){
					if(l[y][x]>j){inch[x]=0;break;}
					if(y==0) continue;
					if(l[y][x]<j){res=0;goto endlp;}
					if(l[y][x]==l[y-1][x])continue;
					inch[x]=0;
					break;
				}
			}
			for(int x=0;x<w;x++){
				for(int y=0;y<h;y++){
					if(inch[y]||incv[x])l[x][y]++;
				}
			}
		}
		for(int x=0;x<w;x++){
			for(int y=0;y<h;y++){
				if(l[x][y]==ma) continue;
				res=0;
				goto endlp;
			}
		}
endlp:		out<<"Case #"<<i<<": "<<message[res]<<'\n';
	}
	return 0;
}
int main(){
	ifstream fin("lawn.in");
	ofstream fout;
	if(fin.is_open()){
		fout.open("lawn.out");
		return lawn(fin,fout);
	}
	return lawn(cin,cout);
}

