#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_LETTERS				1001

struct LetterInfo
{
	int iFrequency;
	int iKey;
	int iPlaceInKey;
};



int compare (const void * a, const void * b)
{
	int * ptrA = (int*)a;
	int * ptrB = (int*)b;

    return (ptrB[0] - ptrA[0]);
}



int fnFindMinimumKeyTyping(LetterInfo pArrLetterFrequecies[], int pMaxLettersOnAKey, int pNumKeys, int pNumLetters)
{
	int iNumTyppings = 0;

	/////*printf("Array unsorted\n");
	////for(int i=0; i < pNumLetters; i++)
	////	printf("%d ", pArrLetterFrequecies[i].iFrequency);
	////printf("\n");*/

	// Order by frequency descending
	// Elements, NumElements, SizeOfElement, Comparison
	qsort (pArrLetterFrequecies, pNumLetters, sizeof(int)*3, compare);
	
	/////*printf("Array sorted\n");
	////for(int i=0; i < pNumLetters; i++)
	////	printf("%d ", pArrLetterFrequecies[i].iFrequency);

	////printf("\n");*/

	// Assign key and place in key
	int iLetter = 0;
	// Cycle over place
	for(int i=1; i <= pMaxLettersOnAKey; i++)
	{
		// Cycle over key
		for(int j=1; j <= pNumKeys; j++)
		{
			pArrLetterFrequecies[iLetter].iKey = j;
			pArrLetterFrequecies[iLetter++].iPlaceInKey = i;
		}
	}
		
	/////*printf("Assigned keys and places in key\n");
	////for(int i=0; i < pNumLetters; i++)
	////	printf("%d %d %d\n", pArrLetterFrequecies[i].iFrequency, pArrLetterFrequecies[i].iKey, pArrLetterFrequecies[i].iPlaceInKey);
	////printf("\n");*/

	// Calculate num typpings
	iNumTyppings = 0;
	for(int i=0; i < pNumLetters; i++)
		iNumTyppings += pArrLetterFrequecies[i].iFrequency * pArrLetterFrequecies[i].iPlaceInKey;

	//////printf("Minimum typpings: %d\n", iNumTyppings);

	return iNumTyppings;

}


int main(int argc, char* argv[])
{
	LetterInfo ArrLetterFrequecies[MAX_NUM_LETTERS];
	char Aux[1000];


	int iMaxLettersOnAKey = 0, iNumKeys = 0, iNumLetters = 0, iMinimumKeyTyping = 0;
	int iCases = 0; 

	scanf("%d", &iCases);
	for(int i=0; i<iCases; i++)
	{
		scanf("%d %d %d", &iMaxLettersOnAKey, &iNumKeys, &iNumLetters);	gets(Aux);
		for(int j=0; j<iNumLetters; j++)
		{
			scanf("%d", &ArrLetterFrequecies[j].iFrequency);	
		}
		gets(Aux);
		
		iMinimumKeyTyping = fnFindMinimumKeyTyping(ArrLetterFrequecies, iMaxLettersOnAKey, iNumKeys, iNumLetters);

		printf("Case #%d: %d\n", i+1, iMinimumKeyTyping);
	}
	return 0;
}




