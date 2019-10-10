#include<stdio.h>

int isConsonent(char a)
{
    return (a != 'a' && a != 'e' && a != 'i' && a != 'o' && a != 'u' );
}

int main()
{
    int t,kas,i,j,count;
    char a[1000002];
    int n,last,len,pre,post;
    
    scanf("%d",&t);
    
    
    for(kas = 1; kas <= t; kas++)
    {
            scanf("%s %d",a,&n);
            len = strlen(a);
            count = last = 0;  
            for(i=0;i<len;i++)
            {
                            
                            if(isConsonent(a[i]))
                            {
                                                 for(j=1;j<n && i+j < len && isConsonent(a[i+j]);j++);
                                                 
                                                 if(j==n)
                                                 {
                                                         count++;                                                         
                                                         post = len - (i+j);
                                                         count += post;
                                                         pre = i-last;
                                                         if(i>0)
                                                         {
                                                                count +=  pre * (post+1);
                                                         }                                         
                                                         
                                                         last = i+1;
                                                 }
                            }
                            
            }

               
            printf("Case #%d: %d\r\n",kas,count);        
    }
    
    return 0;
}


              
              
              
              
