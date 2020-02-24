#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int getdat(char *a, int *n)//The function called to collect the data to Encrpt or Decrypt the data
{
    int len;
    printf("Type the message :\n");
    fgets(a,20,stdin);
    len=strlen(a);
    printf("Enter shift length: ");
    scanf("%d",n);
    getchar(); //to clear stdin cache
    return len;
}
int main()
{
    char *a;
    int n,i=0,v,len;
    a = (char*)malloc(100*sizeof(char));
    while(3)//Asking for the choice to Encrypt or Decrypt the data or to exit the program
    {
    printf("1.Encrypt 2.Decrypt 3.Exit\n:");
    scanf("%d",&v);
    getchar();
    switch (v)
    {
        case 1:
                len = getdat(a,&n); //call the the function to get the data
                n%=26; //this will prevent ascii values from going out of range for large values of n
                for(char c=a[0],i=0;c!='\0';c=a[i]) //logic for Encrypt the data
                {
                    c = (c > 96 && c < 123)?(int) c + n:c;
                    if((int)c > 122)
                        c = ((int)c - 97)%26 + 97;
                    a[i]=c;
                    i++;
                }
                a[len-1]='\0';
                printf("%d",len);
                printf("%s\n",a);
                break;
        case 2:
                len = getdat(a,&n);//calling the function to collect the data 
                n%=26;
                for(char c=a[0],i=0;c!='\0';c=a[i])//logic for Decrypt the data
                {
                    c = (c > 96 && c < 123)?(int) c - n:c;
                    if((int)c < 97)
                        c = ((int)c - 97 + 26)%26 + 97;
                    a[i]=c;
                    i++;
                }
                printf("%d",len);
                a[len-1]='\0';
                printf("%s\n",a);
                break;
        case 3:
                exit(0);//to come out of the program
                break;
        default:
                printf("Select a valid choice \n");//displace in case of wrong choice 
    }
    }
}
