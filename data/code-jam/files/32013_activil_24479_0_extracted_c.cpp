#include <stdio.h>
#include <cmath>

#define PI 3.1415926535897932384626433832795

#define MIN(x, y) ((x)<(y) ? (x) : (y))
#define MAX(x, y) ((x)>(y) ? (x) : (y))
#define ABS(x) ((x)<0 ? -(x) : (x))
#define FOR(i,f,t) for(int i=f; i<t; ++i)
#define FOR_(i,f,t) for(int i=f; i>t; --i)
#define REP(i,n) for(int i=0; i<n; ++i)
#define SWAP(i,j) {i^=j; j^=i; i^=j;}

double f, R, t, r, g;
double rad;

#define S(x) ((x)*(x))

double seg_area(double a, double b){
    double aca = acos(a/rad);
    double acb = acos(b/rad);
    double res = aca-acb;
    res += (sin(2.0*acb)-sin(2.0*aca))/2.0;
    res *= rad*rad;
    res /= 2.0;
    return res;
}

double get_area(double xa, double xb, double ya, double yb, int& d){
    if(xa*xa+ya*ya>=rad*rad){
        d = -1;
        return 0.0;
    }
    if(xb*xb+yb*yb<=rad*rad){
        d = -2;
        return (xb-xa)*(yb-ya);
    }

    double area=0.0;
    double xc2=S(rad)-S(yb);
    double xd2=S(rad)-S(ya);
    // yb>=ya => xd2>=xc2
    double xc, xd;
    double yc2=S(rad)-S(xb);

    if(xc2>=S(xa) && xc2<=S(xb)){
        xc=sqrt(xc2);
        if(yc2>=S(ya) && yc2<=S(yb)){
            xd=xb;
            d=0;
        }else{
            xd=sqrt(xd2);
            d=1;
        }
        area = seg_area(xc, xd)-ya*(xd-xc);
        area += (yb-ya)*(xc-xa);
    }else{
        xc=xa;
        if(yc2>=S(ya)&&yc2<=S(yb)){
            xd=xb;
            d=0;
        }else{
            xd=sqrt(xd2);
            d=1;
        }
        area = seg_area(xc, xd)-ya*(xd-xc);
    }
    return area;
}

double solve(){ //float f, float R, float t, float r, float g){
    rad=R-t-f;
    int x, y, fullsquares = 0;
    double area = 0.0;
    double square0area = (g-2*f)*(g-2*f);
    double squareside = g+2*r;
    x=0; y=(int)(rad/squareside);
    double xa, xb, ya, yb;
    int prevd=1, d=0;
    for (; y>=0; ){
        d=-1;
        xa=x*squareside+f+r;
        xb=(x+1)*squareside-f-r;
        ya=y*squareside+f+r;
        yb=(y+1)*squareside-f-r;
        double ar = get_area(xa, xb, ya, yb, d);
        area += ar;
        if (d == -2){
            fullsquares += y;
            x++;
        } else if (d == -1){
            y--;
        } else if (d == 0){
            fullsquares += y;
            x++;
        } else { //d == 1
            y--;
        }
    }
    area += fullsquares*square0area;
    return 1.0-(area*4.0)/(PI*R*R);
}

int main(){
    int k;
    FILE* fi=fopen("test.txt", "r");
    FILE* fo=fopen("test_out.txt", "w+");
    fscanf(fi, "%d", &k);
    REP(i,k){
        fscanf(fi, "%lf %lf %lf %lf %lf", &f,&R,&t,&r,&g);
        fprintf(fo, "Case #%d: %.6lf\n", i+1, solve());
    }
    return 0;
}
