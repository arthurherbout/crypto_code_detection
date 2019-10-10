#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {
    
    long long int t, t1, R, C, W, answer;
    scanf("%lld", &t);
    t1 = t;
    while ( t-- )
    {
        scanf("%lld %lld %lld", &R, &C, &W);
        if(W == C)
            answer = R-1+W;
        else if(W == 1)
            answer = R*C;
        else
            answer = ((R*(C-1))/W)+W; 
        printf("Case #%lld: %lld\n",(t1-t), answer);
    }
    return 0;
}

