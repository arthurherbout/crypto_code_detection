#include<iostream>
#include<string>
#include<sstream>
using namespace std;
int heights[3000];
int main(){
    int t,cas=0;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        int input=2*n-1;
        for(int i=0;i<3000;i++)heights[i]=0;

        for(int i=0;i<input;i++){
            int tmp;
            for(int j=0;j<n;j++){
                cin>>tmp;
                heights[tmp]++;
            }
        }


        cout<<"Case #"<<++cas<<":";
        for(int i=0;i<3000;i++){
            //cout<<i<<" : "<<heights[i]<<endl;
            if((heights[i]%2)!=0){
                cout<<" "<<i;
            }
        }
        cout<<endl;
    }
}

