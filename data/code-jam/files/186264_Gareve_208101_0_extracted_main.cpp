#include <iostream>
#include <set>
#define MAX 5555

using namespace std;

char m[30][30];
int numero;
int mov[][4]={
		{0,1},
		{0,-1},
		{-1,0},
		{1,0},
};
int ab(int x){
	if(x<0)
		return -x;
	else
		return x;
}
struct estado{
	string res;
	int nro,x,y;
	estado(string a,int b,int c,int d){
		res=a;
		nro=b;
		y=c;
		x=d;
	}
	estado(){}
	bool operator < (const estado& b)const{//para poder ser usado en set, priority_queue, etc.
		if(res==b.res){
			if(ab(nro-numero)==ab(b.nro-numero))
				if(res.length()==b.res.length())
					if(y==b.y)
						if(x==b.x)
							return true;
						else
							return x<b.x;
					else
						return y<b.y;
				else
					return res.length()<b.res.length();
			else
				return ab(nro-numero)<ab(b.nro-numero);
		}else{
			if(ab(nro-numero)==ab(b.nro-numero))
				if(res.length()==b.res.length())
					return res<b.res;
				else
					return res.length()<b.res.length();
			else
				return ab(nro-numero)<ab(b.nro-numero);
		}
	}
};
//primero el y luego el x
string mem[30][30][MAX];
bool es_car(char c){
	return c=='+'||c=='-';
}
bool es_valido(int y,int x,int n){
	return y>=0&&x>=0&&x<n&&y<n;
}
int to_num(int x){
	return (MAX/2)+x;
}
bool menor(string a,string b){
	if(a.length()==b.length())
		return b<a;
	else
		return b.length()<a.length();
}

void resuelva(){
	int n,nro_q,x,y,nro,yy,xx;
	scanf("%d %d\n",&n,&nro_q);
	string aux;
	for(int i=0;i<n;i++){
		//scanf("%s\n",m[i]);
		cin>>aux;
		for(int j=0;j<n;j++)
			m[i][j]=aux[j];
		//printf("%s\n",m[i]);
	}
	/*printf("______________\n");
	for(int i=0;i<n;i++)
		printf("%s\n",m[i]);

	printf("______________\n");*/

	for(int z=1;z<=nro_q;z++){
		scanf("%d",&numero);
		set<estado> st;

		string str="0",res="zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";

		int len_res=99999999;


		char car;
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				for(int k=0;k<MAX;k++){
					mem[i][j][k]="zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
				}
				/*
				2+1-2
				+3-4+
				5+2+1
				-4-0-
				9+5+1
				*/
				car=m[i][j];
				if(!es_car(m[i][j])){
					str[0]=m[i][j];
					//if(m[i][j]!='1')continue;

					st.insert(estado(str,m[i][j]-'0',i,j));
					mem[i][j][to_num(m[i][j]-'0')]=true;
				}
			}
		}
		while(!st.empty()){
			str=st.begin()->res;
			yy=st.begin()->y;
			xx=st.begin()->x;
			nro=st.begin()->nro;
			//cout<<nro<<endl;
			st.erase(st.begin());
			if((int)str.length()>len_res)
				continue;
			if(nro==numero){
				if((int)str.length()<len_res){
					res=str;
					len_res=str.length();
				}else if((int)str.length()==len_res){
					if(str<res)
						res=str;
				}
			}else{
				bool signo=es_car(str[str.length()-1]),signo_ultimo;
				for(int j=0;j<4;j++){
					x=xx+mov[j][0];
					y=yy+mov[j][1];
					if(es_valido(y,x,n)){
						signo_ultimo=es_car(m[y][x]);
						if(signo!=signo_ultimo){
							int nuevo_numero;
							if(signo==true){
								//el ultimo caracter si es un signo, hay que calcular la nueva suma
								int mult=1;
								if(str[str.length()-1]=='-')
									mult=-1;
								nuevo_numero=nro+(mult*(m[y][x]-'0'));
								string nuevo=str+m[y][x];
								if(to_num(nuevo_numero)<0||to_num(nuevo_numero)>=MAX){
									cout<<"ERRROOOOOR\n\n\n\n";
									return;
								}
								if(menor(mem[y][x][to_num(nuevo_numero)],nuevo)){
									st.insert(estado(nuevo,nuevo_numero,y,x));
									mem[y][x][to_num(nuevo_numero)]=nuevo;
								}
							}else{
								//voy a aumentar solo un signo
								st.insert(estado(str+m[y][x],nro,y,x));
							}
						}
					}
				}
			}
		}
		cout<<res<<endl;
	}
}
int main(){
	int n;
	scanf("%d",&n);
	for(int i=1;i<=n;i++){
		printf("Case #%d:\n",i);
		resuelva();
	}
}
/*
2
5 3
2+1-2
+3-4+
5+2+1
-4-0-
9+5+1
20 30 40
3 2
2+1
+4+
5+1
2 20

2
5 3
2+9-2
+3-4+
5+2+9
-4-0-
9+5+1
20 30 40

*/
