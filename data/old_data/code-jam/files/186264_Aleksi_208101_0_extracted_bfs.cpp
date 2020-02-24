#include<iostream>
#include<tr1/unordered_set>
#include<cassert>
#include<algorithm>
#include<vector>
#include<string>
int W;
char square[22][22];
struct bfs_node
{
  int x,y,sum;
  int prev;
  char op;
  int num;
  
};
struct bfs_hasher
{
  size_t operator()(const bfs_node& n)const 
  {
    return n.x<<5+n.y<<10+n.sum<<15;
  };
};
struct bfs_eq
{
  bool operator()(const bfs_node& n1,const bfs_node& n2)const
  {
    return n1.x==n2.x && n1.y==n2.y && n1.sum == n2.sum;
  }
};
std::tr1::unordered_set<bfs_node,bfs_hasher,bfs_eq> used;
void unset_used()
{
  used.clear();
}
int get_num(int x,int y)
{
  return square[x][y]-'0';
}
bool n_cmp(const bfs_node& n1,const bfs_node& n2)
{
  return n1.sum<n2.sum;
}

bool& get_used(int x,int y,int sum)
{
  //assert(abs(sum)<2500);
  //return used[x][y][sum+2500];
}

bool get_used(bfs_node n)
{
  return used.count(n);
}
void set_used(bfs_node n)
{
  used.insert(n);
}
void print_square();
void bfs(int target)
{
  unset_used();
  std::vector<bfs_node> nodes;
  for(int i=0;i<W;i++)
  {
    for(int j=0;j<W;j++)
    {
      if(isdigit(square[i][j]))
      {
        bfs_node n;
        n.x = i;
        n.y = j;
        n.sum = get_num(i,j);
        n.num =  get_num(i,j);
        n.prev = -1;
        set_used(n);
        nodes.push_back(n);
      }
    }
  }
  std::sort(nodes.begin(),nodes.end(),n_cmp);
  for(int i=0;i<nodes.size();i++)
  {
    const bfs_node n = nodes[i];
    int x = n.x;
    int y = n.y;
    assert(n.num>=0);
    if(square[4][4]<2)
    {
      print_square();
      assert(false);
    }
    if(n.sum == target)
    {
      std::vector<int> sol;
      int cur = i;
      while(cur!=-1)
      {
        sol.push_back(cur);
        cur = nodes[cur].prev;
      }
      std::reverse(sol.begin(),sol.end());
      for(int i=0;i<sol.size()-1;i++)
      {
        std::cout<<nodes[sol[i]].num;
        std::cout<<nodes[sol[i+1]].op;
      }
      std::cout<<nodes[sol.back()].num;
      std::cout<<"\n";
      return;
    }
    int dx[] = {0,1,0,-1};
    int dy[] = {-1,0,1,0};
    for(int minus=0;minus<2;minus++)
    {
      for(int j=0;j<4;j++)
      {
        int opx = x+dx[j];
        int opy = y+dy[j];
        if(opx<0 || opx>=W||opy<0||opy>=W)
          continue;
        if(square[opx][opy]=='+' xor minus)
        {
          std::vector<bfs_node> add_nodes;
          for(int k=0;k<4;k++)
          {
            int npx = opx+dx[k];
            int npy = opy+dy[k];
            if(npx<0 || npx>=W||npy<0||npy>=W)
              continue;
            int add = minus?-get_num(npx,npy):get_num(npx,npy);
            bfs_node n2;
            n2.x = npx;
            n2.y = npy;
            n2.sum = n.sum+add;
            n2.prev = i;
            n2.op = minus?'-':'+';
            n2.num = get_num(npx,npy);
            add_nodes.push_back(n2);
          }
          std::sort(add_nodes.begin(),add_nodes.end(),n_cmp);
          if(minus)
            std::reverse(add_nodes.begin(),add_nodes.end());

          for(int k=0;k<add_nodes.size();k++)
          {
            if(get_used(add_nodes[k])==0)
            {
              set_used(add_nodes[k]);
              nodes.push_back(add_nodes[k]);
            }
          }

        }
      }
    }
  }
  std::cout<<"WTF:\n";
  std::cout<<nodes.size()<<"\n";
}
void print_square()
{
  std::cout<<"-\n";
  for(int i=0;i<W;i++)
  {
    for(int j=0;j<W;j++)
    {
      std::cout<<square[j][i]<<"";
    }
    std::cout<<"\n";
  }
}

void handle_case(int case_no)
{
  int q;
  std::cin>>W>>q;
  std::string line;
  std::getline(std::cin,line);

  for(int i=0;i<W;i++)
  {
    std::getline(std::cin,line);
    for(int j=0;j<W;j++)
    {
      square[j][i] = line[j];
  //    std::cout<<square[j][i]<<"";
    }
  //  std::cout<<"\n";
  }
  std::cout<<"Case #"<<case_no<<":\n";
  for(int i=0;i<q;i++)
  {
    int t;
    std::cin>>t;
    bfs(t);
  }
}

int main()
{
  int T;
  std::cin>>T;
  for(int i=0;i<T;i++)
  {
    handle_case(i+1);
  }
}

