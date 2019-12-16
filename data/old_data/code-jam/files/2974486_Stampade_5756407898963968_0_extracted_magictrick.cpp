#include <iostream>
#include <fstream>
#include<set> 

using namespace std;

int printmagictrickanswers(int argc, char** argv)
{
	if (argc != 2)
	{
		cout<<"supply file name plz"<<endl;
		return 0;
	}

	ifstream input(argv[1]);
	ofstream output("output");
	if (!input) 
	{
		cout<<"can't open file:"<<argv[1]<<endl;
		return 0;
	}

	int testcases;
	int pick;
	int flush;
	int count;
	int result;
	set<int> first;
	int j;
	input>>testcases;
	for ( int i = 0 ; i < testcases; i++)
	{
		count = 0;
		first.clear();
		input>>pick;
		for (j = 0; j < 4*(pick - 1); j++)
			input>>flush;

		for ( ; j < 4*pick; j++)
		{
			input>>flush;
			first.insert(flush);			
		}

		for ( ; j < 16; j++)
			input>>flush; 

		input>>pick;
		for (j = 0; j < 4*(pick - 1); j++)
			input>>flush;

		for (; j < 4*pick ; j++)
		{
			input>>flush;
			if(first.find(flush) != first.end())
			{
				result = flush;
				count++;
			}			
		}

		for ( ; j < 16; j++)
			input>>flush; 

		if (count == 0 )
			output<<"Case #"<<i+1<<": "<<"Volunteer cheated!"<<endl;
		else if (count != 1)
			output<<"Case #"<<i+1<<": "<<"Bad magician!"<<endl;
		else
			output<<"Case #"<<i+1<<": "<<result<<endl;
	}
}