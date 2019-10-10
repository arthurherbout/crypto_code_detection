#include <bits/stdc++.h>
using namespace std;

#define MOD 1000000007
#define llu long long unsigned
#define lld long long
#define ld long
int main() {
	// your code goes here
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	lld test,n,i,j,k,a,b,c,p;
	string str;
	cin>>test;
	for(k=1;k<=test;k++){
        cout<<"Case #"<<k<<": ";
        cin>>n>>p;
        lld arr[n+1];
        for(i=0;i<n;i++){
            cin>>arr[i];
        }
        multiset<lld> s[n+1];
        multiset<lld>::iterator it;
        for(i=0;i<n;i++){
            for(j=0;j<p;j++){
                cin>>a;
                s[i].insert(a);
            }
        }

        lld answer = 0;
        for(i=1;i<100000;i++){
            //cout<<i<<endl;
            lld low[n+1],high[n+1];
            for(j=0;j<n;j++){
                low[j]=(lld)ceil((arr[j]*i)*0.9);
                high[j]=(lld)floor((arr[j]*i)*1.1);
               // cout<<low[j]<<" "<<high[j]<<endl;
            }
            lld count[n+1]={};
            lld min=1000000000000;
            for(j=0;j<n;j++){
                for(it=s[j].begin();it!=s[j].end();it++){
                    b=*it;
                    if(b>=low[j]&&b<=high[j]){
                        count[j]++;
                    }

                }
               // cout<<count[j]<<endl;
            }
            for(j=0;j<n;j++){
                if(count[j]<min){
                    min=count[j];
                }
            }

            if(min!=0){
        //            cout<<min<<endl;
                    answer+=min;
                for(j=0;j<n;j++){
                    for(it=s[j].begin();it!=s[j].end();it++){
                        b=*it;
                        lld c=0;
                        if(b>=low[j]&&b<=high[j]&&c<min){
                            c++;
                      //      s[j].erase(it);
                        }
                    }
                }
            }
            if(answer>=p){answer=p;break;}
        }
        cout<<answer<<endl;
	}
	return 0;
}
