#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <ctime>

using namespace std;

#define dump(x)  cerr << #x << " = " << (x) << endl;
#define PB push_back
#define MP make_pair
#define ll long long

inline int toInt(string s){int v;istringstream sin(s);sin>>v;return v;}
template<class T> inline string toString(T x){ostringstream sout;sout<<x;return sout.str();}

class attack{
public:
    int str;
    int day;
    int w;
    int e;
    
    attack(){};
    attack(int _str, int _day, int _w, int _e):
        str(_str), day(_day), w(_w), e(_e){};

    bool operator <(const attack arg) const{
        return day < arg.day;
    }
    
};

int main(){
    int cases;
    cin >> cases;
    for(int x=1;x<=cases;x++){
        vector<attack> attacks;
        int n, ret = 0;
        map<int,int> wall;
        cin >> n;
        for(int i=0;i<n;i++){
            int d,num,w,e,s,dd,dp,ds;
            cin >> d >> num >> w >> e >> s >> dd >> dp >> ds;
            for(int j=0;j<num;j++){
                attacks.push_back(attack(s+ds*j, d+dd*j, (w+dp*j)*2, (e+dp*j)*2));
            }
            sort(attacks.begin(), attacks.end());
        }

        for(int i=0;i<(int)attacks.size();i++){
            int end = i;
            for(int j=i;j<(int)attacks.size();j++){
                if(attacks[j].day == attacks[i].day){
                    end = j;
                }else{
                    break;
                }
            }

            map<int,int> attacked;

            for(int it = i; it <= end; it++){
                //cerr << "attack" << it << endl;
                //cerr << "  from " << attacks[it].w << " to " << attacks[it].e << endl;
                bool suc = false;
                for(int k = attacks[it].w * 2; k <= attacks[it].e * 2; k++){
                    if(attacks[it].str > wall[k]){
                        suc = true;
                        attacked[k] = max(attacked[k], attacks[it].str);
                    }
                }
                if(suc) ret++;
            }

            //update walls
            map<int,int>::iterator it = attacked.begin();

            while(it != attacked.end()){
                wall[(*it).first] = (*it).second;
                //cerr << "day "<< attacks[i].day << " wall[" << (*it).first << "] updated to " << (*it).second << endl;
                ++it;
            }
            i = end;
        }
        cout << "Case #" << x << ": " << ret << endl;
    }


}
