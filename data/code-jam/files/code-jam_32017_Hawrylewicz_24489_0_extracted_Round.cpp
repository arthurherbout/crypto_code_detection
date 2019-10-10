#include <iostream>
#include <tchar.h>

#include <list>
#include <string>
#include <vector>

using namespace std;

void doIt(vector<int> &_cards)
{
	const size = _cards.size();
	int cardsLeft = size;
	vector<bool> _inUse;
	_inUse.resize(size);

	for (int i = 0; i < size; ++i)
	{
		_inUse[i]= false;
		_cards[i] = 0;
	}

	int pos = 0;
	int steps = 0;
	int cardCounter = 1;
	while (cardsLeft)
	{
		while (_inUse[pos])
		{
			++pos;
			if (pos == size)
				pos = 0;
		}

		if (steps + 1  == cardCounter)
		{
			_cards[pos] = cardCounter;
			_inUse[pos] = true;
			steps = 0;
			++cardCounter;
			--cardsLeft;
			++pos;
			if (pos == size)
				pos = 0;
			continue;
		}
		else
		{
			++pos;
			if (pos == size)
				pos = 0;
		}
		++steps;
	}
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	const int BUF_SIZE = 101;
	char _buf[BUF_SIZE];

	int _caseCount;
	cin >> _caseCount;
	cin.getline(_buf, BUF_SIZE);

	for (int _case = 0; _case < _caseCount; ++_case)
	{
		int _cardCount;
		cin >> _cardCount;
		cin.getline(_buf, BUF_SIZE);

		int _pozycjeCount;
		cin >> _pozycjeCount;
		vector<int> _pozycje;
		vector<int> _cards;
		_pozycje.resize(_pozycjeCount);
		_cards.resize(_cardCount);
		
		for (int i = 0; i < _pozycjeCount; ++i)
			cin >> _pozycje[i];

        doIt(_cards);
		cout << "Case #" << _case + 1 << ": " ;
		
		for (int i = 0; i < _pozycjeCount; ++i)
			cout << _cards[_pozycje[i] - 1] << " ";
		
		cout << "\n";
	}	

	return 0;
}
