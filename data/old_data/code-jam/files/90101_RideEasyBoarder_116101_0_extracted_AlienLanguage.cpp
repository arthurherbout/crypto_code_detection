#include <fstream>
#include <iostream>
#include <set>

void ReadInPossibleChars(std::ifstream& stream, std::set<char>& outPossibleChars) {
  char curChar;
  stream >> curChar;
  if('(' == curChar) {
    stream >> curChar;
    while(')' != curChar) {
      outPossibleChars.insert(curChar);
      stream >> curChar;
    }
  } else {
    outPossibleChars.insert(curChar);
  }
}

int main(int argc, char* argv[]) {
  typedef std::set<std::string> SetOfWords;

  for(int currentFileNum = 1; currentFileNum < argc; currentFileNum++) {
    std::ifstream fileStream(argv[currentFileNum]);
    size_t numLettersInWords;
    size_t numWordsInDictionary;
    size_t numTestCases;
    fileStream >> numLettersInWords >> numWordsInDictionary >> numTestCases;

    SetOfWords dictionaryWords;
    for(size_t currentDictionaryWordNum = 0; currentDictionaryWordNum < numWordsInDictionary; currentDictionaryWordNum++) {
      std::string currentWord;
      fileStream >> currentWord;
      dictionaryWords.insert(currentWord);
    }

    for(size_t currentTestCaseNum = 1; currentTestCaseNum <= numTestCases; currentTestCaseNum++) {
      SetOfWords possibleWords(dictionaryWords);
      for(size_t currentCharacterNum = 0; currentCharacterNum < numLettersInWords; currentCharacterNum++) {
        std::set<char> possibleChars;
        ReadInPossibleChars(fileStream, possibleChars);
        SetOfWords::iterator possibleWordsIt = possibleWords.begin(), end = possibleWords.end();
        while(possibleWordsIt != end) {
          if(possibleChars.find(possibleWordsIt->at(currentCharacterNum)) == possibleChars.end()) {
            possibleWords.erase(*possibleWordsIt);
            possibleWordsIt = possibleWords.begin();
            end = possibleWords.end();
          } else {
            ++possibleWordsIt;
          }
        }
      }
      std::cout << "Case #" << currentTestCaseNum << ": " << possibleWords.size() << std::endl;
    }
  }
}
