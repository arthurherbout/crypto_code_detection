#include  <iostream>
#include  <algorithm>
#include  <vector>
#include  <cstdlib>
using namespace std;

class trip
{
    public:
        int start;
        int end;
        trip(int s,int e):start(s),end(e){};
};

bool sLess(const trip &a,const trip &b) 
{
    return a.start<b.start;
}
bool eLess(const trip &a,const trip &b)
{
    return a.end<b.end;
}

int main(int argc, char *argv[])
{
    int N;
    cin>>N;
    for( int i=1 ; i<=N ; ++i )
    {
        int T;
        cin>>T;
        int NA,NB;
        cin>>NA>>NB;
        vector<trip> tNA,tNB;
        string sTime,eTime;
        int sm,em;
        for( int j=0 ; j<NA ; ++j )
        {
            cin>>sTime>>eTime;            
            sm=atoi(sTime.substr(0,2).c_str())*60+atoi(sTime.substr(3,2).c_str());
            em=atoi(eTime.substr(0,2).c_str())*60+atoi(eTime.substr(3,2).c_str());
            tNA.push_back(trip(sm,em));
        }
        for( int j=0 ; j<NB ; ++j )
        {
            cin>>sTime>>eTime;            
            sm=atoi(sTime.substr(0,2).c_str())*60+atoi(sTime.substr(3,2).c_str());
            em=atoi(eTime.substr(0,2).c_str())*60+atoi(eTime.substr(3,2).c_str());
            tNB.push_back(trip(sm,em));
        }
        int tripA,tripB;
        tripA=NA;
        tripB=NB;
        sort(tNA.begin(),tNA.end(),eLess);
        sort(tNB.begin(),tNB.end(),sLess);
        int find=0;
        for( int j=0 ; j<NA ; ++j )
        {
            for(; find<NB ; ++find )
            {
                if(tNB[find].start-tNA[j].end>=T)
                {
                    ++find;
                    --tripB;
                    break;
                }
            }
        }
        find=0;
        sort(tNA.begin(),tNA.end(),sLess);
        sort(tNB.begin(),tNB.end(),eLess);
        for( int j=0 ; j<NB ; ++j )
        {
            for(; find<NA ; ++find )
            {
                if(tNA[find].start-tNB[j].end>=T)
                {
                    ++find;
                    --tripA;
                    break;
                }
            }
        }
        cout<<"Case #"<<i<<": "<<tripA<<" "<<tripB<<endl;
        
    }
    
}
