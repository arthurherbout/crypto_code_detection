#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<fstream>
#include<queue>
#include<cmath>
#include<map>
using namespace std;
struct node
{
       int p,q,tm;
       string sd;
       node(){};
       node(int a,int b,int c,string d){p=a;q=b;tm=c;sd=d;}
};
pair<int,int> mk(int a,int b)
{
              return make_pair(a,b);
}
int main()
{
    int i,t,x,y,g,h,tim;
    pair<int,int> p1,p2,p3,p4;
    ifstream fin;
    ofstream fout;
    fin.open("inp.txt");
    fout.open("out.txt");
    string s,str;
    fin>>t;
    for(int k=1;k<=t;k++){
                          map<pair<int,int>,bool > vis; 
                          queue<node> q;
                          node l;
              fin>>x>>y;
              q.push(node(0,0,0,""));
              vis[mk(0,0)]=1;
              while(!q.empty()){
                                l=q.front();q.pop();
                                g=l.p;h=l.q;tim=l.tm;s=l.sd;
                                //fout<<g<<" "<<h<<" "<<tim<<" "<<s<<endl;
                                if(g==x && h==y){str=s;break;}
                                p1=mk(g+tim+1,h);p2=mk(g-tim-1,h);p3=mk(g,h+tim+1);p4=mk(g,h-tim-1);
                               if(vis[p1]==0){vis[p1]=1;q.push(node(g+tim+1,h,tim+1,s+'E'));} 
                               if(vis[p2]==0){vis[p2]=1;q.push(node(g-tim-1,h,tim+1,s+'W'));} 
                               if(vis[p3]==0){vis[p3]=1;q.push(node(g,h+tim+1,tim+1,s+'N'));} 
                               if(vis[p4]==0){vis[p4]=1;q.push(node(g,h-tim-1,tim+1,s+'S'));}
              }                
              fout<<"Case #"<<k<<": "<<str<<endl;
    }
    return 0;
}
