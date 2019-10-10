#include<iostream>
#include<string>

using namespace std;
int check_string(string s,int k)
{
    int ret = 0;
    int len = s.length();
    for(int i = 0;i<len;i++)
    {
            if(s[i] == '-')
            {
               if((len - i) < k)
                       return -1;
               else
               {
                   ret++;
                   for(int j = i;j<(i+k);j++)
                   {
                           if(s[j] == '-')
                                   s[j] = '+';
                           else
                                   s[j] = '-';
                   }
               }
            }
    }
    return ret;
}     
    
int main()
{
    int test;
    freopen ("A-large.in","r",stdin);
    freopen ("A-large.out","w",stdout);
    cin>>test;
    
    for(int t = 0;t<test;t++)
    {
            string s;
            cin>>s;
            int k;
            cin>>k;
            cout<<"Case #"<<t+1<<": ";
            int ret = check_string(s,k);
            if(ret >= 0)
            {
                   cout<<ret<<endl;
            }
            else
            {
                cout<<"IMPOSSIBLE"<<endl;
            }
    }
    return 0;
    
}
    
