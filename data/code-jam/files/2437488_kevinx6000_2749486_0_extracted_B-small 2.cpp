#include<cstdio>
#include<vector>
#include<queue>
#include<map>
#define SIZ 10000
using namespace std;

// Mapping BFS
int id;
map<int,int>dis;
map<int,int>p;

// Queue
typedef struct{
  int x,y,step;
}NODE;
queue<NODE>que;

// Directions
int dir[4][2]={{-1,0},{0,-1},{1,0},{0,1}};

// Seq
vector<int>seq;

// Main
int main(void)
{
  int tc,cs,x,y,i,ans,way;
  bool reach;
  NODE now,next;

  // Read Input
  scanf("%d",&tc);
  for(cs=1;cs<=tc;cs++)
  {
    // Initialize
    id=1;
    scanf("%d%d",&x,&y);
    now.step=1;
    now.x=now.y=SIZ;
    que.push(now);
    dis[now.x*SIZ+now.y]=0;
    reach=false;
    while(!que.empty())
    {
      now=que.front();
      que.pop();
      for(i=0;i<4;i++)
      {
        next.x=now.x+dir[i][0]*now.step;
        next.y=now.y+dir[i][1]*now.step;
        if(!dis[next.x*SIZ+next.y])
        {
          dis[next.x*SIZ+next.y]=dis[now.x*SIZ+now.y]+1;
          next.step=now.step+1;
          que.push(next);
          p[next.x*SIZ+next.y]=i;
          if(next.x-SIZ==x&&next.y-SIZ==y)
          {
            reach=true;
            break;
          }
        }
      }
      if(reach) break;
    }
    ans=dis[(x+SIZ)*SIZ+(y+SIZ)];
    now.x=x+SIZ;
    now.y=y+SIZ;
    for(i=ans;i>0;i--)
    {
      way=p[now.x*SIZ+now.y];
      seq.push_back(way);
      now.x+=dir[(way+2)%4][0]*i;
      now.y+=dir[(way+2)%4][1]*i;
    }
    printf("Case #%d: ",cs);
    for(i=ans-1;i>=0;i--)
    {
      switch(seq[i])
      {
        case 0: putchar('W');break;
        case 1: putchar('S');break;
        case 2: putchar('E');break;
        case 3: putchar('N');break;
      }
    }
    putchar('\n');

    // Clear
    dis.clear();
    p.clear();
    seq.clear();
    while(!que.empty()) que.pop();
  }
  return 0;
}
