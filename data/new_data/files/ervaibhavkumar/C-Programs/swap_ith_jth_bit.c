#include <stdio.h>

int main()
{
    int n;
    int i , j;
    printf("Enter the number whose bits is to be reversed : ");
    scanf("%d",&n);
    int num = n;
    printf("Enter first position : ");
    scanf("%d",&i);
    printf("Enter second position : ");
    scanf("%d",&j);
    if(((n>>i)&1) != ((n>>j)&1))
    {
        n = n ^ (1 << i);
        n = n ^ (1 << j);
    }
    printf("Now %d is %d\n",num,n);
    return 0;
}
