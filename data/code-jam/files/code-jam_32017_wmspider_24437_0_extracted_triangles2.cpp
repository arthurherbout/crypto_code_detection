#include <iostream>

using namespace std;

int main(){
    int ncases; 
    cin >> ncases;
    for(int asdf=1;asdf<=ncases;asdf++){
        long long n, A, B, C, D, X0, Y0, M;
        cin >> n >> A >> B >> C >> D >> X0 >> Y0 >> M;
        int x[200], y[200];
        for(long long i=0;i<n;i++){
            x[i]=X0;
            y[i]=Y0;
            X0 = (A * X0 + B) % M;
            Y0 = (C * Y0 + D) % M;
        }
        long long ans=0;
        for(int i=0;i<n-2;i++){
            for(int j=i+1;j<n-1;j++){
                for(int k=j+1;k<n;k++){
                    if((x[i] + x[j] + x[k])%3==0 && (y[i] + y[j] + y[k])%3==0) ans++;
                }
            }
        }
        cout << dec << "Case #" << asdf << ": " << ans << endl;
    }
}
