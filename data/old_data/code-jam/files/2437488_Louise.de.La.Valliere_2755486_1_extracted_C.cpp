#include <algorithm>
#include <bitset>
#include <cstring>
#include <stdio.h>

using namespace std;

#ifdef _WIN32
#ifdef __MINGW32__
#define getchar_unlocked getchar
#define putchar_unlocked putchar
#else
#define getchar_unlocked _getchar_nolock
#define putchar_unlocked _putchar_nolock
#endif
#endif

static inline int read()
{
	char c;
	while (c = getchar_unlocked(), c <= ' ');
	bool sign = c == '-';
	int x = sign ? 0 : c - '0';
	while (c = getchar_unlocked(), '0' <= c && c <= '9')
		x = x * 10 + (c - '0');
	return sign ? -x : x;
}

static int tree[536870911];
static bitset<536870911> tree_dirty;

static inline int min(int a, int b)
{
	return a < b ? a : b;
}

static inline int max(int a, int b)
{
	return a > b ? a : b;
}

static int min_range_impl(int left, int right, int i, int st, int en)
{
	if (left <= st && right >= en)
		return tree[i];
	else if (right <= st + en >> 1)
		return max(tree[i], min_range_impl(left, right, 2*i+1, st, st + en >> 1));
	else if (left >= st + en >> 1)
		return max(tree[i], min_range_impl(left, right, 2*i+2, st + en >> 1, en));
	else
		return max(tree[i],
		           min(min_range_impl(left, right, 2*i+1, st, st + en >> 1),
		               min_range_impl(left, right, 2*i+2, st + en >> 1, en)));
}

static inline int min_range(int left, int right)
{
	int st = 0, en = sizeof tree / sizeof tree[0] + 1 >> 1;
	return min_range_impl(left, right, 0, st, en);
}

static void raise_range_impl(int left, int right, int h, int i, int st, int en)
{
	tree_dirty[i] = true;
	if (left <= st && right >= en)
		tree[i] = max(tree[i], h);
	else if (right <= st + en >> 1)
	{
		raise_range_impl(left, right, h, 2*i+1, st, st + en >> 1);
		tree[i] = max(tree[i], min(tree[2*i+1], tree[2*i+2]));
	}
	else if (left >= st + en >> 1)
	{
		raise_range_impl(left, right, h, 2*i+2, st + en >> 1, en);
		tree[i] = max(tree[i], min(tree[2*i+1], tree[2*i+2]));
	}
	else
	{
		raise_range_impl(left, right, h, 2*i+1, st, st + en >> 1);
		raise_range_impl(left, right, h, 2*i+2, st + en >> 1, en);
		tree[i] = max(tree[i], min(tree[2*i+1], tree[2*i+2]));
	}
}

static inline void raise_range(int left, int right, int h)
{
	int st = 0, en = sizeof tree / sizeof tree[0] + 1 >> 1;
	return raise_range_impl(left, right, h, 0, st, en);
}

static inline void clean_tree(int i)
{
	tree[i] = 0;
	tree_dirty[i] = false;
	if (2*i+1 < sizeof tree / sizeof tree[0] && tree_dirty[2*i+1]) clean_tree(2*i+1);
	if (2*i+2 < sizeof tree / sizeof tree[0] && tree_dirty[2*i+2]) clean_tree(2*i+2);
}

struct event
{
	int d, w, e, s;
	bool operator<(event e) const { return d < e.d; }
};

int main()
{
	int T = read();
	
	for (int iT = 1; iT <= T; ++iT)
	{
		int N = read();
		
		static event events[1000000];
		event *end = events;
		for (int i = 0; i < N; ++i)
		{
			int d = read();
			int n = read();
			int w = read() + 100900000;
			int e = read() + 100900000;
			int s = read();
			int delta_d = read();
			int delta_p = read();
			int delta_s = read();
			for (int j = 0; j < n; ++j, d += delta_d, w += delta_p, e += delta_p, s += delta_s)
				*end++ = {d, w, e, s};
		}
		sort(events, end);
		
		clean_tree(0);
		int answer = 0;
		
		end = events;
		for (int d = 0; d <= 676060; ++d)
		{
			event *start = end;
			for (; end->d == d; ++end)
				if (min_range(end->w, end->e) < end->s)
					++answer;
			for (; start->d == d; ++start)
				raise_range(start->w, start->e, start->s);
		}
		
		printf("Case #%d: %d\n", iT, answer);
	}
}
