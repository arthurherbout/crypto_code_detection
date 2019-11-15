#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<int, int> Flaver;
typedef vector<vector<Flaver> > Satisfys;

bool solve(const Satisfys&satisfy, int current, vector<int>&flaver, vector<int>&ret)
{
    int retCnt=0, nowCnt=0;
    for(int i=0; i<ret.size(); i++)
    {
        if(ret[i]==1)
            retCnt++;

        if(flaver[i]==1)
            nowCnt++;
    }
    if(nowCnt!=ret.size() && nowCnt>=retCnt)
        return false;

    if(satisfy.size()==current)
    {
        copy(flaver.begin(), flaver.end(), ret.begin());
        return true;
    }

    /*
    // check
    for(int i=current; i<satisfy.size(); i++)
    {
        int check=false;
        for(int j=0; j<satisfy[i].size(); j++)
        {
            int type=satisfy[i][j].first,
                malt=satisfy[i][j].second;
            if(flaver[type] == -1 || flaver[type] == malt)
            {
                check=true;
                break;
            }
        }
        if(!check)
            return false;
    }
*/
    bool retVal=false;
    for(int i=0; i<satisfy[current].size(); i++)
    {
        int type=satisfy[current][i].first,
            malt=satisfy[current][i].second;

        if(flaver[type] == -1 || flaver[type] == malt)
        {
            int tmp=flaver[type];
            flaver[type]=malt;
            if(solve(satisfy, current+1, flaver, ret))
                retVal=true;
            flaver[type]=tmp;
        }
    }

    return retVal;
}

bool comp(const vector<Flaver>&a, const vector<Flaver>&b)
{
    return a.size() < b.size();
}

int main()
{
    int n;
    cin >> n;

    for(int i=0; i<n; i++)
    {
        int flaverNum;
        int customerNum;
        cin >> flaverNum >> customerNum;
        cerr << flaverNum << " " << customerNum << endl;

        Satisfys satisfys(customerNum);

        for(int j=0; j<customerNum; j++)
        {
            int t;
            cin >> t;
            satisfys[j].resize(t);
            for(int k=0; k<t; k++)
            {
                int type, malted;
                cin >> type >> malted;
                satisfys[j][k]=make_pair(type-1, malted);
            }
        }

        sort(satisfys.begin(), satisfys.end(), comp);

        vector<int> flaver(flaverNum, -1);
        vector<int> ret(flaverNum, 1);

        //pre
        /*
        for(int j=0; j<satisfys.size(); j++)
        {
            if(satisfys[j].size()!=1)
                continue;

            int type=satisfys[j][0].first,
                malt=satisfys[j][0].second;
            if(flaver[type]!=-1 && flaver[type]!=malt)
            {
                fill(ret.begin(), ret.end(), 0);
                break;
            }
            flaver[type]=malt;
        }
        */
        cout << "Case #" << i+1 << ": ";
        if(solve(satisfys, 0, flaver, ret))
        {
            int j;
            for(j=0; j<flaverNum-1; j++)
            {
                if(ret[j]==-1)
                    cout << 0 << " ";
                else
                    cout << ret[j] << " ";
            }
            if(ret[j]==-1)
                cout << 0 << endl;
            else
                cout << ret[j]<< endl;
        }
        else
            cout << "IMPOSSIBLE" << endl;
    }
    return 0;
}
