#include<iostream>
#include<string>

using namespace std;
void check_string(string s)
{
     int len = s.length();
     string output;
     int i = 0;
     while(s[i] == '1')
     i++;
     if(s[i] == '0')
     {
             for(int j = 0;j<len - 1;j++)
                     cout<<"9";
             cout<<endl;
             return;
     }
     else
     {
         output.resize(len);
         for(int j = i;j<len;j++)
         {
                 if(s[j] < s[j-1])
                 {
                         for(int k=j;k<len;k++)
                                  output[k] = '9';
                         int k = j-1;
                         while(s[k] == '1')
                         {
                                    output[k] = '9';
                                    k--;
                         }
                         output[k] = s[k] - 1;
                         k--;
                         while(k>=0)
                         {
                                    output[k] = s[k];
                                    k--;
                         }
                         check_string(output);
                         return; 
                 }
         }
         cout<<s<<endl;  
     }
}
     
int main()
{
    int test;
    freopen ("B-large.in","r",stdin);
    freopen ("B-large.out","w",stdout);
    cin>>test;
    
    for(int t = 0;t<test;t++)
    {
            string s;
            cin>>s;
            cout<<"Case #"<<t+1<<": ";
            check_string(s);
    }
    //cin>>test;
    return 0;
}
