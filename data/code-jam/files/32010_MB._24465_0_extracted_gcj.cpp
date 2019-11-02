#include <string>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stack>
#include <queue>
#include <algorithm>
#include <deque>
#include <utility>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
#include <cassert>
#include <set>
using namespace std;
#define sz size()
#define vi vector<int>
#define vs vector<string>
#define dsz size()-1
#define pb push_back
#define minn(a,b) (a) = ((a) > (b) ? (b) : (a))
#define maxn(a,b) (a) = ((a) < (b) ? (b) : (a))
#define FUP(ii,ss,ff) for ((ii) = (ss);(ii) <= (ff);(ii)++)
#define FDOWN(ii,ss,ff) for ((ii) = (ss);(ii) >= (ff);(ii)--)
#define FALL(ii, vv) for ((ii) = 0;signed (ii) <= signed ((vv).dsz);(ii)++)
#define ITERATE(__it,__container) for(__it=__container.begin(); __it!=__container.end(); __it++)
#define EPS 1e-12
#define INF 2147483647
#define sgn(x) ((x)>0 ? 1 : (x)==0 ? 0 : -1)
#define sq(x) ((x)*(x))
#define sorta(a) sort(a.begin(), a.end())
#define cleara(a, b) memset(a, b, sizeof(a))
#define ALL(a) a.begin(), a.end()
#define absd(a) ((a)<0?-(a):(a))
#define absm absd
#define mp make_pair

#define RI io.readInt<int>()

typedef unsigned int uint;
typedef long long ll;


#define READ_BUF_SIZE 100*1024
#define WRITE_BUF_SIZE 100*1024
 
class StdIO{
public:
	StdIO(){
		pos_read = 0;
		pos_write = 0;
		buf_read_len = 0;
		eof = false;
		memset(white_table, false, 256);
		int i;
		FUP(i,0,32)
			white_table[i] = true;
	}
	~StdIO(){
		flushWrite();
	}
	bool isEOF(){
		if (eof)
			return true;
		if (pos_read == buf_read_len)
			return eof = !_read();
		return false;
	}
	int getChar(){
		if(isEOF())
			return -1;
		return buf_read[pos_read++];
	}
	int getCharNonWhite(){
		int c;
		while ((c=getChar()) != -1 && white_table[c])
			;
		return c;
	}
	void ungetChar(){
		if (eof)
			return;
		if(!(pos_read--))
			pos_read = 0;
	}
	void flushWrite(){
		if (!pos_write)
			return;
		fwrite(buf_write, 1, pos_write, stdout);
		pos_write = 0;
	}
	template<typename T> T readInt(){
		T ret=0;
		trimWhite();
		int c=getChar();
		bool sg=false;
		if (c == -1)
			return 0;
		else if (c == '-'){
			sg = true;
			c = getChar();
		}
		do
			ret = (ret<<3) + (ret<<1) + c-'0';
		while ((c=getChar()) >= '0' && c <= '9');
		ungetChar();
		return sg?-ret:ret;
	}
	string readString(){
		string ret;
		trimWhite();
		int c;
		while ((c=getChar()) != -1 && !white_table[c])
			ret += (char)c;
		ungetChar();
		return ret;
	}
	char* readStringLow(){
		throw 0; // unimplemented
	}
	void readStringLow(char* str){
		trimWhite();
		int c;
		int pos = 0;
		while ((c=getChar()) != -1 && !white_table[c])
			str[pos++] = (char)c;
		str[pos] = 0;
		ungetChar();
	}
	void trimWhite(){
		int c;
		while ((c=getChar())!=-1 && white_table[c])
			;
		ungetChar();
	}
	void setWhite(string chars, bool areWhite){
		for(int i = chars.size()-1; i >= 0; i--)
			white_table[chars[i]] = areWhite;
	}
	void putChar(char c){
		if (pos_write == WRITE_BUF_SIZE)
			flushWrite();
		buf_write[pos_write++] = c;
	}
	void writeString(string& str){
		writeString(str.c_str(), str.size());
	}
	void writeString(const char* str){
		writeString(str, strlen(str));
	}
	void writeString(const char* str, int len){
		int i;
		FUP(i,0,len-1)
			putChar(str[i]);
	}
	template<typename T> void writeInt(T num){
		if (num < 0){
			num = -num;
			putChar('-');
		}
		string ret;
		if (num == 0)
			ret += '0';
		while (num != 0){
			ret += '0' + (num%10);
			num/=10;
		}
		reverse(ALL(ret));
		writeString(ret);
	}
private:
	int _read(){
		buf_read_len -= pos_read;
		if (buf_read_len)
			memcpy(buf_read, buf_read + pos_read, buf_read_len);
		pos_read = 0;
		return buf_read_len += fread(buf_read + buf_read_len, 1, READ_BUF_SIZE - buf_read_len, stdin);
	}
	bool eof;
	unsigned char buf_read[READ_BUF_SIZE];
	unsigned char buf_write[WRITE_BUF_SIZE];
	bool white_table[256];
	int pos_read;
	int pos_write;
	int buf_read_len;
};
 
StdIO io;



int parent[1000];
vi childs[1000];

vi gr[1000];
void go(int v, int p){
	int i;
	parent[v] = p;
	FALL(i, gr[v])if (gr[v][i]!=p){
		go(gr[v][i], v);
		childs[v].pb(gr[v][i]);
	}
}

int k;
#define MOD 1000000009

long long col(int v, int prev){
	int i,cols;
	long long ret;
	if (parent[v] == -1)
		cols = k;
	else{
		cols = k - (childs[parent[v]].size());
		if (parent[parent[v]] != -1)
			cols--;
	}
	ret = 1;
	FALL(i, childs[v]){
		ret *= cols-i;
		if (cols-i < 0)
			ret = 0;
		ret %= MOD;
		ret *= col(childs[v][i], i);
		ret %= MOD;
	}
	return ret;
}

int main(){
	int TT=RI,tt;
	int N;
	FUP(tt,1,TT){
		int i;
		FUP(i,0,999)childs[i].clear();
		FUP(i,0,999)gr[i].clear();
		FUP(i,0,999)parent[i] = -1;
		N = RI;
		k = RI;
		FUP(i,1,N-1){
			int a = RI-1;
			int b = RI-1;
			gr[a].pb(b);
			gr[b].pb(a);
		}
		go(0, -1);
		printf("Case #%d: %d", tt, (int)col(0,0));
		printf("\n");
	}
	return 0;
}
