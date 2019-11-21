#include<bits/stdc++.h>
using namespace std;
void rd(int &x){
  int k, m=0;
  x=0;
  for(;;){
    k = getchar_unlocked();
    if(k=='-'){
      m=1;
      break;
    }
    if('0'<=k&&k<='9'){
      x=k-'0';
      break;
    }
  }
  for(;;){
    k = getchar_unlocked();
    if(k<'0'||k>'9'){
      break;
    }
    x=x*10+k-'0';
  }
  if(m){
    x=-x;
  }
}
void wt_L(long long x){
  char f[20];
  int m=0, s=0;
  if(x<0){
    m=1;
    x=-x;
  }
  while(x){
    f[s++]=x%10;
    x/=10;
  }
  if(!s){
    f[s++]=0;
  }
  if(m){
    putchar_unlocked('-');
  }
  while(s--){
    putchar_unlocked(f[s]+'0');
  }
}
void wt_L(const char c[]){
  int i=0;
  for(i=0;c[i]!='\0';i++){
    putchar_unlocked(c[i]);
  }
}
template<class S, class T> inline void chmin(S &a, T b){
  if(a>b){
    a=b;
  }
}
long long attack(long long att, long long hp, long long buff){
  long long r1, r2, res=1000000000000000LL, t1, t2, x, y;
  for(x=0;x<200;x++){
    t1 = x;
    r1 = t1 + (hp + att + buff * t1 - 1) / (att + buff * t1);
    chmin(res, r1);
  }
  return res;
}
long long eval(long long H, long long A, long long D, long long att, long long deb){
  long long h, heal=0, i, j, res=0;
  h = H;
  while(att){
    if(deb){
      if(h > A-D){
        res++;
        deb--;
        A -= D;
        if(A < 0){
          A = 0;
        }
        h -= A;
        heal = 0;
        continue;
      }
    }
    if(att==1){
      res++;
      att--;
      h -= A;
      heal = 0;
      continue;
    }
    if(h <= A){
      res++;
      h = H;
      h -= A;
      if(heal){
        return 1000000000000000LL;
      }
      heal = 1;
      continue;
    }
    res++;
    att--;
    h -= A;
    heal = 0;
  }
  return res;
}
int main(){
  int TEST, test=0;
  rd(TEST);
  while(TEST--){
    int Ad, Ak, B, D, Hd, Hk, i;
    long long Att, res, tmp;
    fprintf(stderr, "r=%d\n", TEST);
    printf("Case #%d: ", ++test);
    rd(Hd);
    rd(Ad);
    rd(Hk);
    rd(Ak);
    rd(B);
    rd(D);
    Att = attack(Ad, Hk, B);
    res = 1000000000000000LL;
    for(i=0;i<200;i++){
      tmp = eval(Hd, Ak, D, Att, i);
      if(res < tmp){
        break;
      }
      res = tmp;
    }
    if(res==1000000000000000LL){
      wt_L("IMPOSSIBLE");
      putchar_unlocked('\n');
    }
    else{
      wt_L(res);
      putchar_unlocked('\n');
    }
  }
  return 0;
}
// cLay varsion 20170413-1 [beta]

// --- original code ---
// ll attack(ll att, ll hp, ll buff){
//   ll x, y, t1, t2, r1, r2;
//   ll res = 1d15;
// 
//   rep(x,200){
//     t1 = x;
//     r1 = t1 + (hp + att + buff * t1 - 1) / (att + buff * t1);
//     res <?= r1;
//   }
// 
//   return res;
// }
// 
// ll eval(ll H, ll A, ll D, ll att, ll deb){
//   ll i, j, h, res = 0, heal = 0;
// 
//   h = H;
//   while(att){
//     if(deb){
//       if(h > A-D){
//         res++;
//         deb--;
//         A -= D; if(A < 0) A = 0;
//         h -= A;
//         heal = 0;
//         continue;
//       }
//     }
//     if(att==1){
//       res++;
//       att--;
//       h -= A;
//       heal = 0;
//       continue;
//     }
//     if(h <= A){
//       res++;
//       h = H;
//       h -= A;
//       if(heal) return 1d15;
//       heal = 1;
//       continue;
//     }
// 
//     res++;
//     att--;
//     h -= A;
//     heal = 0;
//   }
// 
//   return res;
// }
// 
// {
//   int test = 0, TEST;
//   rd(TEST);
//   while(TEST--){
//     fprintf(stderr, "r=%d\n", TEST);
//     printf("Case #%d: ", ++test);
// 
//     int Hd, Ad, Hk, Ak, B, D;
//     int i;
//     ll Att;
//     ll res, tmp;
// 
//     rd(Hd,Ad,Hk,Ak,B,D);
//     Att = attack(Ad, Hk, B);
// 
//     res = 1d15;
//     rep(i,200){
//       tmp = eval(Hd, Ak, D, Att, i);
//       if(res < tmp) break;
//       res = tmp;
//     }
//     
//     if(res==1d15) wt("IMPOSSIBLE"); else wt(res);
//   }
// }
