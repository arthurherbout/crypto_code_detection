#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n , i , j;
    printf("Enter number of elements : ");
    scanf("%d",&n);
    int arr[n];
    for(i=0; i<n; i++)
    {
        printf("Enter the element : ");
        scanf("%d",&arr[i]);
    }
    int val = 0;
    for(i=0; i<n-1; i++)
    {
        for(j=i+1; j<n; j++)
        {
            int xor = arr[i] & arr[j];
            if(xor>val)
            {
                val = xor;
            }
        }
    }
    printf("Maximum xor is : %d",val);
    return 0;
}
