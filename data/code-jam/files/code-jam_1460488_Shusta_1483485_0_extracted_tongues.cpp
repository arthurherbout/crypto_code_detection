#include <iostream>
#include <string>

using namespace std;

char translate(char input) {
	switch (input) {
		case 'e':
			return 'o';
			break;
		case 'j':
			return 'u';
			break;
		case 'p':
			return 'r';
			break;
		case ' ':
			return ' ';
			break;
		case 'm':
			return 'l';
			break;
		case 'y':
			return 'a';
			break;
		case 's':
			return 'n';
			break;
		case 'l':
			return 'g';
			break;
		case 'k':
			return 'i';
			break;
		case 'd':
			return 's';
			break;
		case 'x':
			return 'm';
			break;
		case 'v':
			return 'p';
			break;
		case 'n':
			return 'b';
			break;
		case 'c':
			return 'e';
			break;
		case 'r':
			return 't';
			break;
		case 'i':
			return 'd';
			break;
		case 'b':
			return 'h';
			break;
		case 't':
			return 'w';
			break;
		case 'a':
			return 'y';
			break;
		case 'h':
			return 'x';
			break;
		case 'w':
			return 'f';
			break;
		case 'f':
			return 'c';
			break;
		case 'o':
			return 'k';
			break;
		case 'u':
			return 'j';
			break;
		case 'g':
			return 'v';
			break;
		case 'q':
			return 'z';
			break;
		case 'z':
			return 'q';
			break;
		default:
			break;
	}
}

int main(int argc, char *argv[]) {
	int inputCount;
	cin >> inputCount;
	char eatEnd;
	cin >> eatEnd;
	cin.putback(eatEnd);
	for (int i = 0; i < inputCount; i++) {
		string str;
		getline(cin, str);
		string outputStr;
		
		for (int charIndex = 0; charIndex < str.length(); charIndex++) {
			outputStr.push_back(translate(str[charIndex]));
		}
		
		cout << "Case #" << i + 1 << ": " << outputStr << endl;
		
	}

//	string googleStr = "ejp mysljylc kd kxveddknmc re jsicpdrysi rbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcd de kr kd eoya kw aej tysr re ujdr lkgc jv y qee";
//	string englishStr = "our language is impossible to understand there are twenty six factorial possibilities so it is okay if you want to just give up a zoo";
//	for (int i = 0; i < googleStr.length(); i++) {
//		cout << "case \'" << googleStr[i] << "\':" << endl;
//		cout << "\treturn \'" << englishStr[i] << "\';" << endl;
//		cout << "\tbreak;" << endl;
//		
//	}
	
	
}