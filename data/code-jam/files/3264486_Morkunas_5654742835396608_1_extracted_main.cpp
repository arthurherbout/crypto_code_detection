#include <iostream>
#include <queue>
#include <fstream>
using namespace std;
typedef pair<int,int>pii;
ifstream input ("C-small-2-attempt0.in");
ofstream out ("C-small-2-attempt0.out");
struct compare
{
  bool operator()(const pii& A, const pii& B) const
  {
    if((A.second - A.first) == (B.second - B.first)){
        if( A.first < B.first){
            return true;
        }
        else{
            return false;
        }
    }
    else
    {
        if ((A.second - A.first) > (B.second - B.first)){
            return false;
        }
        else return true;
    }
  }
};

pii solve(int N, int K)
{
    priority_queue<pii,vector<pii>,compare> que;
    pii current, A,B;
    que.push(pii(0,N+1));
    while(K!=1)
    {
        current = que.top();
        que.pop();
            A = pii(current.first, (current.second + current.first)/2);
            B = pii((current.second + current.first)/2, current.second);
         //   cout << A.first << " " << A.second << "    " << B.first << " " << B.second << endl;
            if(A.second - A.first >1)
            que.push(A);
            if(B.second - B.first >1)
            que.push(B);

        K--;
    }
            current = que.top();
            A = pii(current.first, (current.second + current.first)/2);
            B = pii((current.second + current.first)/2, current.second);
       //     cout << A.first << " " << A.second << "    " << B.first << " " << B.second << endl;
            int rezA = A.second - A.first-1;
            int rezB = B.second - B.first-1;
            return pii(max(rezA,rezB), min(rezA,rezB));
}

int main()
{

    int N, A, B;
    input >> N;
    for(int i = 0; i < N; i++)
    {
        cout << i << endl;
        input >> A >> B;
        pii ans = solve(A,B);
        out << "Case #" << i+1 <<": " << ans.first << " " << ans.second << endl;
    }
    return 0;
}
