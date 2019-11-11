
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cmath>
using namespace std;
int gcd(int x,int y){
	x=abs(x);y=abs(y);
	if(x>y)swap(x,y);
	if(y)while(x%=y)swap(x,y);
	return y;
}
int sign(int x){if(x) return x/abs(x); else return 0;}
int round(double x){
	double df=abs(floor(x)-x);
	double dc=abs(ceil(x)-x);
	if(df<dc) return floor(x); else return ceil(x);
}

class Point{public:
	double m_r,m_c;
	double m_dr,m_dc;
	int m_type;
	Point(double r=0,double c=0,double dr=0,double dc=0,int type=0):m_r(r),m_c(c),m_dr(dr*0.25),m_dc(dc*0.25),m_type(type){}
	void operator+=(Point pt){m_r+=pt.m_r;m_c+=pt.m_c;}
	void operator-=(Point pt){m_r-=pt.m_r;m_c-=pt.m_c;}
	double abs2(){return m_r*m_r+m_c*m_c;}
	bool operator<(Point pt){return abs2()>pt.abs2();}	
	bool isame(Point pt){
		pt-=(*this);
		return pt.m_r==0 && pt.m_c==0;
	}
	void MirrorRow(int r){m_r=2*r-m_r;m_dr*=-1;}
	void MirrorCol(int c){m_c=2*c-m_c;m_dc*=-1;}
};
ostream& operator<<(ostream& os,Point pt){return os<<"("<<int(pt.m_r*100)/100.0<<","<<int(pt.m_c*100)/100.0<<")";}
ostream& operator<<(ostream& os,vector<Point> pts){
	for(int k=0;k<pts.size();k++)os<<pts[k];
	return os;
}
class Board:vector<string>{public:
	char operator()(int r,int c){
		if(r<0 || r>=size())return '.';
		string &s=(*this)[r];
		if(c<0 || c>=s.size())return '.';
		return s[c];
	}
	string operator()(int r){return (*this)[r];}
	void push_back(const string& s){vector<string>::push_back(s);}
	int size()const {return vector<string>::size();}
};
bool debug= false ;
int main(){
//	cout<<round(3.4);
//*
	int T;cin>>T;
	for(int t=1;t<=T;t++){
		Point me;
		int H,W,D;cin>>H>>W>>D;//getline(cin,s);
		Board bd;
		for(int k=0;k<H;k++){
			string s,ss;cin>>s;//cout<<"Y"<<s<<endl;
			for(int h=0;h<s.size();h++){
				if(s[h]=='X'){me.m_r=2*k+1;me.m_c=2*h+1;}//s[h]='.';}
				ss.push_back(s[h]);ss.push_back(s[h]);
			}
			bd.push_back(ss);bd.push_back(ss);
		}
		H*=2;W*=2;D*=2;
		int ans=0;
		for(int dr=-D;dr<=D;dr++){if(dr%2)continue;int r=dr;
			for(int dc=-D;dc<=D;dc++){if(dc%2)continue;int c=dc;
				//if(r==0 && c==0)continue;
				int g=gcd(r,c);
				if(g!=2)continue;
				if(r*r+c*c>D*D)continue;
				int b=2;for(;true;b++)if((r*r+c*c)*b*b>D*D)break;b--;
			if(debug)cout<<r<<" "<<c<<" "<<b<<endl;//continue;
				vector<Point> pts;
				int sr=sign(r),sc=sign(c);
				for(int ri=sr;ri!=r;ri+=sr){if(2*ri==r)continue;double f=1.0*ri/r;pts.push_back(Point(ri,f*c,sr,0,1));}
				for(int ci=sc;ci!=c;ci+=sc){if(2*ci==c)continue;double f=1.0*ci/c;pts.push_back(Point(f*r,ci,0,sc,2));}
				pts.push_back(Point(r/2,c/2,sr,sc,abs(sr)*1+abs(sc)*2));
				pts.push_back(Point(r,c,sr,sc,4+abs(sr)*1+abs(sc)*2));
			//if(debug)cout<<endl<<"UNT:"<<pts;
				int sz=pts.size();
				while(b--!=1){
					for(int k=0;k<sz;k++){
						Point pt=pts[k];
						pt+=Point(b*r,b*c);
						pts.push_back(pt);
					}
				}
			//if(debug)cout<<endl<<"BST:"<<pts;
				sort(pts.begin(),pts.end());
			//if(debug)cout<<endl<<"OLD:"<<pts;
				for(int k=0;k<pts.size();k++) pts[k]+=me;
			if(debug)cout<<endl<<pts<<":ALL"<<endl;
				bool mr,mc,mrc;mr=mc=mrc=false;
				while(pts.size()){
			//if(debug)if(mr||mc)cout<<pts<<":BBB"<<endl;
					Point pt=pts.back();pts.pop_back();
			//if(debug)cout<<pts;
			if(debug)cout<<pt;
					if(pt.m_type&4){
						if(pt.isame(me)){ans++;
			if(debug)cout<<"(<span style=background:blue>Ans</span>)"<<endl;
							break;
						}
					}
					mr=mc=mrc=false;
					int pr,pc;
			if(debug)cout<<pt.m_type<<" dr"<<pt.m_dr<<" dc"<<pt.m_dc<<" ";
					if(pt.m_type==1){			pr=floor(pt.m_r+pt.m_dr);	pc=floor(pt.m_c);			mr=(bd(pr,pc)=='#');}
					else if(pt.m_type==2){		pr=floor(pt.m_r);			pc=floor(pt.m_c+pt.m_dc);	mc=(bd(pr,pc)=='#');}
					else if(pt.m_type&3==3){	
						pr=floor(pt.m_r+pt.m_dr);	pc=floor(pt.m_c-pt.m_dc);	mr=(bd(pr,pc)=='#');
			if(debug)cout<<pr<<" "<<pc<<" "<<bd(pr,pc)<<"X";						
						pr=floor(pt.m_r-pt.m_dr);	pc=floor(pt.m_c+pt.m_dc);	mc=(bd(pr,pc)=='#');
			if(debug)cout<<pr<<" "<<pc<<" "<<bd(pr,pc)<<"X";
						pr=floor(pt.m_r+pt.m_dr);	pc=floor(pt.m_c+pt.m_dc);	mrc=(bd(pr,pc)=='#');
			if(debug)cout<<pr<<" "<<pc<<" "<<bd(pr,pc)<<"X";
			if(debug)cout<<" <span style=background:black>"<<mr<<mc<<mrc<<"</span> ";
						if(mrc==false){mr=mc=false;}
						if(mrc && mr==false && mc==false)break;
			if(debug)cout<<" <span style=background:black>"<<mr<<mc<<mrc<<"</span> ";
					}
			//if(debug)cout<<mr<<mc<<mrc;
			if(debug)cout<<endl;
					
					if(mr){	for(int k=0;k<pts.size();k++) pts[k].MirrorRow(round(pt.m_r));}
					if(mc){	for(int k=0;k<pts.size();k++) pts[k].MirrorCol(round(pt.m_c));}
				}
			}
		}
/*		
		return 0;
		//for(int k=0;k<H;k++)cout<<m[k]<<endl;
		for(int dr=0;dr<=D;dr++){int r=dr;
			for(int dc=0;dc<=D;dc++){int c=dc;
				int g=gcd(r,c);
				if(g!=1)continue;
				if(r*r+c*c>D*D)continue;
//				pts.push_back(Point(r,c));
				int b=2;for(;true;b++)if((r*r+c*c)*b*b>D*D)break;b--;
				//r*=b;c*=b;
				int ri,ci;
				for(int sr=-1;sr<=1;sr++){
					if(r && sr==0)continue;if(r==0 && sr)continue;
					for(int sc=-1;sc<=1;sc++){
						if(c && sc==0)continue;if(c==0 && sc)continue;

						vector<Point> pts,bks;
						for(ri=ci=0;ri<r && ci<c;){
							if(ri==0 && ci==0){
								if(r>c) ri++; else if(c<r) ci++; else {ri++;ci++;}
								pts.push_back(Point(r,c));
								bks.push_back(Point(0,0));
								continue;
							}else{
							
							
							}
						}
						
						



						//vector<Point> pts;
						for(ri=1;ri!=r;ri++){double f=1.0*ri/r;pts.push_back(Point(ri,f*c));}
						for(ci=1;ci!=c;ci++){double f=1.0*ci/c;pts.push_back(Point(f*r,ci));}
						pts.push_back(Point(r,c));
						
						while(b--!=1){
							for(int k=0;k<r+c-1;k++){
								pts.push_back(pts[k]+Point(b*r,b*c));
							}
						}
						sort(pts.begin(),pts.end());
						//vector<Point> bks(pts.begin(),pts.end());
						for(int k=0;k<pts.size();k++){
							//bks.push_back(pts[k].m_r+1
								
						}
						
						for(ri=ci=0;ri!=r && ci!=c;){
							if(ri==0 && ci==0){
								if(r>c) ri++; 
								else if(c<r) ci++;
								else {ri++;ci++;}
								continue;
							}
//							double fr=
							
							int rf=1.0*ri*c;// /r
							int cf=1.0*ci*r;// /c
							int tr=(ri+1)/2*2;
							int tc=(ci+1)/2*2;
							bool mirror=true;//bd[(mer+sr*tr)/2][(mec+sc*tc)/2]=='#';
							if(mirror){
								
							
							
							}else{
								if(rf<cf) ri++;
								else if(cf<rf) ci++;
								else{
									ri++;ci++;
								}
							}
						}
					}
				}
		


				
				
				
				r+=mer;c+=mec;
				
				//cout<<s_d++<<" "<<r<<" "<<c<<" "<<endl;
				
				
				
			}
		}
		
		
		int ans=0;
		cout<<"Case #"<<t<<": "<<ans<<endl;
//*/	
		cout<<"Case #"<<t<<": "<<ans<<endl;
	}
	return 0;
}

