#include <cstdlib>
#include <cstdio>

#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include <iostream>
#include <memory>
#include <cmath>

using namespace std;

struct Field {
    char type;
    
    Field():type('.'){}
    Field(char c): type(c){}
};

struct Point {
    double x;
    double y;
    
    Point(double aX, double aY): x(aX), y(aY){}
    Point(): x(0.0), y(0.0){}
};

class Map {
public:
    Map(int w, int h): mW(w), mH(h), mData(new Field[w*h]){}
    
    Field& at(int x, int y) { return *(mData.get() + x + y*mW); }
    
    int mW;
    int mH;
    std::auto_ptr<Field> mData;
    
    vector<Point> getBaseReflections() {
        Point me;
        for(int x=1; x<mW-1; ++x) {
            for(int y=1; y<mH-1; ++y) {
                if(at(x,y).type == 'X') {
                    me.x = x-1 + 0.5;
                    me.y = y-1 + 0.5;
                    break;
                }
            }
        }
        
        Point meN ( me.x, -me.y);
        Point meW (-me.x,  me.y);
        Point meNW(-me.x, -me.y);
        
        vector<Point> ret;
        ret.reserve(4);
        ret.push_back(me);
        ret.push_back(meN);
        ret.push_back(meW);
        ret.push_back(meNW);
        
        return ret;
    }
    
    static void translate(vector<Point>& aPoints, const Point& aTranslation) {
        for(vector<Point>::iterator it = aPoints.begin(); it != aPoints.end(); ++it) {
            it->x += aTranslation.x;
            it->y += aTranslation.y;
        }
    }
    
    void prepareExpandedRelflections(const vector<Point>& aBaseReflections, int aD, vector<Point>& ret) {
        int realW = mW - 2;
        int realH = mH - 2;
        
        int iterW = (aD / realW) + 1;
        int iterH = (aD / realH) + 1;
        
        ret.clear();
        ret.reserve(2*(iterH+1)*2*(iterW+1)*4);
        
        for(int i=-iterW; i<= iterW; ++i){
            for(int j=-iterH; j<= iterH; ++j){
                Point t( i*2*realW, j*2*realH );
                vector<Point> tmp(aBaseReflections);
                translate(tmp,t);
                
                ret.insert(ret.end(), tmp.begin(), tmp.end());
            }
        }
        
        // our point is in 0,0
        Point t(-aBaseReflections[0].x, -aBaseReflections[0].y);
        translate(ret,t);
    }
};

int solveSmall(Map& aMap, int d) {
    int ret = 0;
    /*
    clog << d << endl;
    
    for(int i=0;i<aMap.mH; ++i) {
        for(int j=0;j<aMap.mW; ++j)
            clog << aMap.at(j,i).type;
        clog<< endl;
    }
    */
    vector<Point> base = aMap.getBaseReflections();
    vector<Point> pts;
    aMap.prepareExpandedRelflections(base, d, pts);
    
    set<double> was;
    
    for(vector<Point>::iterator it = pts.begin(); it != pts.end(); ++it) {
        if(it->x == 0.0 && it->y == 0.0) continue;
        
        if( sqrt( pow(it->x,2) + pow(it->y,2)) <= d) {
            double tan = atan2(it->y, it->x);
            if(was.find(tan) == was.end()){
                was.insert(tan);
                ++ret;
            }
        }
    }
    
    return ret;
}

int main(){

    int cases;
    cin >> cases;

    for(int i=0; i<cases; ++i) {
        int h, w, d;
        cin >> h >> w >> d;
        string line;
        getline(cin,line);
        
        Map map(w,h);
        
        for(int y=0;y<h;++y) {
            getline(cin,line);
            for(int x=0;x<w;++x) {
                map.at(x,y).type = line[x];
            }
        }
        
        int ret = solveSmall(map, d);
        
        printf("Case #%d: %d\n", 
            (i+1), ret
        );
    }
    return 0;
}
