#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int n = 16, j = 50;
vector <string> comb, data;
long long myPow(long long x, int p){
    if(p == 0) return 1;
    long long tmp = myPow(x, p/2);
    tmp *= tmp;
    if(p%2)
        tmp *= x;
    return tmp;
}
long long convert(string s, long long base){
    long long ans = 0;
    int x = 0;
    for(int i = s.size()-1; i >= 0; i--){
        if(s[i] == '1')
            ans += myPow(base, x);
        x++;
    }
    return ans;
}
bool isPrime(long long x){
    if(x == 2)return 1;
    long long lim = sqrt(x)+1;
    for(int i = 2; i <= lim; i++){
        if(x % i == 0)
            return 0;
    }
    return 1;
}
void gen(string s){
    if(s.size() == n){
        if(s[0] == '1' && s[n-1] == '1'){
//            bool f = 1;
//            for(int i = 2; i <= 10; i++){
//                long long tmp = convert(s, i);
//                if(isPrime(tmp)){
//                    f = 0;
//                    break;
//                }
//            }
//            if(f)
                comb.push_back(s);
        }
        return;
    }
    gen(s + '0');
    gen(s + '1');
}
void filter(){
    for(int i = 0; i < comb.size(); i++){
        string cur = comb[i];
        bool f = 1;
        for(int i = 2; i <= 10; i++){
            long long tmp = convert(cur, i);
            if(isPrime(tmp)){
                f = 0;
                break;
            }
        }
        if(f)
            data.push_back(cur);
    }
}
int main()
{
    freopen("out.txt", "w", stdout);
    gen("");
    filter();
    cout << "Case #1:\n";
    for(int i = 0; i < j; i++){
        cout << data[i] << " ";
        for(int k = 2; k <= 10; k++){
            long long tmp = convert(data[i], k);
            int lim = sqrt(tmp)+1;
            for(int y = 2; y <= lim; y++){
                if(tmp%y == 0){
                    cout << y << " \n"[k == 10];
                    break;
                }
            }
        }
    }

    return 0;
}
