#include<algorithm>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<iostream>
#include<map>
#include<queue>
#include<set>
#include<sstream>
#include<stack>
#include<vector>

using namespace std;

#define forn(i,n) for(int i = 0; i < (n); i++)
#define dforn(i,n) for(int i = (int)(n-1); i >= 0; i--)
#define all(v) v.begin(), v.end()
#define pb push_back

int main()
{
	freopen("C-small.in","r",stdin);
	freopen("C-small.out","w",stderr);
	int casos;
	cin >> casos;
	forn(casito,casos)
	{
	    int n;
        cin >> n;
        vector<int> vecx(n);
        vector<int> vecy(n);
        forn(i,n)
            cin >> vecx[i] >> vecy[i];
        int res = 1;
        forn(i,n)
        forn(j,n)
            if((vecy[i] == vecy[j] || vecy[i] == vecy[j]+1)&&i!=j)
                res = 2;
        if(res==2)
        {
            forn(i,n)
            forn(j,n)
            {
                if(i!=j&&vecy[i]==vecy[j])
                {
                    int k1=0,k2=0;
                    bool p = true;
                    forn(k,n)
                    {
                        if(vecy[k]==vecy[i]&&(vecx[i]-vecx[k])*(vecx[j]-vecx[k])<0)
                            p = false;
                        if(vecy[k]==vecy[i]+1&&(vecx[i]-vecx[k])*(vecx[j]-vecx[k])<0)
                            k1++;
                        if(vecy[k]==vecy[i]-1&&(vecx[i]-vecx[k])*(vecx[j]-vecx[k])<0)
                            k2++;
                    }
                    if((k1==1||k2==1)&&p==true)
                        res = 3;
                }
            }
        }
	    cerr << "Case #" << casito+1 << ": " << res << endl;
	}
}
