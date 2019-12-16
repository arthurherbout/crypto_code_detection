#include "stdafx.h"

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
#define EPS 1e-12
#define INF 2147483647
#define sgn(x) ((x)>0 ? 1 : (x)==0 ? 0 : -1)
#define sorta(a) sort(a.begin(), a.end())
#define cleara(a, b) memset(a, b, sizeof(a))
#define ALL(a) a.begin(), a.end()
#define absd(a) (a<0?-a:a)

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
#define RI io.readInt<int>()
int dest[100][100];
int que[10000][2];
int done[100][100];
int qs,qe;
int dx[]={0,0,1,-1};
int dy[]={1,-1,0,0};
string mapa[100];
int N,M;

void go(int x, int y){
	qs=0;qe=0;
	que[qs][0]=x;
	que[qs++][1]=y;
	dest[x][y]=0;
	int i;
	while (qs != qe){
		x = que[qe][0];
		y = que[qe++][1];
		FUP(i,0,3)
			if (x+dx[i] >= 0 &&
				y+dy[i] >= 0 &&
				x+dx[i] < N &&
				y+dy[i] < M &&
				mapa[x+dx[i]][y+dy[i]]!='.' &&
				dest[x+dx[i]][y+dy[i]] > dest[x][y]+1){
					dest[x+dx[i]][y+dy[i]] = dest[x][y]+1;
					que[qs][0]=x+dx[i];
					que[qs++][1]=y+dy[i];
			}
	}
}

int was[100][100];
int dfs(int x, int y){
	if (was[x][y])return 0;
	was[x][y] = 1;
	if ((x||y)&&mapa[x][y]=='T'){
		done[x][y]=1;
		return 1;
	}
	int i;
	FUP(i,0,3)
		if (x+dx[i] >= 0 &&
			y+dy[i] >= 0 &&
			x+dx[i] < N &&
			y+dy[i] < M &&
			dest[x][y]+1 == dest[x+dx[i]][y+dy[i]] && dfs(x+dx[i], y+dy[i])){
				done[x][y]=1;
				return 1;
		}

	return 0;
}

int main(){
	int TT=RI,tt;
	FUP(tt,1,TT){
		int i,j;
		FUP(i,0,44)FUP(j,0,44)dest[i][j]=1000000;
		cleara(done,0);
		cleara(was,0);
		N=RI;
		M=RI;
		FUP(i,0,N-1)
			mapa[i] = io.readString();
		go(0,0);
		dfs(0,0);
		int ret=0;
		FUP(i,0,N-1)FUP(j,0,M-1)if (done[i][j])ret += dest[i][j];
		FUP(i,0,N-1)FUP(j,0,M-1)if (mapa[i][j]=='T'&&(i||j))go(i,j);
		FUP(i,0,N-1)FUP(j,0,M-1)if (!done[i][j] && mapa[i][j]!='.')ret += dest[i][j];
		io.writeString("Case #");
		io.writeInt(tt);
		io.writeString(": ");
		io.writeInt(ret);
		io.putChar('\n');
	}
	return 0;
}
