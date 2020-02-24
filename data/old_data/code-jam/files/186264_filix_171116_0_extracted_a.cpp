#include<cstdio>
#include<string>
#include<set>
using namespace std;

struct Drzewo {
	double waga;
	int liczbaDzieci;
	string nazwa;
	Drzewo *dzieci[2],*ojciec;
	void walk(int level=0) {
		for(int i=0;i<level;++i)
			putchar(' ');
		printf("waga=%.3f nazwa=%s dzieci=%d\n",waga,nazwa.c_str(),liczbaDzieci);
		for(int i=0;i<liczbaDzieci;++i)
			dzieci[i]->walk(level+3);
	}
};

enum Typ {NIC,LICZBA,NAZWA,LEWY,PRAWY};

Drzewo mem[10000];
int mempos;

int main() {
	int testow;
	scanf("%d",&testow);
	for(int z=1;z<=testow;++z) {
		mempos=0;
		printf("Case #%d:\n",z);
		int linii,zwierzat;
		scanf("%d\n",&linii);
		Drzewo korzen=(Drzewo){1,false};
		Drzewo *akt=&korzen;
		string opis;
		char stemp[88];
		while(linii--) {
			fgets(stemp,88,stdin);
			opis+=" ";
			for(int i=0;stemp[i];++i) {
				if(stemp[i]=='(' || stemp[i]==')') {
					opis+=" ";
					opis+=stemp[i];
					opis+=" ";
				} else {
					opis+=stemp[i];
				}
			}
			opis+=" ";
		}
		string token;
		Typ typ=NIC;
		for(int i=0,n=opis.size();i<n;++i) {
			char &c=opis[i];
			if(c==' ' || c=='\n') {
				switch(typ) {
					case NIC:
					break;
					case LICZBA: {
						//printf("token LICZBA(%s)\n",token.c_str());
						sscanf(token.c_str(),"%lf",&(akt->waga));
					}
					break;
					case NAZWA: {
						//printf("token NAZWA(%s)\n",token.c_str());
						akt->nazwa=token;
					}
					break;
					case LEWY: {
						//printf("token LEWY\n");
						Drzewo *d=mem+mempos;
						++mempos;
						d->liczbaDzieci=0;
						d->nazwa="";
						d->ojciec=akt;
						akt->dzieci[akt->liczbaDzieci]=d;
						++akt->liczbaDzieci;
						akt=d;
					}
					break;
					case PRAWY: {
						//printf("token PRAWY\n");
						akt=akt->ojciec;
					}
				}
				typ=NIC;
				token="";
			} else if((c>='0' && c<='9') || c=='.') {
				typ=LICZBA;
				token+=c;
			} else if(c>='a' && c<='z') {
				typ=NAZWA;
				token+=c;
			} else if(c=='(')
				typ=LEWY;
			else if(c==')')
				typ=PRAWY;
			else {
				printf("FATAL ERROR NIEZNANY ZNAK (%c) !!!!!!!!!!!!!!!!\n",c);
			}
		}
		//printf("\nopis=[%s]\n\n",opis.c_str());
		//korzen.dzieci[0]->walk();
		scanf("%d",&zwierzat);
		while(zwierzat--) {
			int cech;
			scanf("%*s%d",&cech);
			set<string> cechy;
			for(int i=0;i<cech;++i) {
				scanf("%s",stemp);
				cechy.insert(string(stemp));
			}
			double wynik=1;
			akt=korzen.dzieci[0];
			for(;;) {
				wynik*=akt->waga;
				if(akt->liczbaDzieci>0) {
					if(cechy.find(akt->nazwa)!=cechy.end()) {
						akt=akt->dzieci[0];
					} else {
						akt=akt->dzieci[1];
					}
				} else
					break;
			}
			printf("%.7f\n",wynik);
		}
	}
}
