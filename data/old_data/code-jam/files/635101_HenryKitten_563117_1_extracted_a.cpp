#include <cstdio>
#include <string>
#include <map>
using namespace std;

int cnt;

struct Node{
	map<string,Node*> children;
	Node(){
		children.clear();
	}
};

char input[100001];

int main(){
	int n,m;
	int ncas;
	scanf("%d",&ncas);
	for(int t=1;t<=ncas;t++){
		cnt = 0;
		Node* root = new Node;
		scanf("%d %d",&n,&m);
		while(n--){ 
			Node* curnode = root;
			scanf("%s",input);
			string str = "";
			for(int i=1;;i++){
				if(input[i] == '/' || input[i] == '\0'){
					if(curnode->children.find(str) == curnode->children.end()){
						Node* nextnode = new Node;
						curnode->children[str] = nextnode;
						curnode = nextnode;
					}else{
						curnode = curnode->children[str];	
					}
					if(input[i] == '\0') break;
					str = "";
				}else{
					str.push_back(input[i]);
				}
			}
		}
		while(m--){
			Node* curnode = root;
			scanf("%s",input);
			string str = "";
			for(int i=1;;i++){
				if(input[i] == '/' || input[i] == '\0'){
					if(curnode->children.find(str) == curnode->children.end()){
						Node* nextnode = new Node;
						curnode->children[str] = nextnode;
						curnode = nextnode;
						cnt++;
					}else{
						curnode = curnode->children[str];	
					}
					if(input[i] == '\0') break;
					str = "";
				}else{
					str.push_back(input[i]);
				}
			}
		}
		printf("Case #%d: %d\n",t,cnt);
	}
	return 0;
}
