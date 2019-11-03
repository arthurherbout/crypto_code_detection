#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#define FOR(it, loop) for(auto (it) = (loop).begin();(it) != (loop).end();(it)++)
#define FORP(it, loop) for(auto (it) = (loop)->begin();(it) != (loop)->end();it++)
using namespace std;
struct node
{
	char data;
	int num;
	struct node* prev;
	struct node* next;
};
long long getNumFromText(string text){
	long long result = 0;
	for(auto it = text.begin() ; it != text.end() ; it++){
		if(*it > '9' || *it < '0') throw;
		result *= 10;
		result += *it - '0';
	}
	return result;
}
string getStrFromNum(long long num){
	if(num == 0) return "0";
	string result = "";
	while(num > 0){
		result = (char)((num % 10) + '0') + result;
		num /= 10;
	}
	return result;
}
void split(string text, char delimiter, vector<string>* container){
	string part = "";
	for(auto it = text.begin();it != text.end();it++){
		if(*it == delimiter){
			container->push_back(part);
			part = "";
			continue;
		}
		part += *it;
	}
	container -> push_back(part);
}
void splitToNum(string text, char delimiter, vector<long long>* container){
	vector<string> temp;
	split(text, delimiter, &temp);
	FOR(it, temp){
		container->push_back(getNumFromText(*it));
	}
}
void mainRoutine(string filepath, int unitLine, string (*func)(vector<string> *)){
	ifstream in;
	ofstream out;
	in.open(filepath);
	out.open("result");
	if(in.is_open()){
		string line;
		getline(in, line);
		int testCount = 1;
		vector<string> strData;
		while(getline(in, line)){
			strData.push_back(line);
			if(strData.size() == unitLine){
				out << "Case #" << testCount++ << ": " << (*func)(&strData) << '\n';
				strData.clear();
			}
		}
		in.close();
		out.close();
	}
}
bool is_finished(vector<long long>* data)
{
	for(int i = 1 ; i<=6;i++)
	{
		if((*data)[i] != 0) return false;
	}
	return true;
}
int getMax(vector<long long>* data)
{
	int max = 0;
	int result = 0;

	for(int i = 1; i<6;i++)
	{
		if((*data)[i] > max)
		{
			max = (*data)[i];
			result = i;
		}
	}
	return result;
}
int getMax2(vector<long long>* data, int prev)
{
	if(prev % 2 == 0)
	{
		return (*data)[(prev+3)%6] != 0 ? (prev+3)%6 : 0;
	}
	int max = 0;
	int result = 0;
	for(int i = 1; i<=6;i++)
	{
		if(i == prev) continue;
		else
		{
			if(i == (prev+1)%6 || i % 6 == prev - 1) continue;
		}
		if((*data)[i] > max)
		{
			max = (*data)[i];
			result = i;
		}
	}
	return result;
}
char getData(int i)
{
	char x[] = {'R','O','Y','G','B','V'};
	return x[i-1];
}

string process_small(vector<string>* input)
{
	vector<long long> data = vector<long long>();
	splitToNum((*input)[0], ' ', &data);
	//R O Y G B V
	if(data[1] + data[3] < data[5] || data[1]+data[5] < data[3] || data[3]+data[5] < data[1]) return "IMPOSSIBLE";
	string result = "";

	struct node* prev = (struct node *)malloc(sizeof(struct node));
	prev->data = ' ';
	struct node* init = prev;
	bool check = false;
	while(!is_finished(&data))
	{
		struct node* n = (struct node *)malloc(sizeof(struct node));
		n->prev = prev;
		prev->next = n;
		if(prev->data == ' ') //init
		{
			int next = getMax(&data);
			data[next]--;
			n->data = getData(next);
			n->num = next;
			prev = n;
			init = init->next;

			continue;
		}

		int next = getMax2(&data, prev->num);
		if(data[next] == 1 &&!check)
		{
			if(next != init->num && prev->num != init->num && data[init->num] != 0)
			{
				cout << next << ", "<<init->num<<endl;
				next = init->num;
				check = true;
			}
		}

		if(next == 0) 
		{
			cout <<"error"<<endl;
			return "";
		}
		data[next]--;
		n->data = getData(next);
		n->num = next;
		prev = n;
	}
	
	for(int i = 0;i<data[0];i++)
	{
		result += init->data;
		init = init->next;
	}
	return result;
}


int main(void)
{
	mainRoutine("B-small-attempt2.in", 1, process_small);
	//mainRoutine("1", 1, process_small);

	return 0;
}