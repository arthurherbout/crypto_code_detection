#include <iostream>

using namespace std;


long long fact(long long n){
    long long ans=1;
    for(long long i=n;i>0;i--) ans*=i;
    return ans;
}

long long comb(long long a,long long n){
    long long ans=1;
    for(long long i=0;i<n;i++){
        ans*=a;
        a--;
    }
    ans/=fact(n);
    return ans;
}



int main(){
    long long ncases;
    long long n, A, B, C, D, X0, Y0, M;
    long long ntrees[3][3];
    //cout << comb(5,3) << endl;
    cin >> ncases;
    for(long long asf=1;asf<=ncases;asf++){
        cin >> n >> A >> B >> C >> D >> X0 >> Y0 >> M;
        for(long long i=0;i<3;i++) for(long long j=0;j<3;j++) ntrees[i][j]=0; 
        /*
        X = x0, Y = y0
        print X, Y
        for i = 1 to n-1
            X = (A * X + B) mod M
            Y = (C * Y + D) mod M
            print X, Y
        */
        for(long long i=0;i<n;i++){
            ntrees[X0%3][Y0%3]++;
            X0 = (A * X0 + B) % M;
            Y0 = (C * Y0 + D) % M;
        }
        long long ans=0;
        for(long long i=0;i<3;i++){
            ans+=comb(ntrees[i][0],3);
            ans+=comb(ntrees[i][1],3);
            ans+=comb(ntrees[i][2],3);
            ans+=ntrees[i][0]*ntrees[i][1]*ntrees[i][2];
        }
        for(long long i=0;i<3;i++){
            ans+=ntrees[0][i]*ntrees[1][i]*ntrees[2][i];
        }
        ans+=ntrees[0][0] * ntrees[1][1] * ntrees[2][2];
        ans+=ntrees[0][0] * ntrees[1][2] * ntrees[2][1];
        ans+=ntrees[0][1] * ntrees[1][0] * ntrees[2][2];
        ans+=ntrees[0][1] * ntrees[1][2] * ntrees[2][0];
        ans+=ntrees[0][2] * ntrees[1][1] * ntrees[2][0];
        ans+=ntrees[0][2] * ntrees[1][0] * ntrees[2][1];
        
        cout << "Case #" << asf << ": " << ans << endl;
    }
}
        
