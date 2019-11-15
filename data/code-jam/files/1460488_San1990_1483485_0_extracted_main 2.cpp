#include <string>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>

#define F(i,b) for(int i=0;i<(b);i++)
#define FIAN(i,a,n) for(int i=a;i<n;i++)

using namespace std;

//FILE * inputFile = fopen("D:\\Downloads\\Chrome\\1.in", "r");
//FILE * outputFile  = fopen("D:\\Downloads\\Chrome\\output.txt", "w");
ifstream inputFile;
ofstream outputFile;


string inStr;
string fillArray()
{
	F(i,inStr.length()){
		switch (inStr[i])
		{
		case 'f': inStr[i] = 'c'; break;
		case 'g': inStr[i] = 'v'; break;
		case ' ': inStr[i] = ' '; break;
		case 'd': inStr[i] = 's'; break;
		case 'e': inStr[i] = 'o'; break;
		case 'b': inStr[i] = 'h'; break;
		case 'c': inStr[i] = 'e'; break;
		case 'a': inStr[i] = 'y'; break;
		case 'n': inStr[i] = 'b'; break;
		case 'o': inStr[i] = 'k'; break;
		case 'l': inStr[i] = 'g'; break;
		case 'm': inStr[i] = 'l'; break;
		case 'j': inStr[i] = 'u'; break;
		case 'k': inStr[i] = 'i'; break;
		case 'h': inStr[i] = 'x'; break;
		case 'i': inStr[i] = 'd'; break;
		case 'w': inStr[i] = 'f'; break;
		case 'v': inStr[i] = 'p'; break;
		case 'u': inStr[i] = 'j'; break;
		case 't': inStr[i] = 'w'; break;
		case 's': inStr[i] = 'n'; break;
		case 'r': inStr[i] = 't'; break;
		case 'q': inStr[i] = 'z'; break;
		case 'p': inStr[i] = 'r'; break;
		case 'z': inStr[i] = 'q'; break;
		case 'y': inStr[i] = 'a'; break;
		case 'x': inStr[i] = 'm'; break;
		}
	}
	
    return inStr;
}

void oneStep()
{
    //fscanf(inputFile, "%d %d", &A, &B);
    //fprintf(outputFile, "%d\n", fillArray());
	getline(inputFile, inStr);
	outputFile << fillArray() << "\n";
}

int main()
{
    time_t timer = time(NULL);
    printf("%s\n", ctime(&timer));

    unsigned int T;
	inputFile.open("D:\\Downloads\\Chrome\\A-small-attempt0.in");
	outputFile.open("D:\\Downloads\\Chrome\\output.txt");
	inputFile >> T;
	getline(inputFile, inStr);
    //fscanf(inputFile, "%d", &T);

    F(t,T){
        //TODO: solve here!
        //fprintf(outputFile, "Case #%d: ", t+1);
		outputFile << "Case #" << t << ": ";
        oneStep();
        timer = time(NULL);
        printf("step %d - %s\n", t, ctime(&timer));
    }

    //fclose(inputFile);
    //fclose(outputFile);

    timer = time(NULL);
    printf("%s\n", ctime(&timer));
   // qDebug("Time elapsed: %d ms", time1.elapsed());
    return 0;
}