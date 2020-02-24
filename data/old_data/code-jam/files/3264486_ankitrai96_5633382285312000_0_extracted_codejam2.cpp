#include<iostream>
using namespace std;
int tidy(int n){
    int tidiest;
    if(n<10){
        return n;
    }
    for(int c = 1 ; c <= n ; c++){
        int flag=0, check=0, num=c, doc[1000];
        while(num!=0){
            doc[flag] = num % 10;
            flag++;
            num/=10;
        } 
        for(int i = flag-1 ; i>0 ; i--){
            if(doc[i]>doc[i-1]){
                check = 1; //not tidy
                break;
            }
        }
        if(check==0){
            tidiest = c;
        }
    }
    return tidiest;
}
int main(){
    int T, N[1000]; // number of test cases & last number counted respectively
    int c; //counter
    cin>>T;
    for(c = 0 ; c < T; c++){
        cin>>N[c];
    }
    for(c = 0 ; c < T; c++){
        cout<<"Case #"<<c+1<<": "<<tidy(N[c])<<endl;
    }
}