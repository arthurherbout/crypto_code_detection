#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

const int dr[]={-1,0,0,1};
const int dc[]={0,-1,1,0};

bool isrev(int i,int j) { return (dr[i]+dr[j]==0)&&(dc[i]+dc[j]==0); }

vector<vector<int> > board;
int rows,cols;
int mer,mec;

void showboard(const vector<vector<int> >& board)
{ for(int r=0;r<rows;++r)
  { for(int c=0;c<cols;++c) cout<<board[r][c]<<' ';
    cout<<endl;
  }
  cout<<endl;
}
int attwho(int r,int c,const vector<vector<int> >& board) // not global board
{ int ad=-1,sc=-1,i;
  for(i=0;i<4;++i)
  { int nr=r+dr[i],nc=c+dc[i];
    if(nr<0||nr>=rows||nc<0||nc>=cols) continue;
    if(board[nr][nc]>sc)
    { sc=board[nr][nc];
      ad=i;
    }
  }
  return ad;
}
// board not global board
vector<vector<int> > simul(int medir,const vector<vector<int> >& board)
{
  int r,c,i;
  vector<vector<int> > rv=board;
  for(r=0;r<rows;++r) for(c=0;c<cols;++c) if(board[r][c])
  { int ad=-1,sc=-1;
    if(r==mer&&c==mec)
    { if(medir==-1) continue;
      else ad=medir;
    }else ad=attwho(r,c,board);
    rv[r+dr[ad]][c+dc[ad]]=max(0,rv[r+dr[ad]][c+dc[ad]]-board[r][c]);
  }
  return rv;
}

bool forever(const vector<vector<int> >& board) // not global board
{ int i;
  for(i=0;i<4;++i)
  { int nr=mer+dr[i],nc=mec+dc[i];
    if(nr<0||nr>=rows||nc<0||nc>=cols) continue;
    if(board[nr][nc]) return false;
  }
  return true;
}

bool underthreat(const vector<vector<int> >& board)
{
  for(int i=0;i<4;++i)
  { int nr=mer+dr[i],nc=mec+dc[i];
    if(nr<0||nr>=rows||nc<0||nc>=cols) continue;
    if(isrev(i,attwho(nr,nc,board))) return true;
  }
  return false;
}

int calc()
{
  int rv=0,i,j;
  while(board[mer][mec])
  { ++rv;
    if(forever(board)) return -1;
    vector<vector<int> > sit=simul(-1,board);
//    showboard(sit);
    if(!sit[mer][mec]) break; // nothing to avoid this
    while(!underthreat(sit))
    { if(forever(sit)) return -1;
      else sit=simul(-1,sit);
    }
    int sc=-1,ad=-1;
    for(i=0;i<4;++i)
    { int nr=mer+dr[i],nc=mec+dc[i];
      if(nr<0||nr>=rows||nc<0||nc>=cols) continue;
      j=attwho(nr,nc,sit);
      if(!isrev(i,j)) continue;
      if(sit[nr][nc]>sc)
      { sc=sit[nr][nc];
        ad=i;
      }
      else if(sit[nr][nc]==sc)
      { vector<vector<int> > t1,t2;
        t1=simul(ad,board); t2=simul(i,board);
        while(t1[mer][mec]==t2[mer][mec] && t1[mer][mec] && !forever(t1) && !forever(t2))
        { t1=simul(-1,t1); t2=simul(-1,t2);
        }
        if(t1[mer][mec]<t2[mer][mec]) ad=i;
        else if(t1[mer][mec]==t2[mer][mec] && forever(t2)&&!forever(t1))
          ad=i;
      }
    }
    board=simul(ad,board);
//    cout<<"after my shot "<<endl;
//    showboard(board);
  }
  return rv;
}

int main()
{
  int ci,cn;
  cin>>cn;
  for(ci=1;ci<=cn;++ci)
  {
    int i,j;
    cin>>cols>>rows>>mec>>mer;
    --mer; --mec;
    board=vector<vector<int> >(rows,vector<int>(cols,0));
    for(i=0;i<rows;++i) for(j=0;j<cols;++j) cin>>board[i][j];

    cout<<"Case #"<<ci<<": ";
    int res=calc();
    if(res<0) cout<<"forever"<<endl;
    else cout<<res<<" day(s)"<<endl;
  }
}
