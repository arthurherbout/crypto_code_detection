#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
#define large 100003
long long int A[large];

int main() {
    long long int r,R,T,C,W,i,j,k,count,D,V,val,less;
    cin>>T;
    for(r=1;r<=T;r++){
        cin>>C>>D>>V;
        A[0]=1;
        for(j=0;j<large;j++) A[j]=0;
        for(j=0;j<D;j++){
            cin>>R;
            if(R<large) A[R]=1;
        }


        k=0;
        less=0;
        for(i=1;i<large;i++){
            if(A[i]==0){
                val=C*(i*(i-1))/2-less;
                if(val>=V) break;
                if(i>val) k++;
                else less+=C*i;
            }
        }
        cout<<"Case #"<<r<<": "<<k<<endl;
    }
    return 0;
}
