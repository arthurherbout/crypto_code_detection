#include<stdio.h>
#include<stdlib.h>
struct str_engines
{
char name[100];
int index;
};

//search engine data structure: each engine in S engines
struct str_engines se[101];

//N in problem
char s_not[4];
//S in problem
char s_nose[4];
//Q in problem
char s_nostrs[20];
//  N  ,   S   ,   Q
int not,nose,nostrs;

char strname[100];
char temp[100];

int arr[1001];

int seekpos=0,flag = 0;

void gotillnextline(FILE *fp)
{
char c;
do
{
c = fgetc(fp);
}while(c=='\n'||c=='\0'||c=='\r');
fseek(fp,-1,SEEK_CUR);
}
int getindexofengine(char *s)
{
 int i,n,j;
  for(i=1;i<=nose;i++)
  {
    n = strcmp(s,se[i].name);
//    printf("strlen1=%d(%s)\n",strlen(s),s);
    if(!n)
  	break;

  }

  return i;
}

int getpos(int i)
{
int j;
 if(i>9)
 	j=2;
 else if(i<=9)
 	j =1;
 return j;
 	
}
void copy(char *res,char *src)
{
  int len = 0,i;
  len = strlen(src);

  for(i=0;((i<=(len-1))&&(src[i]!='\r')&&(src[i]!='\n'));i++)
  {
    res[i] = src[i];
  }
     res[i] = '\0';
}
int main()
{

FILE *fp,*fw;
int i,j,count =0,k,len=0,m;
int swneeded = 0,t;

for(i=1;i<=1000;i++)
	arr[i]=0;

//to read no of test cases
fp = fopen("final.txt","r");
fw = fopen("final_out.txt","w");

fgets(s_not,3,fp);
gotillnextline(fp);
not = atoi(s_not);
//printf("no of test cases= %d \n",not);

for(k = 1;k<=not;k++)
{
    for(i=1;i<=1000;i++)
	arr[i]=0;

//to read no of search engines
fgets(s_nose,5,fp);
nose = atoi(s_nose);
//printf("no of search engines = %d \n",nose);
gotillnextline(fp);

//to read all engine names 1 by 1
//printf("Search engines are as below:\n");
for(i=1;i<=nose;i++)
 {
   se[i].index = i;
   fgets(temp,100,fp);
   gotillnextline(fp);
//  printf("string got %s\n",temp);
   copy(se[i].name,temp);
   //printf("%d: %s\n",i,se[i].name);
 }


//to read search strings names 1 by 1
fgets(s_nostrs,5,fp);
gotillnextline(fp);
nostrs = atoi(s_nostrs);
//printf("no of search strings are %d \n",nostrs);

 
//printf("search strings are as below:\n");
 for(i=1;i<=nostrs;i++)
 {
 //reading string name
         fgets(temp,100,fp);
        gotillnextline(fp);
         copy(strname,temp);
        //printf("%d: %s \n",i,strname);
         for(m=1;m<=100;m++)
         temp[m]='\0';
		 
         j = getindexofengine(strname);
	  if(arr[j]==0)
	  {
	    count++;
	    arr[j]=1;
	//    printf("count is incremented:%d\n",count);
	  }
	  if(count ==  nose)
          {
            for(m=1;m<=1000;m++)
            arr[m]=0;
	     arr[j]=1;
            swneeded++;
            count = 1;
	     j =0;
           // printf("SWITCHED\n");
          }        
         
 }

 fprintf(fw,"Case #%d: %d\n",k,swneeded);
 
  swneeded = 0;
  count = 0;

}

fclose(fp);
fclose(fw);
	return 0;
}

