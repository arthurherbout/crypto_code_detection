#include<iostream>
#include<string>
#include<vector>
bool sign=false;
using namespace std;
char val (char c1,char c2);
int main () {
	int t;
	cin >> t;
	
	for(int k=1;k<=t;k++){
		int l,x;
		char now;
		vector<int> v1,v2;
		string s,temp;
		cin >> l >> x >> temp;
		sign=false;
		for(int i=0;i<x;i++){
			s+=temp;
		}
		//now=s[0];
		//cout<<s<<endl;
		int pos;
		bool flg=false;
		for(int i=0;i<s.size();i++){
			if(i!=0)
				now=val(now,s[i]);
			else
				now=s[0];
	
			if(now=='i'){
				pos=i;
				flg=true;
				break;
			}
		}
		//cout<<pos<<endl;
		if(flg==false){
			cout<<"Case #"<<k<<": "<<"NO"<<endl;
			continue;
		}
		pos++;
		flg=false;
		for(int i=pos;i<s.size();i++){
				if(i!=pos)
					now=val(now,s[i]);
				else
					now=s[pos];
	
				if(now=='j'){
					pos=i;
					flg=true;
					break;
				}
		}
		//cout<<pos<<endl;
		if(flg==false){
			cout<<"Case #"<<k<<": "<<"NO"<<endl;
			continue;
		}
		pos++;
		now=s[pos];
		for(int i=pos;i<s.size();i++){
			if(i!=pos)
				now=val(now,s[i]);
			else
				now=s[pos];
		}
		//cout<<now<<endl;
		if(now=='k'&&sign==false){
			cout<<"Case #"<<k<<": "<<"YES"<<endl;
		}else{
			cout<<"Case #"<<k<<": "<<"NO"<<endl;
		}
	}
	
	return 0;
}
char val (char c1,char c2){
	if(c1=='1')
		return c2;
	if(c2=='1')
		return c1;
	if(c1==c2){
		sign=!sign;
		return '1';
	}
	if(c1=='i'&&c2=='j'){
		return 'k';
	}
	if(c2=='i'&&c1=='j'){
		sign=!sign;
		return 'k';
	}
	if(c1=='j'&&c2=='k'){
		return 'i';
	}
	if(c1=='k'&&c2=='j'){
		sign=!sign;
		return 'i';
	}
	if(c1=='k'&&c2=='i'){
		return 'j';
	}
	if(c1=='i'&&c2=='k'){
		sign=!sign;
		return 'j';
	}
}
