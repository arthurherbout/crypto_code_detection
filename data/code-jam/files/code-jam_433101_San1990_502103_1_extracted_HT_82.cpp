#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	int T,N,K;
	
	ifstream text_file;
	text_file.open("text_file.txt");
	ofstream text_file_o;
	text_file_o.open("text_file_o.txt");

	text_file>>T;

	for(unsigned int i=0;i<T;i++)
	{
		text_file>>N>>K;
		if ((K+1)%(1<<N)==0)
			text_file_o<<"Case #"<<i+1<<": ON"<<endl;
		else	text_file_o<<"Case #"<<i+1<<": OFF"<<endl;
	}

	text_file.close();
	text_file_o.close();

	return 0;
}