// BotTrust.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <deque>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int testcases;
	cin >> testcases;

	for(int i=0;i<testcases;++i)
	{
		int num;
		cin >> num;

		deque<int> orangebuttons;
		deque<int> bluebuttons;
		deque<char> order;
		for(int j=0 ; j<num ; ++j)
		{
			char turn;
			cin >> turn;

			order.push_back(turn);
			int btn;
			cin >> btn;
			if(turn == 'O')
				orangebuttons.push_back(btn);
			else
				bluebuttons.push_back(btn);
		}

		int orangepos = 1;
		int bluepos = 1;

		int bluenext = 1;
		int orangenext = 1;
		int moves = 0;

		if(!orangebuttons.empty())
		{
			orangenext = orangebuttons.front();
			orangebuttons.pop_front();
		}
		if(!bluebuttons.empty())
		{
			bluenext = bluebuttons.front();
			bluebuttons.pop_front();
		}
		char turn;

		while(!order.empty())
		{
			turn = order.front();
			order.pop_front();

			if(turn == 'O')
			{
				int diff = orangenext - orangepos;
				if (diff < 0) diff = -diff;
				diff++;

				orangepos = orangenext;

				if(bluepos < bluenext)
					bluepos = (bluepos + diff > bluenext)?bluenext:bluepos+diff;
				else if(bluepos > bluenext)
					bluepos = (bluepos - diff < bluenext)?bluenext:bluepos-diff;
				moves += diff;
				if(!orangebuttons.empty())
				{
					orangenext = orangebuttons.front();
					orangebuttons.pop_front();
				}
			}
			else
			{
				int diff = bluenext - bluepos;
				if (diff < 0) diff = -diff;
				diff++;

				bluepos = bluenext;

				if(orangepos < orangenext)
					orangepos = (orangepos + diff > orangenext)?orangenext:orangepos+diff;
				else if(orangepos > orangenext)
					orangepos = (orangepos - diff < orangenext)?orangenext:orangepos-diff;
				moves += diff;

				if(!bluebuttons.empty())
				{
					bluenext = bluebuttons.front();
					bluebuttons.pop_front();
				}

			}
		}
		if(bluenext!=bluepos)
		{
			int diff = bluenext - bluepos;
			if (diff < 0) diff = -diff;
			moves += diff;
		}
		if(orangenext != orangepos)
		{
			int diff = orangenext - orangepos;
			if (diff < 0) diff = -diff;
			moves += diff;
		}
		cout << "Case #" << i+1 << ": "<< moves << endl;
	}
	return 0;
}