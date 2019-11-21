#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct combiner
{
	char base1;
	char base2;
	char result;
};

struct opposer
{
	char base1;
	char base2;
};

int main()
{
	int t;
	
	cin >> t;
	
	for (int i = 0; i < t; i++)
	{		
		int num_combiners;
		cin >> num_combiners;

		vector<combiner> combiners(num_combiners);
		
		for (int j = 0; j < combiners.size(); j++)
		{
			cin >> combiners[j].base1 >> combiners[j].base2 >> combiners[j].result;
		}
		
		int num_opposers;
		cin >> num_opposers;
		
		vector<opposer> opposers(num_opposers);
		
		for (int j = 0; j < opposers.size(); j++)
		{
			cin >> opposers[j].base1 >> opposers[j].base2;
		}
		
		int num_elements;
		cin >> num_elements;
		
		string elements;
		
		for (int j = 0; j < num_elements; j++)
		{
			char element;
			cin >> element;

			elements = elements + element;
			
			if (elements.size() >= 2)
			{
				char element1 = elements[elements.size() - 1];
				char element2 = elements[elements.size() - 2];
				
				bool combined = false;
				
				for (int k = 0; k < combiners.size(); k++)
				{
					if (((element1 == combiners[k].base1) && (element2 == combiners[k].base2)) ||
						((element1 == combiners[k].base2) && (element2 == combiners[k].base1)))
					{
						elements.erase(elements.size() - 1);
						elements.erase(elements.size() - 1);
						elements = elements + combiners[k].result;
						
						combined = true;
					}
				}

				if (!combined)
				{				
					for (int k = 0; k < opposers.size(); k++)
					{
						for (int l = 0; l < elements.size(); l++)
						{
							if (((element1 == opposers[k].base1) && (elements[l] == opposers[k].base2)) ||
								((element1 == opposers[k].base2) && (elements[l] == opposers[k].base1)))
							{
								elements.clear();
							}
						}
					}
				}
			}
		}
		
		cout << "Case #" << (i + 1) << ": [";
		
		for (int j = 0; j < elements.size(); j++)
		{
			cout << elements[j];
			
			if (j + 1 < elements.size())
			{
				cout << ", ";
			}
		}
		
		cout << "]\n";
	}
}
