#include<iostream>
#include<limits.h>
#include<cmath>
#include<string>
#include<sstream>

using namespace std;

bool isPalindrome(unsigned long long i){

    std::stringstream ss;
    ss.clear();
    ss << i;
    string s = ss.str();
    int low = 0;
    int high = s.length()-1;
    if(high == 0 ) return true;

    while( low < high && s[low] == s[high] ) {low++; high--;}
 
    if (low >= high)return true;

return false;
}
int output(string A, string B){

    unsigned long long  x,y;
    
    std::stringstream ss;
    ss.clear();
    ss.str(A.c_str());
    ss >> x;
    ss.clear();
    ss.str(B.c_str());
    ss >> y;

    if(y < ULLONG_MAX && x < ULLONG_MAX){
	int i = sqrt(x);
	if(i*i != x)i++;
	int max = sqrt(y);
	int count = 0;
	while(i <= max)
	{
		
		if(isPalindrome(i) && isPalindrome(i*i)){
			count++;
//		cout << "\nIs expected "<< i*i <<"  " <<  i;
		}
		i++;
	}
	return count;
     }
     return -1;
}

int main(){

	int n;
	cin >> n;
	string A,B;

	int i = 0;
	while(++i <= n){
		cin >> A >> B;
		cout << "Case #" << i << ": " <<output(A,B)<< endl;
	}

//char *A = "18446744073709551515";

	return 0;
}
