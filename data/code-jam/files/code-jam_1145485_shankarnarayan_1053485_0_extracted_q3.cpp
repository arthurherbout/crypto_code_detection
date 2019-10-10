#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<vector>

using namespace std;

struct cards{
	int c;
	int s;
	int t;
};

vector<cards> hand,deck;
cards temp;
int T,N,M;
int turnsLeft=1,score=0;

int findNext()
{
	int best=0;
	for(int i=1;i<hand.size();i++)
	{
		if(hand[i].t>hand[best].t)
			best=i;
		else if(hand[i].t==hand[best].t && hand[i].t>0)
		{
			if(hand[i].c>hand[best].c)
				best=i;
			else if(hand[i].c==hand[best].c)
			{
				if(hand[i].s>hand[best].s)
					best=i;
			}
			
		}
		else if(hand[best].t==0)
		{
			if(hand[i].s>hand[best].s)
				best=i;
		}
	}
	return best;
}

void drawCards(int n)
{
	if(n>deck.size())
		n=deck.size();
	for(int i=0;i<n;i++)
	{
		hand.push_back(deck[i]);
		deck.erase(deck.begin()+i);
	}
}
int main()
{
	
	int x;
	cin>>T;
	for(int t=0;t<T;t++)
	{
		hand.clear();
		deck.clear();
		turnsLeft=1;score=0;
		cin>>N;
		for(int i=0;i<N;i++)
		{
			cin>>temp.c>>temp.s>>temp.t;
			hand.push_back(temp);
		}
		cin>>M;
		for(int i=0;i<M;i++)
		{
			cin>>temp.c>>temp.s>>temp.t;
			deck.push_back(temp);
		}
	//	cout<<"Play: \n";
		while(hand.size()>0 && turnsLeft>0)
		{
			x=findNext();
	//		cout<<hand[x].c<<"\t"<<hand[x].s<<"\t"<<hand[x].t<<endl;
			turnsLeft+=(hand[x].t-1);
			score+=hand[x].s;
			drawCards(hand[x].c);
			hand.erase(hand.begin()+x);
		}
		cout<<"Case #"<<t+1<<": "<<score<<endl;
	}
	return 0;
}