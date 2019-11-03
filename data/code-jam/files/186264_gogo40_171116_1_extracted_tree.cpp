#include <stdio.h>
#include <string>
#include <map>

using namespace std;

struct tree{
	double w; 
	int n;
	string feature;
	tree *L, *R;
	
	tree():feature(""){L=0; R=0; w=0; n=0; }
};

map<string, bool> tem;
char buffer[100];

void read(tree* r){
	char c; int n;
	n=0;
	while(1){
		c=getc(stdin);
		if(c=='('){
			scanf("%lf",&r->w);
			while(1){
				c=getc(stdin);
				if(c=='('){
					ungetc(c,stdin);
					if(n==0){
						r->L=new tree();
						read(r->L);
						n++;
					}else{
						r->R=new tree();
						read(r->R);
						n++;
					}
				}
				if(c>='a' && c<='z') { 
					ungetc(c,stdin);
					scanf("%s",buffer); 
					r->feature=buffer; 
				}
				if(c==')') break;
			}
		}
		if(c==')') break;
	}
	
	r->n=n;
}

void dfs(tree* r, double acc){
	if(r->n==0){
		printf("%.7lf\n",acc*r->w);
		return;
	}
	if(tem[r->feature]) dfs(r->L,acc*r->w);
	else dfs(r->R,acc*r->w);
}

int main()
{
	int N, c, L, A, n;
	
	scanf("%d\n",&N);
	c=1;
	while(N--){
		
		tree* root;
		root=new tree();
		
		scanf("%d\n",&L);
		read(root);
		
		scanf("%d\n",&A);
		printf("Case #%d:\n",c);
		while(A--){
			tem.clear();
			tem[""]=true;
			scanf("%s",buffer);
			scanf("%d",&n);
			while(n--){
				scanf("%s",buffer);
				tem[buffer]=true;
			}
			
			dfs(root,1.0);
		}
		c++;
	}
	return 0;
}

