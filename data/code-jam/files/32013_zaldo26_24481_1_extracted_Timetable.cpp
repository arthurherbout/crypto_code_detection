

#include <stdio.h>
#include <stdlib.h>

#define MAX_DEPARTURES					300
#define MAX_TURN_AROUND_TIME			130

#define DEPARTURE		-10
#define ARRIVAL			-20


int iArrTimeTableAtA[MAX_DEPARTURES*3][2];
int iArrTimeTableAtB[MAX_DEPARTURES*3][2];

	 
void fnFindRequiredTrains(int pTurnAroundTime, int pDeparturesFromA, int pDeparturesFromB, int *pRequiredTrainsAtA, int *pRequiredTrainsAtB)
{
	if((pDeparturesFromA == 0) || (pDeparturesFromB == 0)) {*pRequiredTrainsAtA = pDeparturesFromA; *pRequiredTrainsAtB = pDeparturesFromB; return; }

	int iTrainsNotAvailable = 0, iAvailable = 0;
	for(int i=1; i<=(pDeparturesFromA + pDeparturesFromB); i++)
	{
		if(iArrTimeTableAtA[i][0] == DEPARTURE)
		{
			if(iAvailable > 0) iAvailable--; else iTrainsNotAvailable++;
		}
		else if(iArrTimeTableAtA[i][0] == ARRIVAL)
		{
			iAvailable++;
		}
	}
	*pRequiredTrainsAtA = iTrainsNotAvailable;


	iTrainsNotAvailable = 0; iAvailable = 0;
	for(int i=1; i<=(pDeparturesFromA + pDeparturesFromB); i++)
	{
		if(iArrTimeTableAtB[i][0] == DEPARTURE)
		{
			if(iAvailable > 0) iAvailable--; else iTrainsNotAvailable++;
		}
		else if(iArrTimeTableAtB[i][0] == ARRIVAL)
		{
			iAvailable++;
		}
	}
	*pRequiredTrainsAtB = iTrainsNotAvailable;
}


int compare (const void * a, const void * b)
{
	int * ptrA = (int*)a;
	int * ptrB = (int*)b;

	if(ptrA[1] != ptrB[1])
		return (ptrA[1] - ptrB[1]);
	else
	{
		// If equal, priorize arrivals
		if(ptrA[0] == ARRIVAL) return -1;
		else if(ptrB[0] == ARRIVAL) return 1;
		else return 0;
	}
}


void fnOrderDepartures(int pDeparturesFromA, int pDeparturesFromB)
{
	int * ptrAux = (int *)iArrTimeTableAtA; ptrAux++; ptrAux++;
	qsort (ptrAux, pDeparturesFromA + pDeparturesFromB, sizeof(int)*2, compare);

	ptrAux = (int *)iArrTimeTableAtB; ptrAux++; ptrAux++;
	qsort (ptrAux, pDeparturesFromA + pDeparturesFromB, sizeof(int)*2, compare);
}

void fnPrintTables(int iDeparturesFromA, int iDeparturesFromB)
{
	char* sArriv = "ARRIVAL";
	char* sDept = "DEPARTURE";


	printf("\nTimetable at A:\n");
	for(int i=1; i<=(iDeparturesFromA + iDeparturesFromB); i++)
	{
		if(iArrTimeTableAtA[i][0] == ARRIVAL)
			printf("%s\t%d \n", sArriv, iArrTimeTableAtA[i][1]);
		else if(iArrTimeTableAtA[i][0] == DEPARTURE)
			printf("%s\t%d \n", sDept, iArrTimeTableAtA[i][1]);
	}
	printf("Timetable at B:\n");
	for(int i=1; i<=(iDeparturesFromA + iDeparturesFromB); i++)
	{
		if(iArrTimeTableAtB[i][0] == ARRIVAL)
			printf("%s\t%d \n", sArriv, iArrTimeTableAtB[i][1]);
		else if(iArrTimeTableAtB[i][0] == DEPARTURE)
			printf("%s\t%d \n", sDept, iArrTimeTableAtB[i][1]);
	}
}

int main()
{
	char Aux[1000];
	
	int iTurnAroundTime = 0, iDeparturesFromA = 0, iDeparturesFromB = 0;
	int iHour1 = 0, iMinutes1 = 0, iHour2 = 0, iMinutes2 = 0;
	int iRequiredTrainsAtA = 0, iRequiredTrainsAtB = 0;

	int iCases = 0; 

	scanf("%d", &iCases);
	for(int i=0; i<iCases; i++)
	{
		scanf("%d", &iTurnAroundTime);	gets(Aux);
		scanf("%d %d", &iDeparturesFromA, &iDeparturesFromB);	gets(Aux);
		
		for(int i=1; i<=(iDeparturesFromA + iDeparturesFromB); i++)
		{
			scanf("%d:%d %d:%d", &iHour1, &iMinutes1, &iHour2, &iMinutes2);	gets(Aux);

			// Timetable just at A
			if(i<=iDeparturesFromA)
			{
				iArrTimeTableAtA[i][0] = DEPARTURE;
				iArrTimeTableAtA[i][1] = iHour1*60 + iMinutes1;

				iArrTimeTableAtB[i][0] = ARRIVAL;
				iArrTimeTableAtB[i][1] = iHour2*60 + iMinutes2 + iTurnAroundTime;
			}
			else // Timetable just at B
			{
				iArrTimeTableAtB[i][0] = DEPARTURE;
				iArrTimeTableAtB[i][1] = iHour1*60 + iMinutes1;

				iArrTimeTableAtA[i][0] = ARRIVAL;
				iArrTimeTableAtA[i][1] = iHour2*60 + iMinutes2 + iTurnAroundTime;
			}
		}

		/*printf("Before order:");
		fnPrintTables(iDeparturesFromA, iDeparturesFromB);*/

		fnOrderDepartures(iDeparturesFromA, iDeparturesFromB);

		/*printf("After order:");
		fnPrintTables(iDeparturesFromA, iDeparturesFromB);*/

		fnFindRequiredTrains(iTurnAroundTime, iDeparturesFromA, iDeparturesFromB, &iRequiredTrainsAtA, &iRequiredTrainsAtB);

		printf("Case #%d: %d %d\n", i+1, iRequiredTrainsAtA, iRequiredTrainsAtB);
	}
	return 0;
}


