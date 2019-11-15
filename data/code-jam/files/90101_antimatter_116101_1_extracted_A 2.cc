#include <cstdio>
#include <string>
#include <vector>
using namespace std;

struct node {
  node* child[26];
} _[100001];
int NC = 1;
node* root = _;

int L, D, N;
char buf[1000];

void add(node* x, char* z) {
  if (z[0] == 0) return;
  node* next = x->child[z[0]-'a'];
  if (next == 0)
    next = x->child[z[0]-'a'] = _ + (NC++);
  add(next, z+1);
}

int main() {
  scanf("%d %d %d", &L, &D, &N);
  for (int i = 0; i < D; i++) {
    scanf("%s", buf);
    add(root, buf);
  }
  for (int it = 1; it <= N; it++) {
    scanf("%s", buf);
    vector<string> poss;
    string cur = "";
    bool inparen = 0;
    for (int i = 0; buf[i]; i++) {
      if (buf[i] == '(') inparen = 1;
      else if (buf[i] == ')') {inparen = 0; poss.push_back(cur); cur = "";}
      else if (inparen) cur += buf[i];
      else {cur += buf[i]; poss.push_back(cur); cur = "";}
    }

    vector<node*> A, B;
    A.push_back(root);
    for (int i = 0; i < L; i++) {
      B.clear();
      string p = poss[i];
      for (int j = 0; j < A.size(); j++) {
	for (int k = 0; k < p.size(); k++) {
	  int idx = p[k] - 'a';
	  if (A[j]->child[idx] != 0)
	    B.push_back(A[j]->child[idx]);
	}
      }
      A = B;
    }
    printf("Case #%d: %d\n", it, A.size());
  }
}
