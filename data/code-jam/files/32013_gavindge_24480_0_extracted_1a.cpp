#include  <set>
#include  <iostream>
#include  <cstdio>
#include  <string>

using namespace std;

int main(int argc, char *argv[])
{
    int N;
    cin>>N;
    for( int i=1 ; i<=N ; ++i )
    {
        int S;
        cin>>S;
        string name;
        char cname[101];
        set<string> id;
        gets(cname);
        for( int j=0 ; j<S ; ++j )
        {
            gets(cname);
            name=cname;
            id.insert(name);
        }
        int Q;
        cin>>Q;
        int a=0;
        set<string> app;
        string query;
        int change=0;
        gets(cname);
        for( int j=0 ; j<Q ; ++j )
        {
            gets(cname);
            query=cname;
            if(app.find(query) ==app.end())
            {
                app.insert(query);
                ++a;
            }
            if(a==S)
            {
                ++change;
                a=1;
                app.clear();
                app.insert(query);
            }
        }
        cout<<"Case #"<<i<<": "<<change<<endl;       
    }
    return 1;
}
