#include <iostream>
#include <string>
#include <vector>
std::string sortRPS(std::string a){
    if(a.size() == 1) return a;
    std::string x = sortRPS(a.substr(0,a.size()/2));
    std::string y = sortRPS(a.substr(a.size()/2));
    if(x>y)return y+x;
    else return x+y;
}
std::string rec(int N, int R, int P, int S, std::string last){
    //std::cerr<<N<<' '<<R<<' '<<P<<' '<<S<<' '<<last<<'\n';
    if(N==0)return sortRPS(last);
    std::string next;
    next.reserve(last.size()<<1);
    for(int i = 0; i < last.size(); ++i){
        if(last[i]=='R')next+="RS",--S;
        else if(last[i]=='P')next+="PR",--R;
        else if(last[i]=='S')next+="PS",--P;
    }
    if(R<0||P<0||S<0)return "";
    return rec(N-1,R,P,S,next);
}
int main(){
    int Ti,Tn;
    std::cin>>Tn;
    for(Ti=1;Ti<=Tn;++Ti){
        std::cout<<"Case #"<<Ti<<": ";
        int N,R,P,S;
        std::cin>>N>>R>>P>>S;
        std::string res = "IMPOSSIBLE";
        std::string a = rec(N,R-1,P,S,"R");
        std::string b = rec(N,R,P-1,S,"P");
        std::string c = rec(N,R,P,S-1,"S");
        res='X';
        if(!a.empty()&&a<res)res = a;
        if(!b.empty()&&b<res)res = b;
        if(!c.empty()&&c<res)res = c;
        if(res=="X")res = "IMPOSSIBLE";
        std::cout<<res<<'\n';
    }
}