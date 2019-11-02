#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string.h>
#include <stack>
#include <list>
#include <math.h>

using namespace std;


int main(int argc, const char * argv[])
{
    int test_case;
    cin >> test_case;
    
    for(int z = 1; z <= test_case; z++) {
        
        int n, k;
        double u;
        cin >> n >> k;
        cin >> u;
        
        priority_queue<double, vector<double>, greater<double> > p;
        
        for(int i = 0; i < n; i++) {
            double t;
            cin >> t;
            p.push(t);
        }
        
        while(u > 0.00000001) {
            double f = p.top(); p.pop();
            int count = 1;
            if(f >= 1) {
                p.push(f);
                break;
            }
            while(!p.empty()&&f == p.top()) {
                count++;
                p.pop();
            }
            if(p.empty()) {
                for(int i = 0; i < count; i++) p.push(f + (u / (double)count));
                break;
            }
            else {
                double d = (u / (double)count);
                double c = p.top();
                if((c - f) * count <= u) for(int i = 0; i < count; i++) { p.push(c); u -= (c - f); }
                else for(int i = 0; i < count; i++) { p.push(f + d); u = 0; }
            }
        }
        
        double result = 1;
        
        while(!p.empty()) {
            double f = p.top();
            result *= f;
            p.pop();
        }
        
        printf("Case #%d: ", z);
        printf("%.6lf", result);
        printf("\n");
    }
    
    return 0;
}
