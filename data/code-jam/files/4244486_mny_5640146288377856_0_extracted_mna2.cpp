#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {
    
    int t, t1, R, C, W, answer;
    scanf("%d", &t);
    t1 = t;
    while ( t-- )
    {
        scanf("%d %d %d", &R, &C, &W);
        if(W == C)
            answer = R-1+W;
        else if(W == 1)
            answer = R*C;
        else
            answer = ((R*(C-1))/W)+W; 
        printf("Case #%d: %d\n",(t1-t), answer);
    }
    return 0;
}

