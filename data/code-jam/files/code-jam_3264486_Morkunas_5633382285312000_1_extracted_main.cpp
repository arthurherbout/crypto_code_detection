#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;
ifstream input ("B-large.in");
ofstream out ("B-large.out");
bool isInOrder(long long int N){
long long int previousDigit = 10LL, currentDigit;
while(N!=0LL){
    currentDigit = N%10LL;
    if(currentDigit > previousDigit){
        return false;
    }
    previousDigit = currentDigit;
    N/=10LL;
}
return true;
}
long long int solve(long long int N){
    long long int numb = 1LL,currentDigit;
    while(!isInOrder(N))
    {

        currentDigit = N/numb%10LL;
        N-=(currentDigit+1)*numb;
        numb*=10LL;
    }
    return N;
}
int main()
{
    long long int N, numb;
    input >> N;
    for(int i = 0; i < N; i++)
    {
        input >> numb;
        out << "Case #" << i+1 <<": " << solve(numb) << endl;
    }
    return 0;
}

