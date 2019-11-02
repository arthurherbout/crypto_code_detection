#include <algorithm>
#include <initializer_list>
#include <numeric>
#include <system_error>
#include <array>
#include <iomanip>
#include <ostream>
#include <thread>
#include <atomic>
#include <ios>
#include <queue>
#include <tuple>
#include <bitset>
#include <iosfwd>
#include <random>
#include <type_traits>
#include <chrono>
#include <iostream>
#include <ratio>
#include <typeindex>
#include <codecvt>
#include <istream>
#include <regex>
#include <typeinfo>
#include <complex>
#include <iterator>
#include <scoped_allocator>
#include <unordered_map>
#include <condition_variable>
#include <limits>
#include <set>
#include <unordered_set>
#include <deque>
#include <list>
#include <sstream>
#include <utility>
#include <exception>
#include <locale>
#include <stack>
#include <valarray>
#include <forward_list>
#include <map>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <memory>
#include <streambuf>
#include <functional>
#include <mutex>
#include <string>
#include <future>
#include <new>
#include <strstream>
using namespace std;


namespace Debug {

    template<typename F, typename S>
    ostream &operator<<(ostream &os, const pair<F, S> &p) {
        return os << "{" << p.first << ", " << p.second << "}";
    }

    template<typename T>
    ostream &operator<<(ostream &os, const vector<T> &v) {
        os << "[";
        typename vector<T>::const_iterator it;
        for (it = v.begin(); it != v.end(); it++) {
            if (it != v.begin()) os << ", ";
            os << *it;
        }
        return os << "]";
    }

    template<typename T>
    ostream &operator<<(ostream &os, const set<T> &v) {
        os << "[";
        typename set<T>::const_iterator it;
        for (it = v.begin(); it != v.end(); it++) {
            if (it != v.begin()) os << ", ";
            os << *it;
        }
        return os << "]";
    }

    template<typename F, typename S>
    ostream &operator<<(ostream &os, const map<F, S> &v) {
        os << "[";
        typename map<F, S>::const_iterator it;
        for (it = v.begin(); it != v.end(); it++) {
            if (it != v.begin()) os << ", ";
            os << it->first << " = " << it->second;
        }
        return os << "]";
    }

#define deb(x) cerr << #x << " = " << x << endl;
}

using namespace Debug;

typedef long long i64;
typedef unsigned long long ui64;
typedef pair<int,int> pii;

#define mx 0
#define IN "/Users/bidhanroy/ClionProjects/C/in"
#define OUT "/Users/bidhanroy/ClionProjects/C/out"

vector<unsigned > tokenize( string a, string b ) {
    hash<string> tt;
    const char *q = a.c_str();
    while( count( b.begin(), b.end(), *q ) ) q++;
    vector<unsigned > oot;
    while( *q ) {
        const char *e = q;
        while( *e && !count( b.begin(), b.end(), *e ) ) e++;
        oot.push_back( tt(string( q, e )) );
        q = e;
        while( count( b.begin(), b.end(), *q ) ) q++;
    }

    return oot;
}

void solve(int kas) {
    cout << "Case #" << kas << ": ";

    int n;
    cin >> n;

    string name;

    unordered_map<string,int> ase;
    //int id = 0;

    getline(cin, name);

    vector< vector<unsigned > > L;

    for(int i = 0; i < n; i++ ) {
        getline(cin, name);

        L.push_back( tokenize(name," ") );

        //deb( L[i].size() );
    }

    // E = 0, F = 1

    int ans = 1000000000;

    unordered_set<unsigned > F1, E1, both;

    for(auto x: L[0]) E1.insert(x);
    for(auto x: L[1]) F1.insert(x);

    for(auto x: E1) if(F1.count(x)) both.insert(x);


    for(int mask = 0; mask < (1<<n); mask++ ) {
        if( mask & 1 ) continue;
        if( !(mask & 2) ) continue;

        //cout << mask << endl;

        int cnt = 0;

        unordered_set<unsigned > E, F;

        for(int i = 2; i < n; i++ ) {
            if( mask & (1<<i) ) {
                // F
                for(auto &x : L[i] ) {
                    if( F1.count(x) || F.count(x) ) continue;
                    F.insert( x );
                    cnt += ( E.count( x ) || E1.count(x) );
                }
            }
            else {
                for( auto &x : L[i] ) {
                    if( E1.count(x) || E.count(x) ) continue;
                    cnt += ( F1.count( x ) || F.count(x) );
                    E.insert( x );
                    //cnt += F.count( x );
                }
            }
        }



        //for(auto &x : F ) cnt += ( E.count( x ) || E1.count(x) );

        ans = min( ans , cnt + (int)both.size() );
    }

    cout << ans << endl;

}

int main() {
    freopen( IN, "r", stdin );
    freopen( OUT, "w", stdout );
    using namespace chrono;
    time_t start=system_clock::to_time_t(system_clock::now());
    ios_base::sync_with_stdio(0);
    int test, kas=0;
    cin>>test;
    while( test-- ) {
        solve(++kas);
    }
    time_t end = system_clock::to_time_t(system_clock::now());
    cerr << " Program has run "<< end-start << " s " << endl;
    return 0;
}