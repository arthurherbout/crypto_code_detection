#include <iostream>
#include <string>
#include <vector>
bool chk(std::string res){
    if(res.size()==1)return true;
    std::string v;
    for(int i=0;i<res.size();i+=2){
        if(res[i]==res[i+1])return false;
        if(res[i]<res[i+1])std::swap(res[i],res[i+1]);
        if(res[i]=='S'&&res[i+1]=='R')v+='R';
        else if(res[i]=='S'&&res[i+1]=='P')v+='S';
        else if(res[i]=='R'&&res[i+1]=='P')v+='P';
        else throw std::runtime_error(std::string("Unhandled case ") + res[i]+res[i+1]);
    }
    return chk(v);
}

bool rec(int i, std::string&v, int r,int p,int s, std::string& res){
    if(i==v.size()){
        if(chk(v)){
            res = v;
            return true;
        }
    }
    if(p>0){
        v[i]='P';
        if(rec(i+1,v,r,p-1,s,res))return true;
    }
    if(r>0){
        v[i]='R';
        if(rec(i+1,v,r-1,p,s,res))return true;
    }
    if(s>0){
        v[i]='S';
        if(rec(i+1,v,r,p,s-1,res))return true;
    }
    return false;
}

int main(){
    int Ti,Tn;
    std::cin>>Tn;
    for(Ti=1;Ti<=Tn;++Ti){
        std::cout<<"Case #"<<Ti<<": ";
        int N,R,P,S;
        std::cin>>N>>R>>P>>S;
        std::string res = "IMPOSSIBLE";
        std::string v = "";
        v.resize(1<<N);
        rec(0,v,R,P,S,res);
        std::cout<<res<<'\n';
    }
}