#include <stdio.h>
#include <limits.h>
void swap(int *a , int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void sortArray(int arr[] , int n)
{
    int min;
    for(int i=0; i<n-1; i++)
    {
        min = i;
        for(int j=i+1; j<n; j++)
        {
            if(arr[j]<arr[min])
            {
                swap(&arr[j],&arr[min]);
            }
        }
    }
}
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
    sortArray(arr,n);
    int start , end;
    int count = 0 , final = 0 , finalArr[n] , newIndex = 0;
    for(int i=0; i<n-1; i++)
    {
        count = 0;
        for(int j=i; j<n-1; j++)
        {
            if(arr[j]+1==arr[j+1])
            {
                count++;
            }
            else
            {
                start = j-count;
                end = j;
                i += count;
                break;
            }
        }
        if(end-start > final)
        {
            newIndex=0;
            for(int k=start; k<end; k++,newIndex++)
            {
                finalArr[newIndex] = arr[k];
            }
        }
    }
    if(newIndex == 0)
    {
        printf("No Sequence detected\n");
    }
    else
    {
        int i;
        printf("Sequence is ");
        for(i=0; i<newIndex; i++)
        {
            printf("%d , ",finalArr[i]);
        }
        printf("%d\n",finalArr[i]+1);
    }
    return 0;
}
