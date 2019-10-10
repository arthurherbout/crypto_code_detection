#include <iostream>
#include <string>
#include <vector>
double rec(std::vector<double>& pr, int i, int j, std::vector<double>& chosen){
    if(j==chosen.size()){
        std::vector<double> res[2];
        res[0].resize(chosen.size()+1);
        res[1].resize(chosen.size()+1);
        int t = 0;
        res[0][0] = 1;
        int n = 1;
        for(auto it : chosen){
            for(auto& jt : res[!t])jt=0;
            for(int k = 0;k<n;++k){
                res[!t][k] += res[t][k] * (1-it);
                res[!t][k+1] += res[t][k] * it;
            }
            ++n;
            t^=1;
        }
        return res[t][chosen.size()/2];
    }
    if(i==pr.size())return 0;
    double best=0;
    for(;i<pr.size();++i){
        chosen[j]=pr[i];
        double v = rec(pr,i+1,j+1,chosen);
        if(v>best)best=v;
    }
    return best;
}
int main(){
    int Ti,Tn;
    std::cin>>Tn;
    for(Ti=1;Ti<=Tn;++Ti){
        std::cout<<"Case #"<<Ti<<": ";
        int n,k;
        std::cin>>n>>k;
        std::vector<double> pr;
        pr.resize(n);
        std::vector<double> chosen;
        chosen.resize(k);
        for(int i = 0;i<n;++i)std::cin>>pr[i];
        double res = rec(pr,0,0,chosen);
        std::cout.precision(15);
        std::cout<<res<<'\n';
    }
}