#include <stdio.h>
#include <string.h>

void swap(char *str1 , char *str2)
{
    char *temp = (char *)malloc((strlen(str1) + 1) * sizeof(char));
    strcpy(temp, str1);
    strcpy(str1, str2);
    strcpy(str2, temp);
    free(temp);
}
void sort(char *arr[] , int n)
{
    if(strcmp(&arr[0],&arr[1])==1)
    {
        swap(&arr[0],&arr[1]);
    }
    // else if(strcmp(&arr[0],&arr[1])==)
    //printf("%d\n",strcmp("Hi","hi"));
}
int main()
{
    int n;
    printf("how many names u want to enter ? : ");
    scanf("%d",&n);
    char *arr[n];
    printf("Enter the names: \n");
    for(int i=0 ; i<n ; i++)
    {
        scanf("\n%s",&arr[i]);
    }

    sort(&arr,n);

    for(int i=0; i<n; i++)
    {
        printf("%s\n",&arr[i]);
    }
    return 0;
}
