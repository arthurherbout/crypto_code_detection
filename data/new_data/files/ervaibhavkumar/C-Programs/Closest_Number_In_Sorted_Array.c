#include <stdio.h>
int main()
{
    int n , i;
    printf("Enter number of elements : ");
    scanf("%d",&n);
    int arr[n];
    printf("Enter the element in sorted way : ");
    for(i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }
    int target , index1 , index2;
    printf("Enter the target number : ");
    scanf("%d",&target);
    if(target<=arr[0])
    {
    	printf("Closest number is %d",arr[0]);
    }
    else if(target>=arr[n-1])
    {
        printf("Closest number is %d",arr[n-1]);
    }
    else
    {
        for(i=0;i<n-1;i++)
        {
            if(arr[i]<target && arr[i+1]>target)
            {
                index1 = i;
                index2 = i+1;
                break;
            }
        }
        if((target-arr[index1]) > (target-arr[index2]))
        {
            printf("Closest number is %d",arr[index2]);
        }
        else
        {
            printf("Closest number is %d",arr[index1]);
        }
    }
    return 0;
}
