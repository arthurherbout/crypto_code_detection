
#include <stdio.h>
#include <string.h>

#define MAX_SEARCH_ENGINES				201
#define MAX_QUESTS_FOR_ENGINES			2001
#define MAX_LENGTH_NAME_SEARCH_ENGINE	201


char sEngines[MAX_SEARCH_ENGINES][MAX_LENGTH_NAME_SEARCH_ENGINE];
char sQuests[MAX_QUESTS_FOR_ENGINES][MAX_LENGTH_NAME_SEARCH_ENGINE];

void fnConvertToNumbers(int pEnginesCount, int pQuestsCount, int pIntArrEngines[], int pIntArrQuests[])
{

	for(int i=1; i<=pEnginesCount; i++)
	{
		pIntArrEngines[i] = i;
	}

	for(int i=1; i<=pQuestsCount; i++ )
	{
		for(int j=1; j<=pEnginesCount; j++)
		{
			if(strcmp(sQuests[i], sEngines[j]) == 0)
			{
				pIntArrQuests[i] = j; break;
			}
		}
	}

}


int fnFindMinimumSwitchings(int pIntArrEngines[], int pEnginesCount, int pIntArrQuests[], int pQuestsCount)
{
	int iNumOfSwitchs = 0, iMaxSequence = 0, iSequenceAux = 0, iEngineWithMaxSequence = 0, iCurQuestIndex = 1;

	while(iCurQuestIndex <= pQuestsCount)
	{
		for(int Engine=1; Engine<=pEnginesCount; Engine++)
		{
			iSequenceAux = 0; 

			for(int j=iCurQuestIndex; j<=pQuestsCount; j++ )	
			{
				if(pIntArrQuests[j] == Engine)	break;
				
				iSequenceAux++;
			}

			if(iSequenceAux > iMaxSequence) iMaxSequence = iSequenceAux;
		}

		iCurQuestIndex += iMaxSequence;
		iMaxSequence = 0;
		if(iCurQuestIndex <= pQuestsCount) iNumOfSwitchs++;
	}

	return iNumOfSwitchs;
}


int main()
{
	
	int iArrEngines[MAX_SEARCH_ENGINES];
	int iArrQuests[MAX_QUESTS_FOR_ENGINES];
	char Aux[1000];
	
	int iEnginesCount = 0, iQuestsCount = 0, iMinimumSwitchings = 0;

	int iCases = 0; 

	scanf("%d", &iCases);
	for(int i=0; i<iCases; i++)
	{
		scanf("%d", &iEnginesCount);	gets(Aux);
		for(int i=1; i<=iEnginesCount; i++)
		{
			gets(sEngines[i]);
		}
		scanf("%d", &iQuestsCount);	gets(Aux);
		for(int i=1; i<=iQuestsCount; i++)
		{
			gets(sQuests[i]);
		}
		
		fnConvertToNumbers(iEnginesCount, iQuestsCount, iArrEngines, iArrQuests);
		iMinimumSwitchings = fnFindMinimumSwitchings(iArrEngines, iEnginesCount, iArrQuests, iQuestsCount);

		printf("Case #%d: %d\n", i+1, iMinimumSwitchings);
	}
	return 0;
}


