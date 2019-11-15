#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int dec_war(vector<double> &na, vector<double> &ke)
{
    int ret=0;
    for (int i=0, j=0; i<na.size(); i++)
        if (na[i]>ke[j]) ret++, j++;
    return ret;
}

int war(vector<double> &na, vector<double> &ke)
{
    int ret=0;
    for (int i=na.size()-1, j=ke.size()-1; i>=0; i--)
        if (na[i]>ke[j]) ret++; else  j--;
    return ret;
}

int main()
{
    int tcs;
    cin>>tcs;
    for (int tc=1; tc<=tcs; tc++)
    {
        int n;
        cin>>n;
        vector<double> na, ke;
        for (int i=0; i<n; i++)
        {
            double num;
            cin>>num;
            na.push_back(num);
        }
        for (int i=0; i<n; i++)
        {
            double num;
            cin>>num;
            ke.push_back(num);
        }
        sort(na.begin(), na.end());
        sort(ke.begin(), ke.end());
        cout<<"Case #"<<tc<<": "<<dec_war(na, ke)<<" "<<war(na, ke)<<endl;
    }
}