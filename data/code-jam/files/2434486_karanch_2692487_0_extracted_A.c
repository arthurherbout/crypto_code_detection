#include<stdio.h>

#define LL long long

void swap(int *a, int *b)
{
     *a = *a + *b;
     *b = *a - *b;
     *a = *a - *b;
}

int min(int a,int b)
{
    return a < b ? a : b;
} 

int findCount(int *a,LL moot,int index,int count,int n)
{
              if(index == n)
                       return count;
                       
              if(moot > a[index])
                      return findCount(a,moot+a[index],index+1,count,n);
              
              return min(findCount(a,moot*2-1,index,count+1,n),
                         findCount(a,moot,index+1,count+1,n));
}

int main()
{
    int t,kas,i,j,count;
    int *a,n;
    LL moot,tempMoot;
    
    scanf("%d",&t);
    
    
    for(kas = 1; kas <= t; kas++)
    {
            scanf("%lld %d",&moot,&n);
            a =  malloc(sizeof(int)*n);
            for(i=0;i<n;i++)
            {
                            scanf("%d",&a[i]);
            }
            
            count = 0;
            for(i=0;i<n;i++)
            {
                            for(j=0;j<n-i-1;j++)
                            {
                                                if(a[j] > a[j+1])
                                                {
                                                        swap(&a[j],&a[j+1]);
                                                }
                            }
            }
            
            //for(i=0;i<n;i++)
//            {
//                            if(a[i] < moot)
//                                    moot+= a[i];
//                            else
//                            {
//                                    tempMoot = moot - 1;
//                                    if(moot + tempMoot > a[i])
//                                    {
//                                            moot += a[i]; 
//                                            moot += tempMoot;
//                                    }
//                                    count++;
//                            }                        
//            }
            
            printf("Case #%d: %d\r\n",kas,moot > 1 ? findCount(a,moot,0,0,n): n);        
    }
    
    return 0;
}


              
              
              
              
