#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// UNWEIGHTED BIPARTITE MATCHING
////////////////////////////////////////////////////////////////////////////////
// Matching resultante: para matching[i]!=-1, (i, matching[i]).
// tab[i][j] -> de i para j. m linhas e n colunas. (mtx no sentido matematico)
////////////////////////////////////////////////////////////////////////////////
// Ideias / extensoes:
// - Para determinar se a aresta encontra-se em algum perfect matching, force
//   ela estar no matching e de um augment() no vertice que sobra (nao esquecer
//   de limpar seen)
////////////////////////////////////////////////////////////////////////////////
const int nmax = 250;

struct UBMatching
{
  int m, n, size, matching[nmax], seen[nmax];
  int tab[nmax][nmax];
  bool augment(int v)
  {
    for(int i=0;i<m;i++)
      if(tab[i][v] && !seen[i])
	{
	  seen[i]=1;
	  if(matching[i]<0 || augment(matching[i]))
 	    { matching[i]=v; return 1; }
	}
    return 0;
  }
  void init(int nm, int nn)
  {
    m = nm; n = nn; size = 0;
    memset(tab, 0, sizeof(tab));
    memset(matching, -1, sizeof(matching));
  }
  void match()
  {
    for(int i=0;i<n;i++)
      { memset(seen, 0, sizeof(seen)); size += augment(i); }
  }
} ubmcross, ubmdiag;

typedef struct { char type; int r, c; } Diff;

char originaltable[nmax][nmax];
char newtable[nmax][nmax];

int main() {
    int T;
    scanf(" %d", &T);
    for (int casenum = 1; casenum <= T; ++casenum) {
        int N, M;
        scanf(" %d %d", &N, &M);

        ubmcross.init(N, N);
        ubmdiag.init(2*N - 1, 2*N - 1);
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                ubmcross.tab[r][c] = 1;
                ubmdiag.tab[r + c][r - c + N - 1] = 1;
            }
        }

        memset(originaltable, 0, sizeof(originaltable));
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                originaltable[r][c] = '.';
            }
        }
        int originalscore = 0;
        for (int i = 0; i < M; ++i) {
            char Type;
            int R, C;
            scanf(" %c %d %d", &Type, &R, &C);
            --R; --C;
            originaltable[R][C] = Type;

            if (Type == '+' || Type == 'x') {
                originalscore += 1;
            } else {
                assert(Type == 'o');
                originalscore += 2;
            }

            if (Type == '+' || Type == 'o') {
                for (int k = 0; k < 2*N - 1; ++k) {
                    ubmdiag.tab[R + C][k] = 0;
                    ubmdiag.tab[k][R - C + N - 1] = 0;
                    // ubmdiag.tab[R + k][R - k + N - 1] = 0;
                    // ubmdiag.tab[k + C][k - C + N - 1] = 0;
                }
            }
            if (Type == 'x' || Type == 'o') {
                for (int k = 0; k < N; ++k) {
                    ubmcross.tab[R][k] = 0;
                    ubmcross.tab[k][C] = 0;
                }
            }
        }

        memcpy(newtable, originaltable, sizeof(originaltable));
        ubmcross.match();
        ubmdiag.match();

        for (int i = 0; i < N; ++i) {
            int c = ubmcross.matching[i];
            if (c != -1) {
                if (newtable[i][c] == '.') {
                    newtable[i][c] = 'x';
                } else {
                    assert(newtable[i][c] == '+');
                    newtable[i][c] = 'o';
                }
            }
        }
        // printf("cross/was:");
        // for (int r = 0; r < N; ++r) {
        //     printf("%s\n", originaltable[r]);
        // }
        // printf("cross/is:");
        // for (int r = 0; r < N; ++r) {
        //     printf("%s\n", newtable[r]);
        // }
        for (int i = 0; i < 2*N - 1; ++i) {
            int beta = ubmdiag.matching[i];
            if (beta != -1) {
                int r = (i + beta - N + 1) / 2;
                int c = (i - beta + N - 1) / 2;
                assert(r + c == i);
                assert(r - c + N - 1 == beta);
                if (newtable[r][c] == '.') {
                    newtable[r][c] = '+';
                } else {
                    assert(newtable[r][c] == 'x');
                    newtable[r][c] = 'o';
                }
            }
        }

        // printf("DEBUG Original:\n");
        // for (int r = 0; r < N; ++r) {
        //     printf("%s\n", originaltable[r]);
        // }
        // printf("DEBUG Final:\n");
        // for (int r = 0; r < N; ++r) {
        //     printf("%s\n", newtable[r]);
        // }

        int newscore = 0;
        vector<Diff> differences;
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                if (newtable[r][c] == '+' || newtable[r][c] == 'x') {
                    newscore += 1;
                } else if (newtable[r][c] == 'o') {
                    newscore += 2;
                } else {
                    assert(newtable[r][c] == '.');
                }
                if (newtable[r][c] != originaltable[r][c]) {
                    assert(newtable[r][c] != '.');
                    if (newtable[r][c] == '+' || newtable[r][c] == 'x') {
                        assert(originaltable[r][c] == '.');
                    } else {
                        assert(newtable[r][c] == 'o');
                        assert(originaltable[r][c] == '+' || originaltable[r][c] == 'x' || originaltable[r][c] == '.');
                    }
                    Diff d = { newtable[r][c], r, c };
                    differences.push_back(d);
                }
            }
        }

        printf("Case #%d: %d %d\n", casenum, newscore, (int)differences.size());
        for (auto it : differences) {
            printf("%c %d %d\n", it.type, it.r + 1, it.c + 1);
        }
    }
}
