#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
using namespace std;

#define foreach(iter, cont) \
    for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)
typedef long long LL;

struct Node {
    double weight;
    int feature;
    Node *left, *right;
};

map <string, int> flist;
int getFeature(string feature)
{
    if (flist.find(feature) != flist.end()) {
        return flist[feature];
    }

    int id = flist.size();
    flist[feature] = id;
    return id;
}

Node* parseTree(char* s, char* &npos)
{
    while (*s != '(') s++;
    s++;

    Node* p = new Node;
    while (*s <= 32) s++;
    sscanf(s, "%lf", &p->weight);
    while (*s > 32 && *s != ')') s++;
    //fprintf(stderr, "found weight %.4lf\n", p->weight);
    while (*s <= 32) s++;
    if (*s == ')') {
        //fprintf(stderr, "Found leaf\n");
        p->left = NULL;
        p->right = NULL;
        npos = s + 1;
        return p;
    }

    char feature[20];
    char* tmp = feature;
    while (*s >= 'a' && *s <= 'z') {
        tmp[0] = *s;
        tmp++;
        s++;
    }
    tmp[0] = 0;

    //fprintf(stderr, "Found feature %s, %d\n", feature, getFeature(feature));
    p->feature = getFeature(feature);

    p->left = parseTree(s, s);
    p->right = parseTree(s, s);
    npos = s;
    return p;
}

Node* readTree()
{
    int L;
    scanf("%d\t\n", &L);
    char *buf = new char [90 * 1000];
    char *cur = buf, *tmp;
    for (int i = 0; i < L; i++) {
        gets(cur);
        cur += strlen(cur);
    }
    Node* root = parseTree(buf, tmp);
    delete [] buf;
    return root;
}

void dumpTree(Node* t)
{
    printf("(%.2lf", t->weight);
    if (t->left) {
        printf(" %d\n", t->feature);
        dumpTree(t->left);
        dumpTree(t->right);
        printf(")\n");
    } else printf(")\n");
}

void solve()
{
    flist.clear();
    Node* root = readTree();
    //dumpTree(root);

    char buf[101];
    int A;

    scanf("%d", &A);
    for (int i = 0; i < A; i++) {
        scanf("%s", buf);
        int n;

        vector <int> features;
        scanf("%d", &n);
        for (int j = 0; j < n; j++) {
            scanf("%s", &buf);
            features.push_back(getFeature(buf));
        }
        //printf("features: ");
        //foreach(x, features) printf("%d ", *x);
        //printf("\n");

        bool *have = new bool[flist.size()];
        memset(have, false, flist.size());
        foreach (f, features) have[*f] = true;

        Node* cur = root;
        double p = root->weight;
        while (cur->left) {
            if (have[cur->feature])
                cur = cur->left;
            else
                cur = cur->right;
            p *= cur->weight;
        }
        printf("%.7lf\n", p);

        delete [] have;
    }
}

int main()
{
    int T;
    scanf("%d", &T);
    for (int i = 1; i <= T; i++) {
        printf("Case #%d:\n", i);
        solve();
    }
    return 0;
}
