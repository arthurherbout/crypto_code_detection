#include<cstdio>
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<algorithm>

using namespace std;


int hexmagic(char ch)
{
  switch(ch)
  {
    case '1' :return 1;
    case '2' :return 2;
    case '3' :return 3;
    case '4' :return 4;
    case '5' :return 5;
    case '6' :return 6;
    case '7' :return 7;
    case '8' :return 8;
    case '9' :return 9;
    case 'A' :return 10;
    case 'B' :return 11;
    case 'C' :return 12;
    case 'D' :return 13;
    case 'E' :return 14;
    case 'F' :return 15;
    case '0' :return 0;
  }
}
int main()
{
  FILE * infile;
  FILE * outfile;
  int test_case;
  int t;
  int valid,size,col;
  
  infile=fopen("in.txt","r");
  outfile=fopen("out.txt","w");
  
  
  if(!infile || !outfile)
  {
    printf("Unable to read/write file\n");
    exit(0);
  }
  fscanf(infile,"%d",&test_case);
  
  
  t=0;
  int m,n;
  int i,j,k;
  int p,q,r,s;
  int num;
  int board[512][512];
  int score[512];
  char ch;
  while(t<test_case)
  {
    t++;
  
    fscanf(infile,"%d %d",&m,&n);
    
    for(i=0;i<m;i++)
      for(j=0;j<n/4;j++)
      {
	//fscanf(infile,"%x",&num);
	ch='\n';
	while(ch=='\n') fscanf(infile,"%c",&ch);
	
	num=hexmagic(ch);
	for(k=3;k>=0;k--)
	{
	    board[i][(4*j)+k]=num%2;
	    num/=2;
	}
      }
      
      for(i=0;i<512;i++)
	score[i]=0;
    //read variables;

    for(i=0;i<m;i++)
      for(j=0;j<n;j++)
      {
	if(board[i][j]!=-1)
	{
	  //find a cut piece
	  size=0; // one cell
	  col=board[i][j];
	  board[i][j]=-1;
	  //size 2 or more
	  for(p=1;p<m && p<n ;p++)
	  {
	    //search by col
	    valid=1;
	    for(q=0;q<=p;q++)
	    {
	      if(board[i+q][j+p]==(1-col))
	      {
		col=1-col;
	      }
	      else
	      {
		valid=0;
		break;
	      }
	    }
	    
	    for(q=p-1;q>=0;q--)
	    {
	      if(board[i+p][j+q]==(1-col))
	      {
		col=1-col;
	      }
	      else
	      {
		valid=0;
		break;
	      }
	    }
	    
	    if(valid)
	    {
	      //set all -1;
		for(q=0;q<=p;q++)
	       {
	         board[i+q][j+p]=-1;      
	       }
	       for(q=0;q<=p;q++)
	       {
	         board[i+p][j+q]=-1;
	       }
	      
	      size+=1;
	    }
	    
	    else break;
	  }
	  
	    {
	      // store result and continue;
	      score[size]+=1;
	      if(size) printf("::::%d %d::::%d\n",i,j,size);
	    }
	  
	}
      }
    
    //apply logic
    
    int total;
    total=0;
    for(i=0;i<512;i++)
    {
      if(score[i]) total+=1;
    }
    
    
    //write output
    fprintf(outfile,"Case #%d: %d\n",t,total);
    for(i=511;i>=0;i--)
    {
      if(score[i]) fprintf(outfile,"%d %d\n",i+1,score[i]);
    }
  }
  
  fcloseall();
}