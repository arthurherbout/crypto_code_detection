#include <stdio.h>
#include <limits.h>
int main()
{
    int n;
    printf("Enter number of elements : ");
    scanf("%d",&n);
    int arr[n];
    printf("Enter the elements: \n");
    for(int i=0 ; i<n ; i++)
    {
        scanf("%d",&arr[i]);
    }
    int index = -1 , count = 0 , final = 0;
    for(int i=0; i<n; i++)
    {
        if(arr[i]<0)
        {
            if(final<count)
            {
                final = count;
                index = i-count;
                count = 0;
            }
        }
        else
        {
            count++;
        }
    }
    if(index == -1)
    {
        printf("No positive sequence detected\n");
    }
    else
    {
        printf("Length of Largest positive sequence is %d\nStarting index is %d",final,index);
    }
    return 0;
}
