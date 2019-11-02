#include<stdio.h>
#include<string.h>
    FILE *fpr,*fpw; char str[]="welcome to code jam";
    char input[501];int output;

int match(int len,int j,int lens,int k)
{
    if(input[j]==str[k])
    {
                     if(j<len-1&&k<lens-1)
                     {
                                match(len,j+1,lens,k+1);
                                match(len,j+1,lens,k);
                     }
                     else if(j==len-1)
                         {
                                      return -1;
                         }
                     else if(k==lens-1)
                         {             
                                       output++; 
                                       if(j<len-1)
                                                  match(len,j+1,lens,k);
                                       return 1;
                         }

    }
    
    else
    {
        if(j<len-1&&k<=lens-1)
        {
                             match(len,j+1,lens,k);
        }
        else if(j==len-1)
        {
             return -1;
        }

    }
    return -1;
}

int main()
{
    int N,i,l,j,k,m,p,q;

    if((fpr=fopen("C-small.in","r"))==NULL)
                                          {printf("cant open input file \n");return 1;}
    if((fpw=fopen("C-small.out","w"))==NULL)
                                          {printf("cant open output file \n");return 1;}
fflush(NULL);
    
    fscanf(fpr,"%d\n",&N);
//    fprintf(stdout,"%d",N);
    for (i=0;i<N;i++)
    {
        
        //l=getline(input,500,fpr);
        fgets(input,500,fpr);
        l=strlen(input);
    //    fprintf(stdout,"%s %d",input,l);
        output=0;
        match(l,0,19,0);
        /*
        for(j=0,q=0;j<l;j++)
        {
          for(p=j,k=q;p<l;p++)
          {
               if(input[p]==s[k])
                 {
                                    m=p+1;k++;
                                    for(;m<l&&k<19;)
                                    {
                                                   if(input[m]==s[k])
                                                                     {
                                                                     if(k==18)
                                                                              {output=(output+1)%10000;break;        }
                                                                     else
                                                                         k++;m++;
                                                                     }
                                                   else if(input[m]!=s[k])
                                                   {
                                                        if(m==l-1){break;}
                                                        else  {m++;}
                                                   }
                                    }
                 }
                 else{continue;}
                            
           }
        }*/
        fprintf(fpw,"Case #%d: %0.4d\n",i+1,output);
    }
     //fprintf(fpw,"Case #%d: %0.4d\n",i+1,output);
    fclose(fpr);
    fclose(fpw);
    return 0;
}
