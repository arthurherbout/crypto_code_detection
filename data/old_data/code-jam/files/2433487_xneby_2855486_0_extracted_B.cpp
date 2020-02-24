#include <cstdio>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

typedef long long T;

struct Point {
	T x, y;
	Point(): x(0), y(0) {}
	Point(T _x, T _y): x(_x), y(_y) {}

	bool operator < (const Point& other) const {
		if(x == other.x) return y < other.y;
		return x < other.x;
	}
};

T operator * (const Point& A, const Point& B){
	return A.x * B.y - A.y * B.x;
}

Point operator - (const Point& A, const Point& B){
	return Point(A.x-B.x, A.y-B.y);
}

const int N = 1007;

Point p[N];
int xy[N];
bool onhull[N];

bool xycmp(int x, int y){
	if(p[x].x == p[y].x) return p[x].y < p[y].y;
	return p[x].x < p[y].x;
}

template<typename T>
struct Vector : vector<T> {
	Vector& operator << (const T& elem){
		this->push_back(elem);
		return * this;
	}

	T& operator[](int x){
		if(x < 0) x += this->size();
		return vector<T>::operator[] (x);
	}

	void pop_front(){
		this->erase(this->begin());
	}
};

T segment(int a, int b){
	return (p[a].y+p[b].y) * (p[b].x - p[a].x);
}

T area(Vector<int> v){
	T wy = 0;
	for(int i=1; i<v.size(); i++){
		wy += segment(v[i-1], v[i]);
	}
	return wy;
}

void test(int TT){
	int n;
	scanf("%d", &n);
	for(int i=0; i<n; i++){
		long long x, y;
		scanf("%lld %lld", &x, &y);
		p[i] = Point(x, y);

		xy[i] = i;
		onhull[i] = false;
	}

	sort(xy, xy+n, xycmp);

//	for(int i=0; i<n; i++) printf("%d ", xy[i]);
//	puts("");

	Vector<int> hull[2];
	
	for(int z=0; z<2; z++){
		hull[z] << xy[0];
		hull[z] << xy[1];

		for(int i=2; i<n; i++){
			while(hull[z].size()>1 && (p[hull[z][-1]] - p[hull[z][-2]])*( p[xy[i]]- p[hull[z][-1]]) > 0){
				hull[z].pop_back();
			}
			hull[z] << xy[i];
		}

		reverse(xy, xy+n);
	}

	for(int i=0; i<2; i++){
		for(auto it = hull[i].begin(); it != hull[i].end(); it++){
//			printf("%d ", *it);
			onhull[*it] ^=1;
		}
//		puts("");
	}


	int idx = 0;

	Vector<int> xy2;

	for(int i=0; i<n; i++){
		if(!onhull[i]){
		   	xy2 << i;
//			printf("xy2 %d\n", i);
		}
	}

	sort(xy2.begin(), xy2.end(), xycmp);

	Vector<int> ans;
	Vector<int> toadd;

	if(area(hull[0]) - area(xy2) > area(xy2) + area(hull[1]) || (area(hull[0]) - area(xy2) == area(xy2) + area(hull[1]) && hull[1].size() > 2)){
		ans = hull[0];
		toadd = hull[1];
		toadd.insert(toadd.end(), xy2.rbegin()+1, xy2.rend()-1);
	} else {
		ans = hull[1];
		toadd = hull[0];
		toadd.insert(toadd.end(), xy2.begin()+1, xy2.end()-1);
	}
	sort(toadd.begin(), toadd.end(), xycmp);
/*
	printf("ans: ");
	for(auto it = ans.begin(); it != ans.end(); it++){
		printf("%d ", *it);
	}
	puts("");

	printf("toadd: ");
	for(auto it = toadd.begin(); it != toadd.end(); it++){
		printf("%d ", *it);
	}
	puts("");
*/
	if(toadd[0] == ans[0]){
		reverse(toadd.begin(), toadd.end());
	}
	toadd.pop_front(); toadd.pop_back();

	ans.insert(ans.begin(), toadd.begin(), toadd.end());

	printf("Case #%d:", TT);
	for(auto it = ans.begin(); it != ans.end(); it++){
		printf(" %d", *it);
	}
	puts("");

	ans << ans[0];

	assert(abs(area(ans))*2 > (area(hull[0]) + area(hull[1])));

}

int main(){
	int t;
	scanf("%d", &t);
	for(int i=1; i<=t; i++) test(i);
}
