#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

#define f fgets(buf,1000,stdin)
char buf[1000];

struct node {
  double weight;
  string feature;
  node *l, *r;
  int state;
} nodes[5000];
int ni = 0;
node* nodestack[1000];
int nsi = 0;

node* make() {
  node* ret = nodes + (ni++);
  ret->feature ="";
  ret->l = 0;
  ret->r = 0;
  ret->state = 0;
  return ret;
}
void push(node* z) {nodestack[nsi++] = z;}
node* pop() {return nodestack[--nsi];}
node* top() {return nodestack[nsi-1];}
bool ws(char c) {return c == ' ' || c == '\n' || c == '\r'; }
double pd(string s) {
  double ret;
  stringstream S; S << s; S >> ret;
  return ret;
}

double eval(node* n, const vector<string> &features) {
  if (n->feature == "") return n->weight;
  for (int i = 0; i< features.size(); i++)
    if (features[i] == n->feature)
      return n->weight * eval(n->l, features);
  return n->weight * eval(n->r, features);
}

void pp(node* n) {
  printf("(%lf", n->weight);
  if (n->feature != "") {
    printf(" %s ", n->feature.c_str());
    pp(n->l); printf(" "); pp(n->r);
  }
  printf(")");
}

int main() {
  int N, L;
  f; sscanf(buf,"%d",&N);
  for (int it = 1; it <= N; it++) {
    printf("Case #%d:\n", it);
    ni = 0; nsi = 0;
    f; sscanf(buf, "%d", &L);

    // incremental parser
    node*last;
    for (int i = 0; i < L; i++) {
      f;
      string number;
      string feature;
      for (int j = 0; buf[j]; j++) {
	int state = nsi ? top()->state : 0;

	if (state == 0) { // waiting for '(' to start it all off
	  if (buf[j] == '(') { push(make()); top()->state = 1; }
	}
	else if (state == 1) { // opened tree, waiting for number
	  if (!ws(buf[j])) {number = ""; number += buf[j]; top()->state = 2;}
	}
	else if (state == 2) { // inside number
	  if (ws(buf[j]) || buf[j] == ')') {
	    top()->weight = pd(number);
	    top()->state = 3;
	    if (buf[j] == ')')
	      pop();
	  }
	  else number += buf[j];
	}
	else if (state == 3) { // after number.  could end tree, could go to feature
	  if (buf[j] == ')') {
	    pop();
	  }
	  else if (buf[j] >= 'a' && buf[j] <= 'z') {
	    j--;
	    feature = "";
	    top()->state = 4; // feature
	  }
	}
	else if (state == 4) { // in feature
	  if (buf[j] >= 'a' && buf[j] <= 'z') feature += buf[j];
	  else {top()->feature = feature; top()->state = 5;}
	}
	else if (state == 5 || state == 6) { // after feature, waiting on subtree (5/6)
	  if (buf[j] == '(') {
	    node* n = make();
	    n->state = 1;
	    if (state == 5)
	      top()->l = n;
	    else
	      top()->r = n;
	    top()->state++;
	    push(n);
	  }
	}
	else if (state == 7) { // after both subtrees, waiting on close paren to pop
	  if (buf[j] == ')') {
	    last = pop();
	  }
	}
      }
    }

    //pp(last); printf("\n");

    // now done!
    int A;
    f; sscanf(buf, "%d", &A);
    for (int i = 0; i < A; i++) {
      f;
      stringstream S; S << buf;
      string animal_name; int nf;
      S >> animal_name >> nf;
      
      string tmp;
      vector<string> features;
      for (int j = 0; j < nf; j++) {
	S >> tmp;
	features.push_back(tmp);
      }

      printf("%.8lf\n", eval(nodes, features));
    }
  }
}
