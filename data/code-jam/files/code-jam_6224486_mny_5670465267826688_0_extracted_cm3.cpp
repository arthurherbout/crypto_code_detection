 
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
 
int main () {
     int t,t1,l,x,i,j,len,sign,flag;
    char *str,*temp,res;
    scanf("%d",&t);
    t1 = t;
    while(t--)
    {
        scanf ("%d %d",&l,&x);
    len=l*x;
	str=malloc(sizeof(char)*len+1);
	temp=malloc(sizeof(char)*l+1);
	//str="";temp="";
        scanf("%s",str);
	strcpy(temp,str);
	sign=1;flag=0;
	while(--x)
	 strcat(str,temp);
	res=str[0];
	for(i=1;i<=len;i++)//inner loop
	{
	if(sign==1)
           {
		if(res=='i' && flag==0)
			{res=str[i];i++;flag++;}
	        if(res=='j' && flag==1)
			{res=str[i];i++;flag++;}
		if(res=='k' && flag==2 && i==len)
 			{flag++;break;}
	   }	   
 
	if(res=='1')
	  res=str[i];
	else if(res=='i' && str[i]=='i')
	  {sign*=-1;res='1';}
	else if(res=='i' && str[i]=='j')
	  res='k';
	else if(res=='i' && str[i]=='k')
	  {sign*=-1;res='j';}
 
	else if(res=='j' && str[i]=='j')
	  {sign*=-1;res='1';}
	else if(res=='j' && str[i]=='k')
	  res='i';
	else if(res=='j' && str[i]=='i')
	  {sign*=-1;res='k';}
 
	else if(res=='k' && str[i]=='k')
	  {sign*=-1;res='1';}
	else if(res=='k' && str[i]=='i')
	  res='j';
	else if(res=='k' && str[i]=='j')
	  {sign*=-1;res='i';}	  
	}//inner loop ends
	if(flag==3)	
         printf ("Case #%d: YES\n",(t1-t));
	else
	 printf ("Case #%d: NO\n",(t1-t));
   }
    return 0;
}
