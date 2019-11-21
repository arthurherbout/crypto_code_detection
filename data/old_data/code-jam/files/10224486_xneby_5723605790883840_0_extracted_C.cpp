#include <bits/stdc++.h>

using namespace std;
using namespace rel_ops;

using Board = vector<string>;

Board make_board(int n, int m) { return Board(n, string(m, '.')); }

enum class Direction { UP, RIGHT, DOWN, LEFT };
struct Point {
	int x, y;
	Point() {}
	Point(int _x, int _y): x(_x), y(_y) {}

	bool in_board(int n, int m) const {
		return 0 <= x && x < n && 0 <= y && y < m;
	}

	char& b(Board & B) const {
		return B[x][y];
	}
	char b(Board const& B) const {
		return B[x][y];
	}

	void go(Direction D) {
		switch(D) {
			case Direction::UP: x--; break;
			case Direction::DOWN: x++; break;
			case Direction::LEFT: y--; break;
			case Direction::RIGHT: y++; break;
		}
	}

	friend ostream& operator << (ostream & out, Point const& P) {
		return out << "(" << P.x << ", " << P.y << ")";
	}

	bool operator == (Point const& other) const {
		return x == other.x && y == other.y;
	}
};


pair<Point, Direction> border(int n, int m, int D) {
	if(D <= m) return { {0, D-1}, Direction::DOWN };
	if(D <= m + n) return { {D - m - 1, m-1}, Direction::LEFT };
	if(D <= 2*m + n) return { { n-1, 2*m+n - D}, Direction::UP };
	return { {2*(n+m)-D, 0}, Direction::RIGHT };
}

pair<Direction, char> decide(Direction D) {
	switch(D) {
		case Direction::UP: return {Direction::LEFT, '\\'};
		case Direction::RIGHT: return {Direction::UP, '/'};
		case Direction::DOWN: return {Direction::RIGHT, '\\'};
		case Direction::LEFT: return {Direction::DOWN, '/'};
	}
}

Direction reflect(Direction D, char C) {
	if(C == '/') {
		switch(D) {
			case Direction::UP: return Direction::RIGHT;
			case Direction::RIGHT: return Direction::UP;
			case Direction::LEFT: return Direction::DOWN;
			case Direction::DOWN: return Direction::LEFT;
		}
	} else { // '\'
		switch(D) {
			case Direction::UP: return Direction::LEFT;
			case Direction::RIGHT: return Direction::DOWN;
			case Direction::LEFT: return Direction::UP;
			case Direction::DOWN: return Direction::RIGHT;
		}

	}
}

Direction opposite(Direction D) {
	switch(D) {
		case Direction::UP: return Direction::DOWN;
		case Direction::DOWN: return Direction::UP;
		case Direction::RIGHT: return Direction::LEFT;
		case Direction::LEFT: return Direction::RIGHT;
	}
}
bool go(int n, int m, Board & B, int F, int T) {
	Point Pf, Tf;
	Direction D;
//	cout << "Go from " << F << " to " << T << endl;

	tie(Pf, D) = border(n, m, F);
	Tf = border(n, m, T).first;
//	cout << "Initial: " << Pf << endl;

	while(Pf.in_board(n, m)) {
//		cout << Pf << " " << Pf.b(B) << endl;
		if(Pf.b(B) == '.') {
			tie(D, Pf.b(B)) = decide(D);
		} else {
			D = reflect(D, Pf.b(B));
		}
		Pf.go(D);
	}

	Pf.go(opposite(D));

	return Pf == Tf;
}


ostream & operator << (ostream & out, Direction D) {
	switch(D) {
#define F(X) case Direction::X: cout << #X; break;
		F(UP)
		F(DOWN)
		F(RIGHT)
		F(LEFT)
	}
#undef F
	return out;
}

void check(int n, int m, vector<int> const& A, vector<int> const& B, Board const& D) {
	for(int i=0; i<n+m; i++) {
		Point P,Q;
	   	Direction D1, D2;

		tie(P, D1) = border(n, m, A[i]);
		tie(Q, D2) = border(n, m, B[i]);

		while(P.in_board(n, m)) {
			D1 = reflect(D1, P.b(D));
			P.go(D1);
		}
		P.go(opposite(D1));

		//cout << A[i] << " " << B[i] << " " << Q << " " << D1 << " " << D2 << endl;
		assert(P == Q);
	}
}

void test() {
	int n, m;
	cin >> n >> m;
	Board D = make_board(n, m);

	vector<int> A(n+m), B(n+m), P(n+m), M(n+m);
	
	for(int i=0; i<n+m; i++) {
		cin >> A[i] >> B[i];
		if(A[i] > B[i]) swap(A[i], B[i]);
		P[i] = B[i] - A[i] - 1;
		M[i] = 2*(n+m) - P[i] - 2;
	}
	bool good = true;

	for(int _=0; _<n+m; _++) {
		int I = -1;

		for(int i=0; i<n+m; i++) {
			if(P[i] == 0 || M[i] == 0) I = i;
		}
		
		if(I == -1) {
			good = false;
			break;
		}
		
		int X, Y;
		if(P[I] == 0) {
			X = A[I];
			Y = B[I];
		} else {
			Y = A[I];
			X = B[I];
		}

		for(int $=0; $<2; $++) {
			for(int i=0; i<n+m; i++) {
				if(A[i] <= X && X<= B[i]) P[i] --;
				if(X <= A[i] || B[i] <= X) M[i] --;
			}
			swap(X, Y);
		}

		P[I] = M[I] = -1;

		if(!go(n, m, D, X, Y)) {
			good = false;
			break;
		}
	}

	if(good) {
		for(auto x : D) {
			while(x.find('.') != string::npos) x[x.find('.')] = '/';
			cout << x << endl;
		}
		check(n, m, A, B, D);
	} else {
		cout << "IMPOSSIBLE" << endl;
	}

}

int main() {
	int t;
	cin >> t;
	for(int i=1; i <= t; i++) {
		cout << "Case #" << i << ":" << endl;
		test();
	}
}
