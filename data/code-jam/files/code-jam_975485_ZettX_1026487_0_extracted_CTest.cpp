// CTest.cpp : main project file.
#include "stdafx.h"

using namespace System;

public ref class item {
public:
	item(System::String^ s, bool mode) {
		this->A = s[0];
		this->B = s[1];
		if (mode) {
			this->R = s[2];
			this->D = false;
		}else{
			this->R = ' ';
			this->D = true;
		}
	}
	System::String^ apply(System::String^ res) {
		if (!this->D) {
			int length = res->Length - 1;
			if ((res[length] == this->A && res[length-1] == this->B) || (res[length] == this->B && res[length-1] == this->A)) {
				res = res->Remove(length-1);
				res += this->R;
				//res->Insert(length-1, this->R.ToString());
			} 
		} else {
			int c = res->IndexOf(this->A);
			if (c>=0) {
				int d = res->IndexOf(this->B);
				if (d>=0 && d!=c) {
					res="";
					//if (d<c) res = res->Remove(d, c-d+1);
					//else res = res->Remove(c, d-c+1);
				}
			}
		}
		return res;
	}
	wchar_t A;
	wchar_t B;
	wchar_t R;
	bool D;
};

System::String^ add(System::String^ res, System::Collections::Generic::List<item^> ^rules) {
	if (res->Length < 2) return res;
	for each (item^ rule in rules) res = rule->apply(res);
	return res;
}

System::String^ solve(System::String^ s) {
	array<String^> ^st;
	st = s->Split(' ');
	int cur=0, count=0;
	//array<item^> ^rules = gcnew cli::array<item^>();
	System::Collections::Generic::List<item^> ^rules = gcnew System::Collections::Generic::List<item^>();
	//[x] aaa aaa y ss ss asdfgh
	int i=0;
	count = Int32::Parse(st[i]);
	//x [aaa] aaa y ss ss z asdfgh
	if (count>i) for (i=i+1; i<=count; i++) rules->Add(gcnew item(st[i], true));
	else i++;
	//x aaa aaa [y] ss ss z asdfgh
	count = Int32::Parse(st[i])+i;
	//i++;
	//x aaa aaa y [ss] ss z asdfgh
	if (count>i) for (i=i+1; i<=count; i++) rules->Add(gcnew item(st[i], false));
	else i++;
	//i++;
	//x aaa aaa y ss ss [z] asdfgh
	count = Int32::Parse(st[i]);
	i++;
	//x aaa aaa y ss ss z [asdfgh]
	System::String^ sss = st[i]; //sequence
	System::String ^res = "";
	for (i=0; i<count; i++) {
		res = add(res + sss[i], rules);
	}
	if (res == "") return "[]";
	System::String^ result = "[";
	for (i=0; i<res->Length-1; i++) {
		result += res[i].ToString() + ", ";
	}
	result += res[res->Length-1] + "]";
	return result;
}

//0 aaa 0 aa 0 aasasas
int main(array<System::String ^> ^args) {
    //load file
	System::IO::StreamReader^ file = gcnew System::IO::StreamReader("input.txt");
	int cases_count = Int32::Parse(file->ReadLine());
	System::String ^tmp = "";
	System::String ^res = "";
	for (int i=0; i<cases_count; i++) {
		tmp = file->ReadLine()->Trim();
		res +=	"Case #" + (i+1) + ": " + solve(tmp) + "\r\n";
	}
	file->Close();
	System::IO::StreamWriter^ fileW = gcnew System::IO::StreamWriter("output.txt");
	fileW->Write(res);
	fileW->Close();
	Console::Write(res);
	Console::ReadKey();
    return 0;
}
