#include <stdio.h>
#include <limits.h>
int main()
{
    int n;
    printf("Enter number of elements : ");
    scanf("%d",&n);
    int arr[n];
    printf("Enter the elements: ");
    for(int i=0 ; i<n ; i++)
    {
        scanf("%d",&arr[i]);
    }
    int max = INT_MIN , count = 0;
    for(int i=0; i<n; i++)
    {
        for(int j=i+1; j<n; j++)
        {
            if(arr[i] + arr[j] > max)
            {
                max = arr[i] + arr[j];
            }
        }
    }
    for(int i=0; i<n; i++)
    {
        for(int j = i+1 ; j<n ; j++)
        {
            if(arr[i] + arr[j] == max)
            {
                count++;
            }
        }
    }
    printf("Number of pairs with maximum sum is %d\n",count);
    return 0;
}
