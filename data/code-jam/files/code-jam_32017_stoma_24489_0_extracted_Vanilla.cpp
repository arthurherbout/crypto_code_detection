#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define IO_ERROR_CODE 10
#define SUCCESS_EXIT 0

//#define _DEBUG

using namespace std;

unsigned int ScenariosNum;

std::string int2string(unsigned int i)
{
  std::stringstream w;
  std::string s;
  w << i;
  w >> s;
  return s;
}

unsigned int string2int(std::string s)
{      
      std::stringstream w;
      unsigned int i;
      w << s;
      w >> i;
      return i;
}

unsigned int ReadNumData(std::ifstream& DataFile)
{
  std::string TextLine;
  getline(DataFile,TextLine);
  return string2int(TextLine);
}

void GenerateDeck (std::vector<int>& D, unsigned int Size)
{
     unsigned int CP=0;
     unsigned int NP=0;
     unsigned int CardN, Gap;
     
     for (CardN=0;CardN<=Size;CardN++) D.push_back(0);     

     for (CardN=1;CardN<=Size;CardN++)
     {
         for (Gap=1;Gap<=CardN;Gap++) { 
                                        if (D[NP]==0)
                                            { NP++;
                                              if (NP > Size) NP=1;                                               
                                            }
                                              
                                        if (D[NP]!=0) 
                                          {
                                            while (D[NP]!=0)
                                            {  
                                               NP++;
                                               if (NP > Size) NP=1; 
                                            }
                                          }
                                      }
         D[NP]=CardN;
         CP=NP;
     }

//     for (CardN=0;CardN<=Size;CardN++) cout<<D[CardN]<<" ";   

//     cout << std::endl;
 }

string ParseScenarioFile(std::ifstream& DataFile)
{
  vector<int> Deck;
  unsigned int DeckSize = ReadNumData(DataFile);
  std::string Result="";


  GenerateDeck(Deck,DeckSize);
 
  unsigned int Positions;
  DataFile >> Positions;
  for (unsigned int i=1; i<=Positions; i++) 
  {
    int index;
    DataFile >> index;
//    cout << "Index value read is: " << index << std::endl;
    Result += int2string(Deck[index]);
    Result += " ";
  }

  string TextLine;
  getline(DataFile,TextLine);

  return Result;
}


int main(void)
{
    std::ifstream InputFile;
    std::ofstream OutputFile;
    InputFile.open("data.txt");
    OutputFile.open("results.txt");
    if (!InputFile.good()) return IO_ERROR_CODE;
    if (!OutputFile.good()) return IO_ERROR_CODE;    

    ScenariosNum = ReadNumData(InputFile);

#ifdef _DEBUG    
     std::cout << "Total scenarios: " << ScenariosNum << std::endl;
     std::cout << "---------------------------------------------------" << std::endl;
#endif
     for (int i=0; i<ScenariosNum; i++)
     {
       std::string ScenarioSolution = ParseScenarioFile(InputFile);
       OutputFile << "Case #" << i+1 << ": " << ScenarioSolution << std::endl;
//       system("pause");
     }
     return SUCCESS_EXIT;
}
