#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *removeDuplicates(char *str)
{
    int count[128] = {0};
    int len = strlen(str) , j = 0;
    char *ans = (char *)malloc(len*sizeof(char));
    for(int i=0; i<len; i++)
    {
        count[str[i]]++;
    }
    for(int i=0; i<len; i++)
    {
        if(count[str[i]]==1)
        {
            ans[j] = str[i];
            j++;
        }
    }
    return ans;
}

int main()
{
    char *str = (char *) malloc(100*sizeof(char));
    printf("Enter the string : \n");
    scanf("\n%s",&str);
    char *ans = removeDuplicates(&str);
    printf("%s\n",ans);
    return 0;
}
