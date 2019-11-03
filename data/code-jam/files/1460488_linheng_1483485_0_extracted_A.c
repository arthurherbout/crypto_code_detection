#include<stdio.h>
#include<string.h>

int main() {
    freopen('A-small-attempt0.in','r')
    int t, i, t0 = 0;
    char s[102], p[102];
    char map[30]={'y','h','e','s','o','c','v','x','d','u','i','g','l','b','k','r','z','t','n','w','j','p','f','m','a','q'};
    scanf("%d", &t);
    while (getchar()!='\n');
    while (t--) {
        char ch;
        t0++;
        printf("Case #%d: ", t0);
        while ((ch = getchar())!='\n')
            if (ch == ' ') printf(" ");
            else printf("%c", map[ch-'a']);
        printf("\n");
    }
    return 0;
}
