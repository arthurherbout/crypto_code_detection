
#include <string.h>
#include <unordered_map>
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include <unordered_set>

using namespace std;

int r1,r2;
string s1,s2;
void form(int idx, string& a, string& b){
    if(idx == a.size()){

        int aa = stoi(a);
        int bb = stoi(b);

        if(r1 == -1 || abs(aa-bb) < abs(r1-r2)){
            r1 = aa;
            r2 = bb;
            s1 = a;
            s2 = b;
            return;
        }
        if(abs(aa-bb) > abs(r1-r2)) return;
        if(aa < r1){
            r1 = aa;
            r2 = bb;
                        s1 = a;
            s2 = b;
            return;
        }
        if(aa > r1) return;
        if(bb < r2){
         r1 = aa;
            r2 = bb;
                        s1 = a;
            s2 = b;
            return;
        }
        return;
    }
    if(a[idx] == '?'){
        for(char c = '0'; c <= '9'; c++){
            a[idx] = c;
            if(b[idx] == '?'){
                for(char cc = '0'; cc <= '9'; cc++){
                    b[idx] = cc;
                    form(idx+1,a,b);
                }
                b[idx] = '?';
            }
            else
                form(idx+1,a,b);
        }
        a[idx] = '?';
    }
    else if(b[idx] == '?'){
        for(char cc = '0'; cc <= '9'; cc++){
                    b[idx] = cc;
                    form(idx+1,a,b);
                }
                b[idx] = '?';
    }
    else form(idx+1,a,b);
}
int main(){
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int tst;
    cin >> tst;
    for(int tt=1; tt <= tst; tt++){
        string a,b;
        cin >> a >> b;
        r1 = r2 = -1;
        form(0,a,b);
        printf("Case #%d: %s %s\n",tt,s1.c_str(),s2.c_str());

    }
    return 0;
}