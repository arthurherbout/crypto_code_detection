#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define lint long long
#define uint unsigned long long

struct card
{
	int turns;
	int points;
	int draws;
};

card Deck[80];
int deckSize;

struct game
{
	int points;
	int turns;
	int deckPos;
	bool cardInHand[80];
	
	void playCard(int cardNum)
	{
		cardInHand[cardNum]=false;
		points+=Deck[cardNum].points;
		deckPos+=Deck[cardNum].draws;
		if(deckPos > deckSize)
			deckPos = deckSize;
		turns +=Deck[cardNum].turns-1;
	}
};


int playGame(game g)
{
	if(g.turns==0)
		return g.points;
	
	int bestPointsCard = -1, bestDrawsCard = -1;
	
	//play all turn cards
	for(int c=0; c<g.deckPos; c++)
	{
		if(g.cardInHand[c])
		{
			if(Deck[c].turns > 0)
			{
				g.playCard(c);
				continue;
			}
			
			if(bestPointsCard <0 || Deck[c].points > Deck[bestPointsCard].points
			   || (Deck[c].points == Deck[bestPointsCard].points && Deck[c].draws > Deck[bestDrawsCard].draws))
				bestPointsCard = c;
			
			if(bestDrawsCard <0 || Deck[c].draws > Deck[bestDrawsCard].draws
			   || (Deck[c].draws == Deck[bestDrawsCard].draws && Deck[c].points > Deck[bestPointsCard].points))
				bestDrawsCard = c;
			
		}
	}
	
	if(bestPointsCard > -1 && bestDrawsCard > -1 && g.deckPos< deckSize )
	{
		if(bestDrawsCard == bestPointsCard)
		{
			g.playCard(bestPointsCard);
			return playGame(g);
		}
		game g2 = g;
		g.playCard(bestPointsCard);
		g2.playCard(bestDrawsCard);
		int gPoints = playGame(g);
		int g2Points = playGame(g2);
		if(gPoints > g2Points)
			return gPoints;
		return g2Points;
		
	}
	else if (bestPointsCard > -1)
	{
		g.playCard(bestPointsCard);
		return playGame(g);
	}
	else if(bestDrawsCard > -1)
	{
		g.playCard(bestDrawsCard);
		return playGame(g);
	}
	
	return g.points;
	
	
}


int main (int argc, char * const argv[]) {
    
	ifstream inFile("../../input.txt");
	ofstream outFile("../../output.txt");
	int numCases;
	inFile >> numCases;
	for(int caseNum =1; caseNum <=numCases; caseNum++)
	{
		
		game initialHand;
		initialHand.turns=1;
		for(int c=0; c<80; c++)
			initialHand.cardInHand[c]=true;
		initialHand.points=0;
		
		inFile>>initialHand.deckPos;
		for(int c=0; c< initialHand.deckPos; c++)
		{
			inFile>>Deck[c].draws;
			inFile>>Deck[c].points;
			inFile>>Deck[c].turns;
		}
		
		inFile>>deckSize;
		deckSize+=initialHand.deckPos;
		for(int c= initialHand.deckPos; c< deckSize; c++)
		{
			inFile>>Deck[c].draws;
			inFile>>Deck[c].points;
			inFile>>Deck[c].turns;
			
		}
		
		outFile<<"Case #"<<caseNum<<": "<<playGame(initialHand)<<"\n";
		cout<<"Case #"<<caseNum<<": "<<playGame(initialHand)<<"\n";
	}
	inFile.close();
	outFile.close();
	std::cout << "Done!\n";
    return 0;
}


