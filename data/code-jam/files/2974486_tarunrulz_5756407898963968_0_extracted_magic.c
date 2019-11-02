#include<stdio.h>

int main()
{
		int testcase,i,j,row1,row2, card1[4][4],card2[4][4], count=0, value;
		scanf("%d", &testcase);
		int testcase2 = testcase;
		while(testcase--)
		{	
			count = 0;
			//Input
			scanf("%d", &row1);
			for(i = 0; i < 4; i++)
				for(j = 0; j < 4; j++)
					scanf("%d", &card1[i][j]);
			scanf("%d", &row2);
			for(i = 0; i < 4; i++)
				for(j = 0; j < 4; j++)
					scanf("%d", &card2[i][j]);
			
			//Calculations
			for(i = 0; i < 4; i++)
				for(j = 0; j < 4; j++)
					if(card1[row1-1][i] == card2[row2-1][j])
					{
						value = card1[row1-1][i];
						count++;
					}
					
			if(count == 1)
				printf("Case #%d: %d\n", testcase2-testcase, value);
			else if(count > 1)
				printf("Case #%d: Bad magician!\n", testcase2-testcase);
			else if(count == 0)
				printf("Case #%d: Volunteer cheated!\n", testcase2-testcase);
				
		}
}
