#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

typedef map<char, char> GooToEngMap;

string Translate(const string& goo, 
	GooToEngMap& gooEngMap)
{
	string eng(goo);
	for (unsigned i = 0; i < goo.size(); i++)
	{
		eng[i] = gooEngMap[goo[i]];
	}
	return eng;
}

int main()
{
	ifstream f;
	f.open("a_sample_manual.txt");
	if (f.fail())
	{
		cout << "File not found" << endl;
		return 1;
	}

	//make the map
	GooToEngMap gooEngMap;
	
	string goo;
	char cgoo[200];
	string eng;
	char ceng[200];

	for (unsigned int t = 0; t < 3; t++)
	{
		f.getline(cgoo, 100);
		goo = string(cgoo);
		f.getline(ceng, 100);
		eng = string(ceng);

		for(unsigned int i = 0; i < goo.size(); i++)
		{
			gooEngMap[goo[i]] = eng[i];
			assert(eng[i] != 'z');
		}
	}
	assert(gooEngMap.find('q') == gooEngMap.end());
	gooEngMap['q'] = 'z';
	gooEngMap['z'] = 'q';
	f.close();
	//read input
	f.open("a.in");
	if (f.fail())
	{
		cout << "a.in not found" << endl;
		return 1;
	}
	int t;
	f >> t;
	f.getline(cgoo, 100); // just for the remaining of the first line
	for (int i = 1; i <= t; i++)
	{
		f.getline(cgoo, 200);
		goo = string(cgoo);
		cout <<"Case #" << i << ": " << Translate(goo, gooEngMap) << endl;
	}

}