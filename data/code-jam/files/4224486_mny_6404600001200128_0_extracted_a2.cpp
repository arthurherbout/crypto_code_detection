#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {
    
    int t, t1, n, i, c[100],k;
    scanf("%d", &t);
    t1 = t;
    while ( t-- )
    {
        scanf("%d",&n);
        for(i=0;i<n;i++)
        scanf("%d",&c[i]);
        
        int answer1=0,answer2=0;
        int dif =0;
        int k=0;
        
        for(i=1;i<n;i++)
            {
            	dif=c[i-1]-c[i];
			if(dif>=k)
				k=dif;
			if(dif>=0)
				answer1+=dif;
        }
        
        
        for(i=0;i<n-1;i++)
            {
            if(c[i]<=k)
				answer2+=c[i];
		    if(c[i]>k)
                answer2+=k;
        }
        printf("Case #%d: %d %d\n",(t1-t),answer1, answer2);
    }
    return 0;
}
