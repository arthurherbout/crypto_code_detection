// Problem B

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <cmath>

using namespace std;

const int MAX = 1000;

vector<int> primes;
bool p[MAX + 1];
vector<set<int> > prec(MAX + 1);
vector<int> pert(MAX + 1);

void sieve(){
   for (int i = 0; i <= MAX; i++) {
        p[i] = true;
   }
   p[0] = false;
   p[1] = false;
   int m = (int)sqrt((double)(MAX + 1));
   for (int i = 2; i <= m; i++) {
      if (p[i])
         for (int k = i * i; k <= MAX; k += i)
            p[k] = false;
   }
   for (int i = 0; i <= MAX; i++) {
       if (p[i]) primes.push_back(i);
   }
}

void doprec () {
    for (int i = 1; i <= MAX; i++) {
        int n = 2;
        int num = i;
        while (n * n <= num) {
            if (num % n == 0) {
                num /= n;
                prec[i].insert(n);
            } else {
                n++;
            }
        }
        if (num != 1) prec[i].insert(num);
    }
}

bool checa (const int& A, const int& B, const int& P) {
    for (set<int>::const_iterator cit = prec[A].begin(); cit != prec[A].end(); cit++) {
        if (*cit >= P) {
            if (prec[B].find(*cit) != prec[B].end()) return true;
        }
    }
    return false;
}

int main () {

    ifstream in("B.in");
    ofstream out("B.out");
    int cases;
    in >> cases;
    sieve();
    doprec();
    int A, B, P;
    for (int c = 0; c < cases; c++) {
        in >> A >> B >> P;
        for (int i = A; i <= B; i++) {
            pert[i] = i;
        }
        for (int i = A; i <= B; i++) {
            for (int j = i + 1; j <= B; j++) {
                if (checa(i , j, P)) {
                    //cout << "merge" << i << " " << j << endl;
                    int aux = pert[j];
                    pert[j] = pert[i];
                    for (int k = A; k <= B; k++) {
                        if (pert[k] == aux) {
                            pert[k] = pert[i];
                        }
                    }
                }
            }
        }
        set<int> ans;
        for (int i = A; i <= B; i++) {
            //cout << i << " " << pert[i] << endl;
            ans.insert(pert[i]);
        }
        out << "Case #" << (c + 1) << ": " << ans.size() << endl;
        //cout << ans.size() << endl;
    }
    //system("pause");
    in.close();
    out.close();

}