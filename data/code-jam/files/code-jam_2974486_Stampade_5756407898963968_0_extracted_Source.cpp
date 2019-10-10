#include<iostream>
#include<fstream>
#include<conio.h>

#include<unordered_map>

using namespace std;

int storecredit(int argc, char** argv);
int printreverselines(int argc, char** argv);
int printT9spellings(int argc, char** argv);

/* 2014 qualification round */
/*                          */
int printmagictrickanswers(int argc, char** argv);

int main( int argc,
		  char** argv )
{
	int result;
//	result = storecredit(argc, argv);
//	result = printreverselines(argc, argv);
//	result = printT9spellings(argc, argv);
	result = printmagictrickanswers(argc, argv);
	_getch();
	return result;
}
