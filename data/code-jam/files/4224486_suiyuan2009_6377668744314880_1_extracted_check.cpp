#include<cstdio>
#include<algorithm>
#include<string>
#include<cmath>
#include<cstring>
#include<vector>
#include<queue>
using namespace std;
const int maxn = 100;
const double eps=1e-3;
typedef long long db;

int n;
int sgn(double a) {
    return a<-eps? -1 : a>eps;    //返回double型的符号
}
struct Point2D {
    db x, y;
    int id;
    Point2D(db _x=0, db _y=0):x(_x), y(_y) {}
    void input() {
        scanf("%lld%lld" , &x, &y);
    }

    db len2() {
        return x*x + y*y;
    }
    double len() {
        return sqrt(len2());
    }

    Point2D rotLeft90() {
        return Point2D(-y, x);    //逆时针转90度
    }
    Point2D rotRight90() {
        return Point2D(y, -x);    //顺时针转90度
    }

    Point2D rot(double arc_u) {//绕原点逆时针旋转arc_u
        return Point2D( x*cos(arc_u)-y*sin(arc_u),
                        x*sin(arc_u)+y*cos(arc_u) );
    }
    Point2D rotByPoint(Point2D &center, db arc_u) {
        Point2D tmp( x-center.x, y-center.y );
        Point2D ans = tmp.rot(arc_u);
        ans = ans + center;
        return ans;
    }

    bool operator == (const Point2D &t) const {
        return sgn(x-t.x) == 0 && sgn(y-t.y) == 0;
    }
    bool operator < (const Point2D &t) const {
        if( sgn(x-t.x)==0 ) return y < t.y;
        else return x < t.x;
    }
    Point2D operator + (const Point2D &t) const {
        return Point2D(x+t.x, y+t.y);
    }

    Point2D operator - (const Point2D &t) const {
        return Point2D(x-t.x, y-t.y);
    }

    Point2D operator * (const db &t)const {
        return Point2D( t*x, t*y );
    }

    Point2D operator / (const db &t) const {
        return Point2D( x/t, y/t );
    }

    db operator ^ (const Point2D &t) const { //dot product
        return x*t.x + y*t.y;
    }
    db operator * (const Point2D &t) const { //cross product
        return x*t.y - y*t.x;
    }
    Point2D normalize() {
        return Point2D(x/len(), y/len());
    }
};



Point2D pbase; //The most bottom then most left point

bool polarSortCmp(const Point2D &a , const Point2D &b) {
    return ( (a-pbase)*(b-pbase)>eps ) ||
           ( fabs((a-pbase)*(b-pbase))<eps &&
             (a-pbase).len2()<(b-pbase).len2()-eps);
}

int Graham(Point2D co[] , int &cn , Point2D ps[] , int n) {
    //co中保存凸包的点，cn为凸包点数，ps为原来点集，n为原来点数
    int i, u, p;
    for(u = 0, i = 1; i < n; ++i) { //寻找左下点
        if((ps[i].y < ps[u].y + eps && ps[i].x < ps[u].x - eps)
                ||ps[i].y < ps[u].y - eps ) {
            u = i;
        }
    }
    swap(ps[u], ps[0]);
    pbase = ps[0]; //记录左下点
    sort(ps + 1, ps + n, polarSortCmp); //极角排序
    co[0] = ps[0];
    co[1] = ps[1];
    p = 2; //记录凸包中点数
    for(i = 2; i < n; ++i) {
        //保留了除凸包上最后两点之间的点以外的所有凸包上的点
        while((co[p-1]-co[p-2])*(ps[i]-co[p-2])<-eps) { //改为ep不保留多余点
            --p;
            if(p == 1) break;
        }
        co[p++] = ps[i];
    }
    cn = p;
    return p; //返回凸包点数
}

bool judge(Point2D a,Point2D l,Point2D r) {
    if(l.x>r.x||(l.x==r.x&&l.y>r.y))
        swap(l,r);
    if(a.x<l.x||a.x>r.x)return 0;

    //printf("** %d %d %d %d %d %d\n",a.x,a.y,l.x,l.y,r.x,r.y);

    db tt=(r.y-l.y)*(db)(a.x-l.x);
    db tt2=(a.y-l.y)*(db)(r.x-l.x);
    return tt==tt2;
}

Point2D dzm[maxn],co[maxn],ps[maxn];

int ans[maxn];
int main() {
    //freopen("in.cpp", "r", stdin);
    freopen("C-small-attempt2.in","r",stdin);
    freopen("out", "w", stdout);
    int ncase=0;
    int T;
    scanf("%d",&T);
    while(T--) {
        scanf("%d",&n);
        for(int i=0; i<n; i++)scanf("%lld%lld",&dzm[i].x,&dzm[i].y);
        for(int i=0; i<n; i++)ans[i]=max(0,n-3);
        for(int i=0; i<(1<<n); i++) {
            int cnt=0;
            for(int j=0; j<n; j++)
                if(i&(1<<j)) {
                    ps[cnt]=dzm[j];
                    cnt++;
                }
            if(cnt<4)continue;
            int m;
            Graham(co,m,ps,cnt);
            co[m]=co[0];
            for(int j=0; j<n; j++)
                for(int k=0; k<m; k++)
                    if(judge(dzm[j],co[k],co[k+1])) {
                        ans[j]=min(ans[j],n-cnt);
                        //if(cnt==n&&j==3) printf("%d:%d %d\n",j,dzm[j].x,dzm[j].y);
                    }
        }
        printf("Case #%d:\n",++ncase);
        for(int i=0; i<n; i++)printf("%d\n",ans[i]);
        if(ncase==14)break;
    }
    return 0;
}
