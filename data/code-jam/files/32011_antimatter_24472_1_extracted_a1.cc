#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
using namespace std;

typedef long long ii;

int N;

struct foo {int a, b, c;} f[5000];

struct cmpA : public binary_function<foo,foo,bool> {
  bool operator() (const foo &a, const foo&b) {
    return a.a < b.a;
  }
};
struct cmpB : public binary_function<foo,foo,bool> {
  bool operator() (const foo &a, const foo&b) {
    return a.b < b.b;
  }
};

struct node {
  int val, ct;
  node *l, *r;
  int LS;
} NN[10000];
int NNN = 0;

node* create(int val) {
  NN[NNN].val = val;
  NN[NNN].ct = 1;
  NN[NNN].l = 0;
  NN[NNN].r = 0;
  NN[NNN].LS = 0;
  return NN+(NNN++);
};
void insert(node* v, int val) {
  if (v->val == val) v->ct++;
  else {
    if (val < v->val) {
      if (v->l) insert(v->l, val);
      else v->l = create(val);
      v->LS++;
    }
    else {
      if (v->r) insert(v->r, val);
      else v->r = create(val);
    }
  }
}
int _(node* n, int thresh) {
  if (!n) return 0;

  if (thresh > n->val) return n->LS + n->ct + _(n->r, thresh);
  else if (thresh == n->val) return n->LS + n->ct;
  else return _(n->l, thresh);
}

int main() {
  int T;
  cin >> T;
  for (int it = 1; it <= T; it++) {
    cin >> N;
    for (int i = 0; i < N; i++)
      cin >> f[i].a >> f[i].b >> f[i].c;

    int ret = 0;

    for (int i = 0; i < N; i++) {
      sort(f, f+N, cmpA()); // sort by A

      NNN = 0;

      int A = f[i].a;
      node* root = 0;

      sort(f, f+i+1, cmpB());

      for (int j = 0; j <= i; j++) {	
	int B = f[j].b;
	if (A+B > 10000) break;

	//	printf("A = %d, B = %d\n", A, B);

	if (!root) root = create(f[j].c);
	else insert(root, f[j].c);

	int C = 10000 - A - B;
	int ct = _(root, C);
	ret = max(ret, ct);

	//	for (int k = 0; k <= i; k++) printf("%d %d %d\n", f[k].a, f[k].b, f[k].c);
	//	printf("i = %d, j = %d, ct = %d\n", i, j, ct);
      }
    }

    printf("Case #%d: %d\n", it, ret);
  }
}
