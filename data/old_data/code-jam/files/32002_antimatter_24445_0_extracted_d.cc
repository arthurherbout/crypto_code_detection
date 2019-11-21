#include <cstdio>
#include <cstring>
using namespace std;

int H, W, R;
bool rock[105][105];
long long ways[105][105];

int main() {
    int N;
    scanf("%d", &N);
    for (int it = 1; it <= N; it++) {
        scanf("%d %d %d", &H, &W, &R);
        memset(rock, 0, sizeof(rock));
        for (int i = 0; i < R; i++) {
            int x, y;
            scanf("%d %d", &x, &y);
            rock[x-1][y-1] = 1;
        }

        memset(ways, 0, sizeof(ways));
        ways[0][0] = 1;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (!rock[i+2][j+1])
                    ways[i+2][j+1] = (ways[i+2][j+1] + ways[i][j]) % 10007;
                if (!rock[i+1][j+2])
                    ways[i+1][j+2] = (ways[i+1][j+2] + ways[i][j]) % 10007;
            }
        }
        printf("Case #%d: %lld\n", it, ways[H-1][W-1]);
    }
}
