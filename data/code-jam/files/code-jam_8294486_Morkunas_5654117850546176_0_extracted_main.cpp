#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
string file_name = "B-small-attempt0";

int gcd(int a, int b){
    while(a&&b){
        if(a>b) a%=b;
        else b%=a;
    }
    return a+b;
}

ifstream input(file_name + ".in");
ofstream out(file_name +".out");
typedef pair<int,char> pic;


//#define input cin
//#define out cout
const int MAXN = 1000+5;

string returnRYB(int R, int Y, int B){
   //cout << R << Y << B << endl;
    vector<pic > myVec;
    myVec.push_back(pic(R,'R'));
    myVec.push_back(pic(Y,'Y'));
    myVec.push_back(pic(B,'B'));
    sort(myVec.begin(), myVec.end(),greater<pic>());
    if(myVec[0].first > myVec[1].first + myVec[2].first){
        return "";
    }
    int mSize = R+Y+B;
    int index = 0;
    char result[MAXN];
    int position = 0;
    while(position < mSize){
        if(myVec[index].first > 0){
            result[position]=myVec[index].second;
            position+=2;
            myVec[index].first--;
        }else{
            index++;
        }
    }
    position = 1;
    while(position < mSize){
        if(myVec[index].first > 0){
            result[position]=myVec[index].second;
            position+=2;
            myVec[index].first--;
        }else{
            index++;
        }
    }
    string ans;
    for(int i = 0; i < mSize; i++){
        ans.push_back(result[i]);
    }
    return ans;

}
void solution(){

    int R, O, Y, G, B, V, N;
    input >> N >> R >> O >> Y >> G >> B >> V;
    string ans = returnRYB(R,Y,B);
    if(ans == "") out <<"IMPOSSIBLE" << endl;
    else out << ans << endl;

}

int main()
{
    int T;
    input >> T;
    for(int c = 0; c < T; c++){
        out << "Case #" << c+1 << ": ";
        solution();
    }
    return 0;
}
