#include<iostream>
#include<string>
#include<vector>
using namespace std;
bool isFriends[1005][1005];
bool isSet[1005];
int n;

int len2(int head,int currnet,bool ok,int lens){
    int maxL=0;
    if(ok){
        maxL=lens;
    }
    else if(isFriends[currnet][head]){
        maxL=lens;
    }

    for(int i=0;i<n;i++){
        if(isSet[i]){
            continue;
        }

        if(ok){
            isSet[i]=true;
                int tmp=len2(head,i,isFriends[i][currnet],lens+1);
                isSet[i]=false;
                if(tmp>maxL){
                    maxL=tmp;
                }
                /*
            if(isFriends[i][currnet]){
                isSet[i]=true;
                int tmp=len2(head,i,true,lens+1);
                isSet[i]=false;
                if(tmp>maxL){
                    maxL=tmp;
                }
            }*/
        }

        if(isFriends[currnet][i]){
                isSet[i]=true;
                int tmp=len2(head,i,isFriends[i][currnet],lens+1);
                isSet[i]=false;
                if(tmp>maxL){
                    maxL=tmp;
                }
            }
        }

        return maxL;
    }
int main(){
    int t,cas=0;
    cin>>t;
    while(t--){
        cin>>n;
        for(int i=0;i<1005;i++){
            isSet[i]=false;
            for(int j=0;j<1005;j++)
                isFriends[i][j]=false;
        }

        for(int i=0;i<n;i++){
            int tmp;
            cin>>tmp;
            tmp--;
            isFriends[i][tmp]=true;
        }
        int ans=0;
        for(int i=0;i<n;i++){
            isSet[i]=true;
            int headTest=len2(i,i,false,1);
            isSet[i]=false;
            if(headTest>ans){
                //cout<<"ac: "<<i<<endl;
                ans=headTest;
            }
        }
        cout<<"Case #"<<++cas<<": "<<ans<<endl;
    }
}

