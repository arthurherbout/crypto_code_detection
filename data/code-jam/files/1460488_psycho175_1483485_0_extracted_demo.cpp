#include<iostream>
#include<string.h>
#include<fstream>
using namespace std;
int main()
{
      int t;
      char arr[150];
      char map[26]={'y','h','e','s','o','c','v','x','d','u','i','g','l','b','k','r','z','t','n','w','j','p','f','m','a','q'};
      cin>>t;
      cin.getline(arr,150);
      for(int i=0;i<t;i++)
      {
            cin.getline(arr,150);
            cout<<"Case #"<<i+1<<": ";
            for(int j=0;j<strlen(arr);j++)
            {
                    if(arr[j]==' ')
                      cout<<arr[j];
                    else

                    cout<<map[arr[j]-'a'];
            }
            cout<<"\n";
              
      }
}
