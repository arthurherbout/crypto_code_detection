#include <bits/stdc++.h>

#ifndef LOCAL
#define cerr dolor_sit_amet
#endif

#define mp make_pair
#define sz(x) ((int)((x).size()))
#define X first
#define Y second

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef pair < int , int > ipair;
typedef pair < ll , ll > lpair;
const int IINF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;
const double DINF = numeric_limits<double>::infinity();
const int MOD = 1000000007;
const ld EPS = 1e-10;
const int DX[] = { 1,  0, -1,  0,  1, -1,  1, -1};
const int DY[] = { 0,  1,  0, -1,  1, -1, -1,  1};
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll sqr(ll x) { return x*x; } ll sqr(int x) { return (ll)x*x; }
double sqr(double x) { return x*x; } ld sqr(ld x) { return x*x; }
mt19937 mmtw(960172);
ll rnd(ll x, ll y) { static uniform_int_distribution<ll> d; return d(mmtw) % (y - x + 1) + x; }

// ========================================================================= //

const int N = 55;

struct Vec3
{
	ld x, y, z;
	
	Vec3() {}
	Vec3(ld nx, ld ny, ld nz) : x(nx), y(ny), z(nz) {}

	Vec3 operator + (const Vec3 & b) const { return { x+b.x, y+b.y, z+b.z }; }
	Vec3 operator - (const Vec3 & b) const { return { x-b.x, y-b.y, z-b.z }; }
	Vec3 operator - () const { return { -x, -y, -z }; }
	Vec3 operator * (ld b) const { return { x*b, y*b, z*b }; }
	Vec3 operator / (ld b) const { return { x/b, y/b, z/b }; }

	Vec3 & operator += (const Vec3 & b) { x+=b.x; y+=b.y; z+=b.z; return *this; }
	Vec3 & operator -= (const Vec3 & b) { x-=b.x; y-=b.y; z-=b.z; return *this; }
	Vec3 & operator *= (ld b) { x*=b; y*=b; z*=b; return *this; }
	Vec3 & operator /= (ld b) { x/=b; y/=b; z/=b; return *this; }
};

Vec3 operator * (ld b, const Vec3 & a) { return { b*a.x, b*a.y, b*a.z }; }
ld dot(const Vec3 & a, const Vec3 & b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
ld length2(const Vec3 & a) { return a.x*a.x + a.y*a.y + a.z*a.z; }
ld length(const Vec3 & a) { return sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }
Vec3 normalize(const Vec3 & a) { return a / sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }
Vec3 cross(const Vec3 & a, const Vec3 & b) {
    return {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

int n;
Vec3 p[N];
Vec3 origin = {0.0, 0.0, 0.0};

int ssign(ld x) {
    if (x > EPS)
        return 1;
    if (x < -EPS)
        return -1;
    return 0;
}

bool sameSide(Vec3 const& p1, Vec3 const& p2, Vec3 const& p3, Vec3 const& x1, Vec3 const& x2) {
    Vec3 n = cross(p2-p1, p3-p1);
    Vec3 v1 = x1 - p1;
    Vec3 v2 = x2 - p1;
    return ssign(dot(n, v1)) * ssign(dot(n, v2)) == 1;
}

bool oinside4(Vec3 const& p1, Vec3 const& p2, Vec3 const& p3, Vec3 const& p4) {
    if (!sameSide(p1, p2, p3, p4, origin)) return 0;
    if (!sameSide(p1, p2, p4, p3, origin)) return 0;
    if (!sameSide(p1, p4, p3, p2, origin)) return 0;
    if (!sameSide(p4, p2, p3, p1, origin)) return 0;
    return true;
}

ld getArea(Vec3 const& p1, Vec3 const& p2, Vec3 const& p3) {
    return length(cross(p2-p1, p3-p1));
}

bool oinside3(Vec3 const& p1, Vec3 const& p2, Vec3 const& p3) {
    if (length2(p1-p2) < EPS)
        return 0;
    if (length2(p1-p3) < EPS)
        return 0;
    if (length2(p2-p3) < EPS)
        return 0;
    return abs(getArea(p1,p2,p3) - (getArea(origin,p2,p3) + getArea(p1,origin,p3) + getArea(p1,p2,origin))) < EPS;
}

void solve() {
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> p[i].x >> p[i].y >> p[i].z;
        p[i] = normalize(p[i]);
    }

    for (int i1 = 0; i1 < n; ++i1)
        for (int i2 = i1+1; i2 < n; ++i2)
            for (int i3 = i2+1; i3 < n; ++i3) {
                for (int i4 = i3+1; i4 < n; ++i4) {
                    if (oinside4(p[i1], p[i2], p[i3], p[i4])) {
                        cout << "YES\n";
                        return;
                    }
                }
                if (oinside3(p[i1], p[i2], p[i3])) {
                    cout << "YES\n";
                    return;
                }
            }
    cout << "NO\n";
}

int main() {
    ios::sync_with_stdio(false);
    cout.precision(20);

    int t;
    cin >> t;
    for (int i = 1; i <= t; ++i) {
        cout << "Case #" << i << ": ";
        solve();
        cerr << "                                                                                                                                      Case " << i << "/" << t << " done\n";
    }

    return 0;
}
