#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

struct train
{
    int pre,next;
    int d;
    train()
    {
        d=0,pre=-1,next=-1;
    }
};

int main()
{
    int t;
    ifstream fin;
    ofstream fout;
    fin.open("B-small-attempt1.in");
    fout.open("output.txt");
    fin>>t;
    for(int k=1;k<=t;++k)
    {
        int n;
        fin>>n;
        string s[100];
        for(int i=0;i<n;++i)fin>>s[i];
        bool f=false;
        train z[26];
        /*for(int i=0;i<26;++i)
            cout<<z[i].pre<<" "<<z[i].d<<" "<<z[i].next<<endl;*/
        bool mid[26],side[26];
        fill(mid,mid+26,false);
        fill(side,side+26,false);
        //cout<<"FEEASFESA"<<endl;
        for(int i=0;i<n;++i)
        {
            bool used[26];
            fill(used,used+26,false);
            char pre=' ';
            for(int j=0;j<s[i].size();++j)
            {
                if(s[i][j]!=pre)
                {
                    if(used[s[i][j]-'a'])f=true;
                    pre=s[i][j],used[s[i][j]-'a']=true;
                }
                if(f)break;
            }
            if(s[i][0]==s[i][s[i].size()-1])z[s[i][0]-'a'].d++;
            else
            {
                if(z[s[i][0]-'a'].next==-1)z[s[i][0]-'a'].next=s[i][s[i].size()-1]-'a';
                else
                {
                    f=true;
                    //fout<<s[i][0]<<endl;
                    //fout<<"GSEGSE"<<endl;
                }
                if(z[s[i][s[i].size()-1]-'a'].pre==-1)z[s[i][s[i].size()-1]-'a'].pre=s[i][0]-'a';
                else
                {
                    f=true;
                    //fout<<s[i][s[i].size()-1]<<endl;
                    //fout<<"ESGSE"<<endl;
                }
            }
            used[s[i][0]-'a']=used[s[i][s[i].size()-1]-'a']=false;
            for(int j=0;j<26;++j)
                mid[j]|=used[j];
            side[s[i][0]-'a']=side[s[i][s[i].size()-1]-'a']=true;
            if(f)break;
        }
        //cout<<"FEEASFESA"<<endl;
        for(int i=0;i<26;++i)
            if(side[i]&&mid[i])
            {
                f=true;
                //fout<<"SGSEGSE"<<endl;
            }
        //cout<<"FEEASFESA"<<endl;
        if(f)
        {
            fout<<"Case #"<<k<<": 0"<<endl;
            continue;
        }
        //cout<<"FEEASFESA"<<endl;
        long long int ans=1;
        int r=0;
        while(1)
        {
            /*for(int i=0;i<26;++i)
                cout<<z[i].pre<<" "<<z[i].d<<" "<<z[i].next<<endl;*/
            int now;
            bool y=false;
            for(int i=0;i<26;++i)
                if(z[i].pre==-1&&(z[i].next!=-1||z[i].d!=0))
                {
                    //cout<<i<<endl;
                    ++r;
                    y=true;
                    while(z[i].d!=0)ans=ans*z[i].d%1000000007,z[i].d--;
                    if(z[i].next==-1)break;
                    now=z[i].next,z[i].next=-1;
                    while(z[now].d!=0||z[now].next!=-1)
                    {
                        while(z[now].d!=0)ans=ans*z[now].d%1000000007,z[now].d--;
                        if(z[now].next==-1)break;
                        int o=now;
                        now=z[now].next,z[o].next=-1;
                    }
                    break;
                }
            if(!y)break;
        }
        while(r!=0)ans=ans*r%1000000007,r--;
        //cout<<endl;
        fout<<"Case #"<<k<<": "<<ans<<endl;
    }
    return 0;
}
