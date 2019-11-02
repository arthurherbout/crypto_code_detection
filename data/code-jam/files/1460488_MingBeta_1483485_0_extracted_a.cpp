#include<iostream>

using namespace std;

char a[30];

int main()
{
a[0]='y';
a[1]='h';
a[2]='e';
a[3]='s';
a[4]='o';
a[5]='c';
a[6]='v';
a[7]='x';
a[8]='d';
a[9]='u';
a[10]='i';
a[11]='g';
a[12]='l';
a[13]='b';
a[14]='k';
a[15]='r';
a[16]='z';
a[17]='t';
a[18]='n';
a[19]='w';
a[20]='j';
a[21]='p';
a[22]='f';
a[23]='m';
a[24]='a';
a[25]='q';



    int n,t;
    char s[108];
    freopen("A-small-attempt1.in","r",stdin);
    freopen("A-small-attempt1.out","w",stdout);
    cin>>n;
    gets(s);
    for (int i=1;i<=n;i++){
        gets(s);
        cout<<"Case #"<<i<<": ";
        for (int j=0;j<strlen(s);j++)
            if (s[j]>='a' && s[j]<='z') cout<<a[s[j]-'a'];
               else cout<<s[j];
        cout<<endl;
        }
}
