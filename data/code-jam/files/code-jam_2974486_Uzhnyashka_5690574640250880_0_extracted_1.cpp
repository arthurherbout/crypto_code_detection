#include <iostream>
#include <cstdio>

using namespace std;


int t, h, w, m, x[3000], y[3000], xx, yy, q, r, k, test, i;
bool f[60][60], ok;
char c[60][60];


int main()

{
    freopen("input.txt","r", stdin);
    freopen("output.txt","w",stdout);
    cin>>test;
    for (int tt = 0; tt < test; tt++)
    {
        cin>>h>>w>>m;
        ok = true;
        for (int i = 1; i < 51; i++)
            for (int j = 1; j < 51; j++)
            f[i][j] = true;
        cout<<"Case #"<<tt+1<<":"<<endl;
        k = 0;
        q = h * w;
        t = min (h, w);
        if (t == 1){
            if (q-m<2 && m != 0) {cout<<"Impossible"<<endl; ok = false;}
        }
        else{
            if (q - m < 4) {cout<<"Impossilbe"<<endl; ok = false;}
        }
        if (ok){
            r=q-m;
            k = 1;
            x[k] = 1;
            y[k] = 1;
            f[1][1] = false;
            i = 1;
            while (true)
            {
                if (k == r) break;
                xx = x[i];
                yy = y[i];
                if (xx+1<=h && yy <= w && f[xx+1][yy]) {k++;
                                         x[k] = xx+1;
                                         y[k] = yy;
                                         f[x[k]][y[k]] = false;
                                         if (k == r) break;
                                         }
                if (xx<=h && yy+1 <= w && f[xx][yy+1]) {
                                        k++;
                                        x[k] = xx;
                                        y[k] = yy+1;
                                        f[x[k]][y[k]] = false;
                                        if (k == r) break;
                }
                if (xx+1<=h && yy+1<=w && f[xx+1][yy+1])
                {
                    k++;
                    x[k] = xx+1;
                    y[k] = yy+1;
                    f[x[k]][y[k]] = false;
                    if (k == r) break;
                }
                i++;
            }
            for (int i = 1; i <= h; i++)
                for (int j = 1; j <= w; j++)
                 c[i][j] = '*';
                 c[x[1]][y[1]] = 'c';
            for (int i = 2; i <= k; i++)
                c[x[i]][y[i]] = '.';
            for (int i = 1; i <= h; i++)
            {
                for (int j = 1; j <= w; j++)
                    cout<<c[i][j];
                printf("\n");
            }
        }
        }
    }


