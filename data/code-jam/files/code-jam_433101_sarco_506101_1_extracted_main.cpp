#include "BigUnsigned.h"

#include <stdio.h>
#include <fstream>
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<algorithm>
#include<functional>
#include<iomanip>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<cstdlib>


using namespace std;

#define IN_PATH ("c:\\codejam\\Q2\\1.in" )
#define OUT_PATH ("c:\\codejam\\Q2\\1.out" )
#define MAX_N (1003)

long long T=0, N=0, K=0;

//=======================================
int sign;
int ch;
inline void getBigNum( FILE *fin, BigUnsigned &x )
{
	x=0;
	while((ch<'0' || ch>'9') && ch!='-' && ch!=EOF)	ch=getc( fin );
	if (ch=='-')
		sign=-1 , ch=getc( fin );
	else
		sign=1;
	do
	{
		char val = ch - '0';
		x=(x<<3)+(x<<1)+val;
	}
	while((ch=getc( fin ))>='0' && ch<='9');
		x*=sign;
}
//========================================

BigUnsigned gcd( BigUnsigned a, BigUnsigned b ) {  if( b == 0 ) return a;  return gcd( b, a % b ); }

BigUnsigned getGcdOfArray( BigUnsigned *tArr, BigUnsigned N )
{
	BigUnsigned commonDivisor = tArr[0];
	for ( int i = 1; N > i; ++i )
	{
		commonDivisor = gcd( tArr[i], commonDivisor );
	}
	return commonDivisor;
}

BigUnsigned getMinimum( BigUnsigned *tArr, BigUnsigned N )
{
	BigUnsigned min = tArr[0];
	for ( int i =0; N > i; ++i )
	{
		if ( tArr[i] < min)
		{
			min = tArr[i];
		}
	}
	return min;
}

void printBigNum( ofstream &fout, BigUnsigned T )
{
	if ( T == 0 ) 
	{
		fout << '0';
		return;
	}

	int numOfChars = 1;
	BigUnsigned divisor = 1;
	while ( T >= divisor * 10 )
	{
		divisor = divisor * 10;		
		numOfChars++;
	}
	
	for ( int i = 0; i < numOfChars; ++i )
	{
		char val = (T / divisor).toShort();
		fout.put( val + '0');
		T %= divisor;
		divisor /= 10;
	}
}

void printBigNumToScreen( BigUnsigned T )
{
	if ( T == 0 ) 
	{
		cout << '0';
		return;
	}

	int numOfChars = 1;
	BigUnsigned divisor = 1;
	while ( T >= divisor * 10 )
	{
		divisor = divisor * 10;		
		numOfChars++;
	}
	
	for ( int i = 0; i < numOfChars; ++i )
	{
		char val = (T / divisor).toShort();
		cout.put( val + '0');
		T %= divisor;
		divisor /= 10;
	}
}


void mySwap( BigUnsigned &x, BigUnsigned &y )
{
	BigUnsigned temp = x;
	x = y;
	y= temp;
}

void mySort( BigUnsigned  *arr, BigUnsigned N )
{
	BigUnsigned min;
	int minIndex = 0;
	for ( int i =0; N > i; ++i )
	{
		min = arr[i];
		minIndex = i;
		for ( int j = i+1; N > j; ++j )
		{
			if ( arr[j] < min )
			{
				minIndex = j;
				min = arr[j];
			}
		}
		if ( minIndex != i )
		{
			swap( arr[minIndex], arr[i] );
		}
	}
}


int main()
{
	cout << "hello world" << endl;

	FILE *fin = fopen( IN_PATH, "r" );
	ofstream fout( OUT_PATH );
	BigUnsigned C, N, tArr[ MAX_N], diffArr[MAX_N];
	getBigNum( fin, C );
	printBigNumToScreen( C);
	for ( int i =0; C > i; ++i )
	{
		if ( i == 99 )
		{
			cout << "99"<<endl;
		}
		cout << i << endl;
		getBigNum( fin, N );
		for ( int j = 0; N > j; ++j )
		{
			getBigNum( fin, tArr[j] );
		}
		mySort( tArr, N );
		for ( int j = 0; N-1 > j; ++j )
		{
			diffArr[j] = tArr[j+1] - tArr[j];
		}
		BigUnsigned commonDivisor = getGcdOfArray( diffArr, N-1 );
		BigUnsigned min = getMinimum( tArr, N );
		BigUnsigned shift = min % commonDivisor;
		BigUnsigned T = 0;
		if (shift > 0 )
		{
			T = commonDivisor - shift;
		}
		fout << "Case #" << i+1 << ": ";
		printBigNum( fout, T );
		if ( C -1 > i )
		{
			fout << endl;
		}
	}

	fout.close();
	fclose(fin);
	return 0;
}