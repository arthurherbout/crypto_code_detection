#pragma warning(disable:4996)

#include <stdio.h>
#include <algorithm>
#include <functional>
#include <vector>
#include <tuple>
#include <string>
#include <map>
#include <set>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

using namespace std;
typedef long long LL;
typedef function<int(int)> VALF;

#define pb push_back
#define mp make_pair
#define mt make_tuple
#define SZ(V) ((int)((V).size()))

#define fr first
#define sd second

enum NodeType {
	none = 0,
	digit,
	concat,
	star,
	or
};

class Node {
public:
	Node **childs;
	int cn;
	NodeType type;
	char data;
	virtual ~Node() {
		for (int i = 0; i < cn; i++) delete childs[i];
	}
	int s, e;
} *root;

char regex[128];
int rlen;

char num[128];
int len = 0;
int bit_to_num[1 << 16];
int bitnum(unsigned int bit) {
	if (bit >= (1 << 16)) return bit_to_num[bit >> 16] + 16;
	return bit_to_num[bit];
}
unsigned int getbit(unsigned int i) {
	return 1 << i;
}
unsigned int low_bit(unsigned int i) {
	return (i ^ (i - 1)) & i;
}

NodeType matregex[128][128];
bool matp[128][128];
int stk[128], head;
int level[128];

int node_number = 0;
Node *nodemap[128];
Node *nodemap2[128];
Node *parent[128];

unsigned int spread_internal(unsigned int S) {
	unsigned int rS = 0;
	bool did[128];
	memset(did, 0, sizeof(did));
	while(S>0) {
		unsigned int lbit = low_bit(S);
		S -= lbit;
		unsigned int who = bitnum(lbit);
		if (did[who]) continue;
		did[who] = true;
		if (who == rlen) {
			rS |= getbit(who);
			continue;
		}
		if (who > rlen) continue;
		if (nodemap[who] == NULL && nodemap2[who] == NULL) { // no map
			if (regex[who] == '|') {
				S |= getbit(parent[who]->e);
			}
			else {
				S |= getbit(who + 1);
			}
			continue;
		}
		if (nodemap[who] != NULL) {
			int s = nodemap[who]->s;
			int e = nodemap[who]->e;
			switch (nodemap[who]->type) {
			case NodeType::digit:
				rS |= getbit(who);
				break;
			case NodeType::star:
				S |= getbit(s + 1);
				S |= getbit(e + 1);
				break;
			case NodeType:: or:
				for (int i = 0; i < nodemap[who]->cn; i++) {
					S |= getbit(nodemap[who]->childs[i]->s);
				}
				break;
			}
		}
		else if (nodemap2[who] != NULL) { // ) or *
			switch (nodemap2[who]->type) {
			case NodeType::star:
				S |= getbit(nodemap2[who]->s);
				break;
			case NodeType::or:
				S |= getbit(who + 1);
				break;
			}
		}
	}
	return rS;
}

unsigned int spreadU[1 << 16];
unsigned int spreadD[1 << 16];
void initSpread() {
	memset(spreadU, 0, sizeof(spreadU));
	memset(spreadD, 0, sizeof(spreadD));
	for (unsigned int i = 0; i < 16; i++) {
		spreadD[1 << i] = spread_internal(1 << i);
		spreadU[1 << i] = spread_internal((1 << i) << 16);
	}
	for (unsigned int i = 1; i < (1 << 16); i++) {
		unsigned int lbit = low_bit(i);
		spreadD[i] |= spreadD[i^lbit];
		spreadD[i] |= spreadD[lbit];

		spreadU[i] |= spreadU[i^lbit];
		spreadU[i] |= spreadU[lbit];
	}
}
const unsigned int mask_spread = (1 << 16) - 1;
unsigned int spread(unsigned int S) {
	unsigned int rS = spreadD[S & mask_spread] | spreadU[S>>16];
	return rS;
}

unsigned int numdigit[10];
map<unsigned int, LL> hash1[20];
LL get_length(int len, unsigned int S) {
	S = spread(S);
	if (S == 0) return 0ll;
	if (len == 0) {
		if (S & getbit(rlen)) return 1;
		return 0;
	}
	if (hash1[len].find(S) != hash1[len].end()) return hash1[len][S];
	LL ret = 0;
	for (int i = 0; i < 10; i++) {
		unsigned int nS = (S & numdigit[i]) << 1;
		if (nS != 0) {
			ret += get_length(len - 1, nS);
		}
	}
	hash1[len][S] = ret;
	return ret;
}

map<unsigned int, LL> hash2[20];
LL get_le(unsigned int p, unsigned int S, bool no_leading_zero) {
	S = spread(S);
	if (S == 0) return 0ll;
	if (p == len) {
		if (S & getbit(rlen)) return 1;
		return 0;
	}
	if (!no_leading_zero) {
		if (hash2[p].find(S) != hash2[p].end()) return hash2[p][S];
	}
	LL ret = 0;
	for (int i = 0; i < 10; i++) {
		if (i == 0 && no_leading_zero) continue;
		char digit = '0' + i;
		unsigned int nS = (S & numdigit[i]) << 1;
		if (digit < num[p]) {
			ret += get_length((len-p)-1, nS);
		}
		else if (digit == num[p]) {
			ret += get_le(p + 1, nS, false);
			break;
		}
		else {
			break;
		}
	}
	if (!no_leading_zero) {
		hash2[p][S] = ret;
	}
	return ret;
}

LL getCount(LL mx) {
	sprintf(num, "%lld", mx);
	len = strlen(num);
	for (int i = 0; i < 20; i++) {
		hash1[i].clear();
		hash2[i].clear();
	}
	LL ret = get_le(0, 1, true);
	unsigned int S = 1;
	S = spread(S);
	for (int i = 1; i < 10; i++) {
		unsigned int nS = (S & numdigit[i]) << 1;
		for (int t = 0; t + 1 < len; t++) {
			ret += get_length(t, nS);
		}
	}
	return ret;
}
bool isdigit(char a) {
	return '0' <= a && a <= '9';
}

Node *createNode(int s, int e) {
	Node *ret = new Node();
	nodemap[s] = ret;
	nodemap2[e] = ret;
	ret->s = s;
	ret->e = e;
	ret->type = matregex[s][e];
	ret->cn = 0;
	switch (ret->type) {
	case NodeType::digit:
		ret->data = regex[s];
		break;
	case NodeType::concat:
		for (int k = s; k < e; k++) {
			if (matregex[s][k] && matregex[k + 1][e]) {
				ret->cn = 2;
				ret->childs = new Node*[2];
				ret->childs[0] = createNode(s, k);
				ret->childs[1] = createNode(k + 1, e);
				break;
			}
		}
		break;
	case NodeType::star:
		ret->cn = 1;
		ret->childs = new Node*[1];
		ret->childs[0] = createNode(s + 1, e - 2);
		break;
	case NodeType::or:
		int now_s = s + 1;
		vector<Node*> childs;
		for (int k = s + 1; k < e; k++) {
			if (level[k] == level[s] && regex[k] == '|') {
				parent[k] = ret;
				childs.push_back(createNode(now_s, k - 1));
				now_s = k + 1;
			}
		}
		childs.push_back(createNode(now_s, e - 1));
		ret->cn = childs.size();
		ret->childs = new Node*[ret->cn];
		for (int k = 0; k < childs.size(); k++) {
			ret->childs[k] = childs[k];
		}
		break;
	}
	return ret;
}
Node *parse() {
	memset(matp, 0, sizeof(matp));
	memset(matregex, 0, sizeof(matregex));
	memset(nodemap, 0, sizeof(nodemap));
	memset(nodemap2, 0, sizeof(nodemap2));
	memset(parent, 0, sizeof(parent));
	rlen = strlen(regex);
	for (int i = 0; i < 10; i++) {
		numdigit[i] = 0;
		char num = '0' + i;
		for (int j = 0; j < rlen; j++) {
			if (regex[j] == num) numdigit[i] |= getbit(j);
		}
	}
	head = 0;
	for (int i = 0; i < rlen; i++) {
		if (regex[i] == '(') stk[head++] = i;
		level[i] = head;
		if (regex[i] == ')') matp[stk[--head]][i] = true;
		if (isdigit(regex[i])) {
			matregex[i][i] = NodeType::digit;
		}
	}
	for (int i = 2; i <= rlen; i++) {
		for (int j = 0; j < rlen - i + 1; j++) {
			int s = j, e = j + i - 1;
			for (int k = s; k < e; k++) {
				if (matregex[s][k] && matregex[k + 1][e]) matregex[s][e] = NodeType::concat; // concat
			}
			if (i > 3) {
				if (regex[s] == '(' && regex[e] == '*' && matp[s][e - 1]) matregex[s][e] = NodeType::star; // star
				if (regex[s] == '(' && regex[e] == ')' && matp[s][e] && (e + 1 == rlen || regex[e + 1] != '*')) matregex[s][e] = NodeType::or; // or
			}
		}
	}

	return createNode(0, rlen - 1);
}

int main() {
	for (int i = 0; i < 16; i++) {
		bit_to_num[1 << i] = i;
	}
	//freopen("test.in", "r", stdin);
	freopen("A-small-attempt1.in", "r", stdin);
	freopen("A-small-attempt1.out", "w", stdout);
	int T;
	scanf("%d", &T);
	for (int tc = 1; tc <= T; tc++) {
		fprintf(stderr, "%d\n", tc);
		LL A, B;
		scanf("%lld %lld", &A, &B);
		scanf("%s", regex);
		root = parse();
		initSpread();
		LL Bcount = getCount(B);
		LL Acount = getCount(A - 1);
		LL sol = Bcount - Acount;
		printf("Case #%d: %lld\n", tc, sol);
		delete root;
	}
	return 0;
}