#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#define IMAX 1234567890

using namespace std;

int main() {
  int T, Hd, Ad, Hk, Ak, B, D;
  cin>>T;
  for (int t=1;t<=T;t++) {
    cin>>Hd>>Ad>>Hk>>Ak>>B>>D;
    int minattack=(Hk-1)/Ad+1;
    for (int i=1;i<100000 && i*B<Hk;i++) {
      int anattack = i + (Hk-1)/(Ad+i*B)+1;
      if (anattack < minattack) {
	minattack = anattack;
      }
    }
    if (minattack == 1) {
      cout<<"Case #"<<t<<": "<<1<<endl;
      continue;
    }
    if (minattack == 2) {
      if (Hd > Ak) {
	cout<<"Case #"<<t<<": "<<2<<endl;
	continue;
      }
    }
    int cycle = (Hd-1)/Ak + 1;
    if (cycle==1) {
      cout<<"Case #"<<t<<": IMPOSSIBLE"<<endl;
      continue;
    }
    int nodebuff = IMAX;
    if (cycle>=3) {
      nodebuff = (minattack-2)/(cycle-2)-1;
    }
    if (D!=0) {
      int tmpHd=Hd;
      int moves=0;
      for (int i=1;i<100000 && Ak>0;i++) {
	//	cout<<i<<' '<<moves<<endl;
	Ak-=D;
	tmpHd-=Ak;
	moves++;
	if (tmpHd<=Ak) {
	  tmpHd=Hd-Ak+D-Ak;
	  if (tmpHd<=Ak) break;
	  moves++;
	}
	if (Ak<=0) {
	  if (nodebuff > moves) {
	    nodebuff = moves;
	  }
	  break;
	}
	int ncycle = (Hd-1)/Ak + 1;
	int fcycle = (tmpHd -1)/Ak + 1;
	int nheals = IMAX;
	//	cout<<ncycle<<endl;
	if (minattack<=fcycle) {
	  nheals = 0;
	} else {
	  nheals = (minattack-fcycle+2)/(ncycle-2);
	}
	if (nodebuff > moves+nheals) {
	  nodebuff = moves+nheals;
	}
      }
    }
    if (nodebuff < IMAX) {
      cout<<"Case #"<<t<<": "<<nodebuff+minattack<<endl;
    } else {
      cout<<"Case #"<<t<<": IMPOSSIBLE"<<endl;
    }
  }
}

