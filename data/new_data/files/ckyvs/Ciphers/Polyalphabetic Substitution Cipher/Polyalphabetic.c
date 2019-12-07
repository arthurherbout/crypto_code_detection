#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int getdat(char *a,char *key)
{
    int len;
    printf("Type the message :\n");
    fgets(a,40,stdin);
    len=strlen(a);
    printf("Enter shift keyword :\n");
    fgets(key,20,stdin);
    key[strlen(key)-1]='\0';
    return len;
}

int main()
{
    char *a,*key,c;
    int n,i=0,v,len,j=0,count=0;;
    a = (char*)malloc(100*sizeof(char));
    key = (char*)malloc(20*sizeof(char));
    while(3)
    {
    printf("1.Encrypt 2.Decrypt 3.Exit\n:");
    scanf("%d",&v);
    getchar();
    switch (v)
    {
        case 1:
                len = getdat(a,key);
                for(i=0;i<len;i++)
                {
                    c=a[i];
                    if(c > 96 && c < 123)
                    {
                    if(key[j] == '\0')
                        j = 0;
                     n = (int)key[j] - 97;
                    c = (c > 96 && c < 123)?(int) (c + n):c;
                    j++;
                    
                    if((int)c > 122)
                        c = ((int)c - 97)%26 + 97;
                    }
                    a[i]=c;
                }
                a[len-1]='\0';
                j=0;
                printf("%s\n",a);
                break;
        case 2:
                len = getdat(a,key);
                for(i=0;i<len;i++)
                {
                    c=a[i];
                    if (c > 96 && c < 123)
                    {
                    if(key[j] == '\0')
                        j = 0;
                     n = (int)key[j] - 97;
                    c = (int) (c - n);
                    j++;
                    
                    if((int)c < 97)
                        c = ((int)c - 97 + 26)%26 + 97;
                    }
                    a[i]=c;
                }
                a[len-1]='\0';
                j=0;
                printf("%s\n",a);
                break;
        case 3:
                exit(0);
                break;
        default:
                printf("Select a valid choice \n");
    }
    }
    return 0;
}
