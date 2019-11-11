#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;


int t, n, j, t1, t2, ans1, ans2, a1, a2, b1, b2;
double a[100000],b[10000];

int main()

{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w", stdout);
    cin>>t;
    for (int tt = 0; tt < t; tt++)
    {
        cin>>n;
        for (int i = 0; i < n; i++)
            scanf("%lf", &a[i]);
        for (int i = 0; i < n; i++)
            scanf("%lf", &b[i]);
        sort(a, a+n);
        sort(b, b+n);
        cout<<"Case #"<<tt+1<<": ";
        ans1 = 0;
        ans2 = 0;
        a1 = 0;
        a2 = n-1;
        b1 = 0;
        b2 = n-1;
        while (a2>=a1 && b2>=b1)
        {
            if (a[a1]<b[b2] && a[a1]>b[b1]) {b1++;
                                             ans1++;
                                             a1++;
                                            }
            else if (a[a1]<b[b2] && a[a1]<b[b1]) {b2--;
                                                  a1++;
                                                 }
            else if (a[a1]>b[b2]) {a1++; ans1++; b1++;}
        }
        t2 = n-1;
        t1 = 0;
        for (int i = n-1; i>=0; i--)
            if (a[i]>b[t2]) {ans2++; t1++;}
            else {t2--;}
        cout<<ans1<<" "<<ans2<<endl;
    }
}

