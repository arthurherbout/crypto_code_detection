
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

unordered_map<string,int> fst,scnd;

int main(){
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int tst;
    cin >> tst;
    for(int tt = 1; tt <= tst; tt++){
        fst.clear();
        scnd.clear();
        int n;
        cin >> n;
        vector<pair<string,string> > v;
        for (int i = 0; i < n; ++i)
        {
            string a,b;
            cin >> a >> b;
            v.emplace_back(a,b);
            fst[a]++;
            scnd[b]++;
        }
        int res=0;
        for(auto p : v){
            if(fst[p.first] > 1 && scnd[p.second] > 1) res++;
        }
        printf("Case #%d: %d\n",tt,res);
    }
    return 0;
}