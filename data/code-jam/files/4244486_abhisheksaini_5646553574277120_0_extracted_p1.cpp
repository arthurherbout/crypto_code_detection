#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    long long int r,R,T,C,W,i,j,k,count,D,V,A[35],val,less;
    cin>>T;
    for(r=1;r<=T;r++){
        cin>>C>>D>>V;
        A[0]=1;
        for(j=0;j<=V;j++) A[j]=0;
        for(j=0;j<D;j++){
            cin>>R;
            A[R]=1;
        }

        k=0;
        less=0;
        for(i=1;i<=V;i++){
            if(A[i]==0){
                val=(i*(i-1))/2-less;
                if(i>val) k++;
                else less+=i;
            }
        }
        cout<<"Case #"<<r<<": "<<k<<endl;
    }
    return 0;
}
