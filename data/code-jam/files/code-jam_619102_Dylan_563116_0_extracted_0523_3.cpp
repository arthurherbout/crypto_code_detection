// Minimum Scalar Product.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <afx.h>
#include <map>
#include <vector>
#include <stdlib.h>

using namespace std;
int mat[513][513];
int M,N;
int use[513][513];
int R[513];
int HowBig(int m,int n)
{
	int iIndex;
	int iRet = 1;
	for (iIndex = 1;iIndex<512;iIndex++)
	{
		if (m+iIndex >= M||n+iIndex >=N)
		{
			break;
		}
		int i1,i2;
		for ( i1 = m;i1<=m+iIndex;i1++)
		{
			for (  i2= n;i2<=n+iIndex;i2++)
			{
				if ((i1 + i2 - m - n )%2 == 0)
				{
					if (mat[i1][i2]*mat[m][n]!=4&&mat[i1][i2]*mat[m][n]!=1)
					{
						return iRet;
					}
				}
				if ((i1 + i2 - m - n )%2 == 1)
				{
					if (mat[i1][i2]*mat[m][n]!=2)
					{
						return iRet;
					}
				}
			}
			if (i2 != n+iIndex+1)
			{
				return iRet;
			}
		}
		if ((i1 == m+iIndex+1)&&(i2==n+iIndex+1))
		{
			iRet++;
		}
	}
	return iRet;
}
BOOL Use(int m,int n,int size)
{
	BOOL bRet = TRUE;
	int i1,i2;
	for ( i1 = m;i1<m+size;i1++)
	{
		for (  i2= n;i2<n+size;i2++)
		{
			mat[i1][i2] = 0;
		}
	}
	return bRet;
}
int _tmain(int argc, _TCHAR* argv[])
{

	CStdioFile cstdfRead;
	CStdioFile cstdfWrite;
	wchar_t databuffer[16*1024];
	INT64 iNumOfCases;
	wchar_t * seps = L" ";
	wchar_t * pwcToken;
	INT64 iTemp,iResult;
	int iBigSize,iNowSize;

	//cstdfRead.Open(L"D:\\A-large-practice.in.txt",CStdioFile::modeRead);
	cstdfRead.Open(L"test.txt",CStdioFile::modeRead);
	cstdfWrite.Open(L"out.txt",CStdioFile::modeCreate);
	cstdfWrite.Close();
	cstdfWrite.Open(L"out.txt",CStdioFile::modeWrite);

	memset(databuffer,0,sizeof(databuffer));
	cstdfRead.ReadString(databuffer,sizeof(databuffer)-1);
	iNumOfCases = _wtoi(databuffer);

	for (int iIndex1 = 0;iIndex1 < iNumOfCases;iIndex1++)
	{
		iBigSize = 1;
		memset(R,0,sizeof(R));
		memset(mat,0,sizeof(mat));
		memset(databuffer,0,sizeof(databuffer));
		cstdfRead.ReadString(databuffer,sizeof(databuffer)-1);
		pwcToken = wcstok(databuffer,seps);
		M = _wtoi(pwcToken);
		pwcToken = wcstok(NULL,seps);
		N = _wtoi(pwcToken);
		for (int iIndex2 = 0;iIndex2<M;iIndex2++)
		{
			memset(databuffer,0,sizeof(databuffer));
			cstdfRead.ReadString(databuffer,sizeof(databuffer)-1);
			pwcToken = databuffer;
			for (int iIndex3 = 0;iIndex3<N/4;iIndex3++)
			{
				int temp = *pwcToken - L'0';
				if (temp > 9)
				{
					temp -= 7;
				}
				mat[iIndex2][4*iIndex3+3] = temp%2 +1;
				temp/=2;
				mat[iIndex2][4*iIndex3+2] = temp%2 +1;
				temp/=2;
				mat[iIndex2][4*iIndex3+1] = temp%2 +1;
				temp/=2;
				mat[iIndex2][4*iIndex3] = temp%2 +1;
				pwcToken++;
			}
		}
		int m,n,iSize;
		for (m = 0;m<M;m++)
		{
			for (n = 0;n<N;n++)
			{
				iSize = HowBig(m,n);
				if (iSize > iBigSize)
				{
					iBigSize = iSize;
				}
			}
		}
		R[1] = M*N;
		for (int i = iBigSize;i>1;i--)
		{
			for (m = 0;m<M;m++)
			{
				for (n = 0;n<N;n++)
				{
					iSize = HowBig(m,n);
					if (iSize == i)
					{
						Use(m,n,i);
						R[i]++;
					}
				}
			}
			R[1] -= R[i]*i*i;
		}
		iResult = 0;
		for (int iIndex4 = 0;iIndex4<=iBigSize;iIndex4++)
		{
			if (R[iIndex4] > 0)
			{
				iResult++;
			}
		}

		wchar_t num[64];
		_i64tow(iResult,num,10);
		memset(databuffer,0,sizeof(databuffer));
		wsprintf(databuffer,L"Case #%d: %s\n",iIndex1+1,num);
		cstdfWrite.WriteString(databuffer);
		for (int iIndex5 = iBigSize;iIndex5>=1;iIndex5--)
		{
			if (R[iIndex5] >0)
			{
				memset(databuffer,0,sizeof(databuffer));
				wsprintf(databuffer,L"%d %d\n",iIndex5,R[iIndex5]);
				cstdfWrite.WriteString(databuffer);
			}
		}
	}

	cstdfRead.Close();
	cstdfWrite.Close();
	return 0;
}

