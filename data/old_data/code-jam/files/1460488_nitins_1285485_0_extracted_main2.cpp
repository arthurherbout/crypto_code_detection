/* 
 * File:   main.cpp
 * Author: nitin
 *
 * Created on April 14, 2012, 6:12 AM
 */
#include <iostream>
#include<cstring>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<math.h>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    ifstream myfile("example.txt");
    int row,col,dist,tempdist,test,val=0,xpos,ypos,count,flag;
    float xwork,ywork,floatdist;
    myfile>>test;
    while(val<test)
    {
        val++;
        count=0;
        myfile>>row>>col>>dist;
        char area[row][col];
        for(int i=0;i<row;i++)
            for(int j=0;j<col;j++)
                myfile>>area[i][j];
        for(int i=0;i<row;i++)
            for(int j=0;j<col;j++)
            {
                if(area[i][j]=='X')
                {
                    xpos=i,ypos=j;
                    break;
                }
            }
        //now find the mirrors along x and y for reflection
        for(int j=0;j<col;j++)
        {
            tempdist=(abs(ypos-j)-1)*2+1;
            if((area[xpos][j]=='#')&&(tempdist<=dist))
                count++;
        }
        for(int i=0;i<col;i++)
        {
            tempdist=(abs(xpos-i)-1)*2+1;
            if(area[i][ypos]=='#'&&(tempdist<=dist))
                count++;
        }
        //now find the remaining mirrors for reflection
        for(int i=0;i<row;i++)
            for(int j=0;j<col;j++)
            {
                flag=0;
                if(area[i][j]=='#')
                {
                    //searching for corner
                    if((i==0&&j==0)||(i==row&&j==col)||(i==row&&j==0)||(i==0&&j==col))
                        flag=1;
                    else if((i<xpos&&j<ypos)&&(area[i][j+1]=='#'&&area[i+1][j]=='#')&&((xpos-i)==(ypos-j)))
                        flag=1;
                    else if((i>xpos&&j<ypos)&&(area[i][j+1]=='#'&&area[i-1][j]=='#')&&((xpos-i)==(ypos-j)))
                        flag=1;
                    else if((i<xpos&&j>ypos)&&(area[i][j-1]=='#'&&area[i+1][j]=='#')&&((xpos-i)==(ypos-j)))
                        flag=1;
                    else if((i>xpos&&j>ypos)&&(area[i][j-1]=='#'&&area[i-1][j]=='#')&&((xpos-i)==(ypos-j)))
                        flag=1;
                }
                if(flag==1)
                {
                    xwork=abs(xpos-i);
                    ywork=abs(ypos-i);
                    xwork=pow(xwork,2);
                    ywork=pow(ywork,2);
                    floatdist=sqrt(xwork+ywork);
                    if(floatdist<=dist)
                        count++;
                }
            }
        cout<<"Case #"<<val<<": "<<count<<endl;
    }
    return 0;
}

