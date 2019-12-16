#include <stdio.h>

typedef struct 
{
	int feet;
	int inches;
}distance;
distance addDistances(distance *d1 , distance *d2)
{
	distance add;
	while(d1->inches>=12)
	{
		d1->feet++;
		d1->inches -= 12;
	}
	while(d2->inches>=12)
	{
		d2->feet++;
		d2->inches -= 12;
	}
	add.feet = d1->feet + d2->feet;
	add.inches = d1->inches + d2->inches;
	while(add.inches>=12)
	{
		add.feet++;
		add.inches -= 12;
	}
	return add;
}
distance subtractDistances(distance *d1 , distance *d2)
{
	distance sub;
	while(d1->inches>=12)
	{
		d1->feet++;
		d1->inches -= 12;
	}
	while(d2->inches>=12)
	{
		d2->feet++;
		d2->inches -= 12;
	}
	if(d1->feet > d2->feet)
	{
		sub.feet = d1->feet - d2->feet;
	}
	else
	{
		sub.feet = d2->feet - d1->feet;
	}
	if(d1->inches > d2->inches)
	{
		sub.inches = d1->inches - d2->inches;	
	}
	else
	{
		sub.inches = d2->inches - d1->inches;
	}
	
	while(sub.inches<0)
	{
		sub.feet--;
		sub.inches += 12;
	}
	return sub;
}
int main()
{
	distance d1 , d2;
	printf("Enter feet for distance 1 :");
	scanf("%d",&d1.feet);
	printf("Enter inches for distance 1 : ");
	scanf("%d",&d1.inches);
	printf("Enter feet for distance 2 :");
	scanf("%d",&d2.feet);
	printf("Enter inches for distance 2 : ");
	scanf("%d",&d2.inches);
	distance add = addDistances(&d1,&d2);
	printf("Distance after adding is %d feet %d inches\n",add.feet,add.inches);
	distance subtract = subtractDistances(&d1,&d2);
	printf("Distance after subtracting is %d feet %d inches\n",subtract.feet,subtract.inches);
    return 0;
}
