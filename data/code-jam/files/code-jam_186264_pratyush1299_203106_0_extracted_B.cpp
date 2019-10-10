#include<iostream>
#include<algorithm>
#include<string>
using namespace std;
int main()
{
    string str,str1,str2;
    int n,no=0;
    cin>>n;
    while(n--)
    {
              no++;
              cin>>str;
                   str1=str;
                   next_permutation(str.begin(),str.end());
                   if(str1<str)
                   cout<<"Case #"<<no<<": "<<str<<endl;
                   else
                   {
                       str1.insert(1,"0");
                       sort(str1.begin(),str1.end());
                       while(next_permutation(str1.begin(),str1.end()))
                       {
                                                                    if(str1[0]!='0')
                                                                    break;
                                                                    }
                       cout<<"Case #"<<no<<": "<<str1<<endl;
                   }
                   }
    return 0;
}
