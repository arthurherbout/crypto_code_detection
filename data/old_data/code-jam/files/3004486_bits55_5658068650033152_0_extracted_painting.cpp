#include<stdio.h>
#include<algorithm>
#define FILEI                    freopen("input.txt","r",stdin);
#define FILEO                    freopen("output.txt","w",stdout);

using namespace std;
int n,mi,mj;
int check(char a[][51],char b[][51])
{
  int i,j,flag=1;
  for(i=0;i<n&&flag;i++)
   for(j=0;j<n&&flag;j++)
   if(a[i][j]!=b[i][j])
    { mi=i;
    mj=j;
    flag=0;
    break;
   }
   if(i==n&&j==n)
   return 1;
   else
   return 0;
}
void change(char a[][51],char b[][51],int p,int q)
{
            int i,j;
            char c=a[mi][mj];
            for(i=mi;i<=p;i++)
            for(j=mj;j<=q;j++)
              b[i][j]=c;    
            }
main()
{
      int i,j,p,q,c[2501][5],tp[2501][5],l,min=2501,k,temp[51],count,stop,max,count2;
      char a[51][51],b[51][51];
      //FILEI;
      scanf("%d",&n);
      for(i=0;i<n;i++)
      scanf("%s",a[i]);
      /*
      for(i=0;i<n;i++){
       for(j=0;j<n;j++)
       b[i][j]='W';
       b[i][n]='\0';
       }
       l=0;
      while(!check(a,b))
      {
     k=0;
      p=mj;q=mi;
      for(i=mi;i<n;i++){
       for(j=mj;j<n;j++)
       if(a[mi][mj]==a[i][j])
       {
       if(j==n-1)
        temp[k++]=n-1;
       continue;
       }
       else{
            if(j!=mj)
       temp[k++]=j-1;
       break;}
       }
       sort(temp,temp+k);
  //    for(i=0;i<k;i++)
    //  printf("%d ",temp[i]);
      // printf("\n");
       count=1;
       max=1;
       p=temp[0];
       for(i=1;i<k;i++)
       {
         if(temp[i]==temp[i-1])
         count++;
         else{
         if(max<=count)
         {p=temp[i-1];
         max=count;}
         //printf("::%d\n",p);
         count=1;
         }
         }
         if(max==1)
         p=temp[k-1];
         if(max<n/2)
         {
      
      for(i=mi;i<n;i++)
      {             
             if(a[mi][mj]==a[i][mj])
             continue;
             else
             {
                 q=i-1;
                 break;}
                 }}
                 else{      
                         
       // printf("::%d\n",p);
        for(i=mi;i<n;i++){
                          count=mj;
       for(j=mj;j<n;j++)
         if(a[mi][mj]==a[i][j])
          count++;
         else
         { if(count-1==p)
             q=i;
             break;
             }
             }}
         //    printf("%d %d\n",q,p);
       change(a,b,q,p);
       tp[l][0]=mi;
       tp[l][1]=mj;
       tp[l][2]=q;
       tp[l][3]=p;
       if(a[mi][mj]=='B')
       tp[l++][4]=66;
       else
       tp[l++][4]=87;
     //  for(i=0;i<n;i++)
      //printf("%s\n",b[i]);
      //printf(":::::::\n");
       }
       if(l<min)
      { for(i=0;i<l;i++)
        {c[i][0]=tp[i][0];
        c[j][1]=tp[i][1];
        c[i][2]=tp[i][2];
        c[i][3]=tp[i][3];
        c[i][4]=tp[i][4];}
        min=l;
        }
      for(i=0;i<n;i++){
       for(j=0;j<n;j++)
       b[i][j]='B';
       b[i][n]='\0';
       }
       tp[0][0]=0;
       tp[0][1]=0;
       tp[0][2]=n-1;
       tp[0][3]=n-1;
       tp[0][4]=66;
       l=1;
      while(!check(a,b))
      {
      k=0;
      p=mj;q=mi;
      for(i=mi;i<n;i++){
       for(j=mj;j<n;j++)
       if(a[mi][mj]==a[i][j])
       {
       if(j==n-1)
        temp[k++]=n-1;
       continue;
       }
       else{
            if(j!=mj)
       temp[k++]=j-1;
       break;}
       }
       sort(temp,temp+k);
  //    for(i=0;i<k;i++)
    //  printf("%d ",temp[i]);
      // printf("\n");
       count=1;
       max=1;
       p=temp[0];
       for(i=1;i<k;i++)
       {
         if(temp[i]==temp[i-1])
         count++;
         else{
         if(max<=count)
         {p=temp[i-1];
         max=count;}
         //printf("::%d\n",p);
         count=1;
         }
         }
         if(max==1)
         p=temp[k-1];
         if(max<n/2)
         {
      
      for(i=mi;i<n;i++)
      {             
             if(a[mi][mj]==a[i][mj])
             continue;
             else
             {
                 q=i-1;
                 break;}
                 }}
                 else{      
                         
       // printf("::%d\n",p);
        for(i=mi;i<n;i++){
                          count=mj;
       for(j=mj;j<n;j++)
         if(a[mi][mj]==a[i][j])
          count++;
         else
         { if(count-1==p)
             q=i;
             break;
             }
             }}
         //    printf("%d %d\n",q,p);
       change(a,b,q,p);
       tp[l][0]=mi;
       tp[l][1]=mj;
       tp[l][2]=q;
       tp[l][3]=p;
       if(a[mi][mj]=='B')
       tp[l++][4]=66;
       else
       tp[l++][4]=87;
     //  for(i=0;i<n;i++)
      //printf("%s\n",b[i]);
      //printf(":::::::\n");
       }
        if(l<min)
      { for(i=0;i<l;i++)
        {c[i][0]=tp[i][0];
        c[j][1]=tp[i][1];
        c[i][2]=tp[i][2];
        c[i][3]=tp[i][3];
        c[i][4]=tp[i][4];}
        min=l;
        }*/
       count2=1;
       while(count2++<1000){
                    
       if(rand()%10<6){
       for(i=0;i<n;i++)
       for(j=0;j<n;j++)
       b[i][j]='W';
       l=0;}
       else{
             for(i=0;i<n;i++)
       for(j=0;j<n;j++)
       b[i][j]='B';
       tp[0][0]=0;
       tp[0][1]=0;
       tp[0][2]=n-1;
       tp[0][3]=n-1;
       tp[0][4]=66;
       l=1;
       }
      while(!check(a,b))
      {
       k=0;
      p=mj;q=mi;
      for(i=mi;i<n;i++){
       for(j=mj;j<n;j++)
       if(a[mi][mj]==a[i][j])
       {
       if(j==n-1)
        temp[k++]=n-1;
       continue;
       }
       else{
            if(j!=mj)
       temp[k++]=j-1;
       break;}
       }
       sort(temp,temp+k);
  //    for(i=0;i<k;i++)
    //  printf("%d ",temp[i]);
      // printf("\n");
       count=1;
       max=1;
       p=temp[0];
       for(i=1;i<k;i++)
       {
         if(temp[i]==temp[i-1])
         count++;
         else{
         if(max<=count)
         {p=temp[i-1];
         max=count;}
         //printf("::%d\n",p);
         count=1;
         }
         }
         if(max<n/2-1)
         p=temp[rand()%k];
         if(rand()%10<5)
         {
      for(i=mi;i<n;i++)
      {             
             if(a[mi][mj]==a[i][mj])
             continue;
             else
             {
                 q=i-1;
                 break;}
                 }}
                 else{      
                         
       // printf("::%d\n",p);
        for(i=mi;i<n;i++){
                          count=mj;
       for(j=mj;j<n;j++)
         if(a[mi][mj]==a[i][j])
          count++;
         else
         { if(count-1==p)
             q=i;
             break;
             }
             }}
         //    printf("%d %d\n",q,p);
       change(a,b,q,p);
       tp[l][0]=mi;
       tp[l][1]=mj;
       tp[l][2]=q;
       tp[l][3]=p;
       if(a[mi][mj]=='B')
       tp[l++][4]=66;
       else
       tp[l++][4]=87;
       }
      if(l<min)
       {  for(i=0;i<l;i++)
        {c[i][0]=tp[i][0];
        c[j][1]=tp[i][1];
        c[i][2]=tp[i][2];
        c[i][3]=tp[i][3];
        c[i][4]=tp[i][4];}
        min=l;
        }
       
       }
       printf("%d\n",min);
            for(i=0;i<min;i++)
       printf("%d %d %d %d %c\n",c[i][0],c[i][1],c[i][2],c[i][3],c[i][4]);
       scanf("%d",&i);
return 0;
//       getch();
       }
      
         
       
       
       
       
       
       
       
       
       
      


