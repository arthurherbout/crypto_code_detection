#include <stdio.h>
#include <string.h>

void countMaxChar(char *str)
{
    int count[128] = {0};
    for(int i=0; i<strlen(str); i++)
    {
        count[str[i]]++;
    }
    int max = -1;
    char ans;
    for(int i=0; i<strlen(str); i++)
    {
        if(max<count[str[i]])
        {
            max = count[str[i]];
            ans = str[i];
        }
    }
    printf("Maximum char is %c occuring %d times \n",ans,max);
}

int main()
{
    char *str = (char *) malloc(100*sizeof(char));
    printf("Enter the string : \n");
    scanf("\n%s",&str);
    countMaxChar(&str);
    return 0;
}
