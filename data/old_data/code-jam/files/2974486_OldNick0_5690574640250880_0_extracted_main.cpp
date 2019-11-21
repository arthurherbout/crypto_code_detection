#include<iostream>
#include<iomanip>
#include<vector>
#include<cstdio>
using namespace std;
int r,c,m;
int map[60][60];
vector< pair<int,int> >pos;
int find_mine(int x,int y){
    int dx[8]={1,1,1,0,0,-1,-1,-1};
    int dy[8]={1,0,-1,1,-1,1,0,-1};
    int num=0;
    for(int i=0;i<8;i++){
        int tx=x+dx[i];
        int ty=y+dy[i];
        if(tx<0||tx>=r)continue;
        if(ty<0||ty>=c)continue;
        if(map[tx][ty]==-1)num++;
    }
    return num;
}
int set_not_mine(int x,int y){
    int dx[8]={1 ,1,1,0,0,-1,-1,-1};
    int dy[8]={1,0,-1,1,-1,1,0,-1};
    int num=0;
    for(int i=0;i<8;i++){
        int tx=x+dx[i];
        int ty=y+dy[i];
        if(tx<0||tx>=r)continue;
        if(ty<0||ty>=c)continue;
        if(map[tx][ty]==-1){
            pos.push_back(make_pair(tx,ty));
            map[tx][ty]==10;
            num++;
        }
    }
    return num;
}
int main(){
    int t,cas=0;
    cin>>t;
    while(t--){
        cin>>r>>c>>m;
        cout<<"Case #"<<++cas<<":"<<endl;
        if(r*c-1==m){
            for(int i=0;i<r;i++){
                for(int j=0;j<c;j++){
                    if(i==0&&j==0)cout<<"c";
                    else cout<<"*";
                }
                cout<<endl;
            }
        }
        else if(r==1||c==1){
            if(r*c-m<2)cout<<"Impossible"<<endl;
            else if(c==1){
                int now=0;
                for(int i=1;i<r;i++){
                    if(now<m){
                        cout<<"*"<<endl;
                        now++;
                    }
                    else cout<<"."<<endl;
                }
                cout<<"c"<<endl;
            }
            else if(r==1){
                int now=0;
                for(int i=1;i<c;i++){
                    if(now<m){
                        cout<<"*";
                        now++;
                    }
                    else cout<<".";
                }
                cout<<"c"<<endl;
            }
        }else {
            if(r*c-m<4)cout<<"Impossible"<<endl;
            else{
                for(int i=0;i<r;i++){
                    for(int j=0;j<c;j++)map[i][j]=-1;
                }
                int now=4;
                map[0][0]=0;
                map[0][1]=map[1][0]=2;
                map[1][1]=5;
                pos.clear();
                pos.push_back(make_pair(0,1));
                pos.push_back(make_pair(1,0));
                pos.push_back(make_pair(1,1));
                int taget=r*c-m;
                bool falg=true;
                while(now<taget){
                    int  round=-1;
                    for(int i=0;i<pos.size();i++){
                        if(map[pos[i].first][pos[i].second]+now<=taget){
                            if(round==-1||map[pos[i].first][pos[i].second]>map[pos[round].first][pos[round].second]){
                                round=i;
                            }
                        }
                    }
                    if(round==-1){
                        falg=false;
                        break;
                    }
                    //cout<<"ch"<<pos[round].first<<" "<<pos[round].second<<" "<<now<<endl;
                    now+=set_not_mine(pos[round].first,pos[round].second);
                    //cout<<"af"<<now<<endl;
                    map[pos[round].first][pos[round].second]=0;
                    pos.erase(pos.begin()+round);
                    for(int i=0;i<pos.size();i++){
                        map[pos[i].first][pos[i].second]=find_mine(pos[i].first,pos[i].second);
                    }
                }
                if(falg){
                    /*
                    for(int i=0;i<r;i++){
                        for(int j=0;j<c;j++){
                            cout<<map[i][j];
                        }
                        cout<<endl;
                    }*/
                    for(int i=0;i<r;i++){
                        for(int j=0;j<c;j++){
                            if(i==0&&j==0)cout<<"c";
                            else if(map[i][j]>=0)cout<<".";
                            else cout<<"*";
                        }
                        cout<<endl;
                    }
                }
                else {
                   /* for(int i=0;i<r;i++){
                        for(int j=0;j<c;j++){
                            cout<<map[i][j];
                        }
                        cout<<endl;
                    }*/
                    cout<<"Impossible"<<endl;
                }
            }
        }
    }
}
