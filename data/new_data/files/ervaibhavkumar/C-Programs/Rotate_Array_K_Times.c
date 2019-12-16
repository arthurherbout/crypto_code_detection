#include <stdio.h>

int main()
{
    int n;
    printf("Enter the number of elements : ");
    scanf("%d",&n);
    int arr[n];
    for(int i=0; i<n; i++)
    {
        scanf("%d",&arr[i]);
    }
    int k;
    printf("Enter number of rotation : ");
    scanf("%d",&k);
    while(k--)
    {
        int temp = arr[0];
        for(int i=0; i<n-1; i++)
        {
            arr[i] = arr[i+1];
        }
        arr[n-1] = temp;
    }
    printf("Array after %d rotations is : \n",k);
    for(int i=0; i<n; i++)
    {
        printf("%d\n",arr[i]);
    }
    return 0;
}
