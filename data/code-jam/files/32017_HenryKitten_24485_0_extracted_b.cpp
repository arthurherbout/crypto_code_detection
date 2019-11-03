#include <iostream>
#include <set>

using namespace std;

long long gcd(long long a,long long b){
	if(b==0) return a;
	return gcd(b,a%b);
}

int max_prime_fac[1001];
int nset[1001];

int primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997,1009};

void init(){
	for(int i=1;i<=1000;i++){
		for(int j=0;primes[j]<=1000;j++){
			if(i%primes[j] == 0) max_prime_fac[i] = primes[j];
		}
	}
}

int tofind(int a){
	if(nset[a] == a) return a;
	return nset[a] = tofind(nset[a]);
}

int tounion(int a,int b){
	nset[tofind(b)] = a;
}

int main(){
	int ncas,ti;
	init();
	cin >> ncas;
	for(ti=1;ti<=ncas;ti++){
		int A,B,P;
		cin >> A >> B >> P;
		for(int i=A;i<=B;i++) nset[i] = i;

		bool flag = 1;
		while(flag){
			flag = 0;
			for(int i=A;i<=B;i++){
				for(int j=A;j<=B;j++){
					if(tofind(i) != tofind(j) && max_prime_fac[gcd(i,j)] >= P){
						tounion(i,j);
						flag = 1;
					}
				}
			}
		}
		set<int> s;
		for(int i=A;i<=B;i++) s.insert(nset[i]);
		printf("Case #%d: ",ti);
		cout << s.size() << endl;
	}
	return 0;
}
