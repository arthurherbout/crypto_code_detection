#include <stdlib.h>
#include <fstream>

using namespace std;

int main(void){
  ifstream infile("A.in");
  ofstream outfile("A.out");    

  int T;
  infile >> T;
  
  //char line[200];
  string line;
  
  char mapping[26];
  for (int i = 0; i < 26; i++) mapping[i] = '*';


  // looking at the mapping, we see only z-->q is missing, since 1-1 and onto,
  //   so I just hard code that back to itself
  string keyStringA = "z ejp mysljylc kd kxveddknmc re jsicpdrysi rbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcd de kr kd eoya kw aej tysr re ujdr lkgc jv y qee\n";

  string keyStringB = "q our language is impossible to understand there are twenty six factorial possibilities so it is okay if you want to just give up a zoo\n";
  
  // make the mapping with the keys
  int i = 0;
  while (keyStringA[i] != '\n')
    {
      if (keyStringA[i] != ' '){
	int index = (int)keyStringA[i]-'a';
	mapping[index] = keyStringB[i];
      }

      i++;
    }

  // for (int i = 0; i < 26; i++) outfile << mapping[i] << " ";

  // shoot it back out for the cases
  getline(infile, line);
  for (int j = 1; j <= T; j++)
    {
      //      infile.getline(line, 200);
      getline(infile, line);
      for (int i = 0; i < line.length(); i++){
	if (line[i] != ' ' && line[i] != '\n'){
	  int index = (int)line[i] - 'a';
	  line[i] = mapping[index];
	}
      }
      
      outfile << "Case #" << j << ": " << line << endl;
    }
  
  
  outfile.close();  

  return 0;
}
