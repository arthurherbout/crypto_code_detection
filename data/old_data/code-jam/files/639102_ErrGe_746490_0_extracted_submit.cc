// Time-stamp: <2010-06-06 23:08:55 errge>

#include <cassert>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <error.h>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <queue>
#include <algorithm>
#include <numeric>
#include <limits>
#include <set>
#include <list>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <utility>
#include <signal.h>
#include <stack>
#include <fstream>
#include <ext/hash_set>
#include <ext/hash_map>
#include <functional>

namespace rg {

// #ifdef RG_DEBUG
static inline void CRASH() { *((int *)0) = 0; }
// #else
// static inline void CRASH() { for (;;); }
// #endif

// ------------------------------------------------- INPUT/OUTPUT
// ------------------------------------------------- INPUT/OUTPUT
// ------------------------------------------------- INPUT/OUTPUT

std::istringstream linestream;
std::string linebuf;

bool getline() {
    bool retval = getline(std::cin, linebuf);
    if (retval) {
        linestream.str(linebuf);
        linestream.clear();
    }
    return retval;
}

void trim(std::string &str) {
    std::string::size_type  notwhite = str.find_first_not_of(" \t\n");
    str.erase(0,notwhite);
    notwhite = str.find_last_not_of(" \t\n");
    str.erase(notwhite+1);
}

bool startswith(std::string const &str, std::string const &prefix) {
    return 0 == str.compare(0, prefix.size(), prefix);
}

// an iomanip that eats everything until the next newline
std::istream&
endl(std::istream &is)
{
    return is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// an iomanip that skips spaces and tabs, but not newlines
std::istream&
skipsp(std::istream &is)
{
    char c;
    while (is.get(c)) {
        if (c != ' ' && c != '\t') {
            is.unget();
            break;
        }
    }
    return is;
}

// asserts that the next char is delim
// if yes, it it consumed, otherwise failbit is set
std::istream&
operator>>(std::istream &is, char const &delim)
{
    if (is.peek() != delim)
        is.setstate(std::ios::failbit);
    else
        is.ignore();
    return is;
}

struct __ignore_range_s {
    __ignore_range_s(char const *range) : set(range) {}
    char const *set;
};

std::istream&
operator>>(std::istream &is, __ignore_range_s const &ir)
{
    char c;
    while (is.get(c)) {
        bool inset = strchr(ir.set, c) != NULL;
        if (!inset) {
            is.unget();
            break;
        }
    }
    return is;
}

// skips over all characters from RANGE
__ignore_range_s
skip(char const *range)
{
    return __ignore_range_s(range);
}

struct __skipto_s {
    __skipto_s(char c) : delim(c) {};
    char delim;
};

std::istream&
operator>>(std::istream &is, __skipto_s const &sts)
{
    return is.ignore(std::numeric_limits<std::streamsize>::max(), sts.delim);
}

// ignores all characters till DELIM
__skipto_s
skipto(char c)
{
    return __skipto_s(c);
}

struct __accept_range_s {
    __accept_range_s(char const *range, std::string &s, bool inclusive = true) :
        set(range), s(s), inclusive(inclusive) {}
    __accept_range_s const &operator!() { inclusive = !inclusive; return *this; }
    char const *set;
    std::string &s;
    bool inclusive;
};

std::istream&
operator>>(std::istream &is, __accept_range_s const &ar)
{
    char c;
    ar.s.clear();
    while (is.get(c)) {
        bool inset = strchr(ar.set, c) != NULL;
        if (ar.inclusive != inset) {
            is.unget();
            break;
        }
        ar.s += c;
    }
    return is;
}

// inputs a string S consisting of characters in RANGE.  use the negation
// operator in the return value to input characters NOT in the range
__accept_range_s
from(char const *range, std::string &s)
{
    return __accept_range_s(range, s, true);
}

// split S into INTO along chars from DELIM
template <typename T>
static void
split(std::string const &s, T &into, char const *delim = " ")
{
    std::istringstream iss(s);
    std::string w;
    while (iss) {
        iss >> skip(delim) >> !from(delim, w);
        into.push_back(w);
    }
}

template <typename S>
void
join(S const &s, std::ostream& str, std::string delim = "\n") {
    int i = 0;
    for (typename S::const_iterator v = s.begin(), e = s.end(); v != e ; ++v)
        if (i++ == 0)
            str << *v;
        else
            str << delim << *v;

}

// braindead sprintf into a string, for small formatting ops
std::string
fmt(char const *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char buf[512];
    int w = vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    if (w < 0)
        return std::string("(sprintf error)");
    std::string ret(buf);
    if (w > int(sizeof(buf) - 1))
        ret += "...";
    return ret;
}

// ------------------------------------------------- ITERATION, ELEM, UNIQ, CONTAINERS UTILS
// ------------------------------------------------- ITERATION, ELEM, UNIQ, CONTAINERS UTILS
// ------------------------------------------------- ITERATION, ELEM, UNIQ, CONTAINERS UTILS
#define AUTO(var, expr) typeof(expr) var(expr);
#define FE(var, cont) for (typeof((cont).begin()) var((cont).begin()), rg_fe_end((cont).end()); var != rg_fe_end; ++var)
#define ALL(cont) (cont).begin(), (cont).end()
#define SORT(cont) std::sort(ALL(cont))
#define COUNT(cont, value) std::count(ALL(cont), value)
#define SUM(cont) std::accumulate(ALL(cont), 0)
#define MAX(cont) *std::max_element(ALL(cont))
#define MIN(cont) *std::min_element(ALL(cont))
#define BII(iter) back_insert_iterator<typeof(iter)>(iter)
#define PTUF(fn, val) pointer_to_unary_function<typeof(val), typeof(fn(val))>(fn)

// repeat N times
#define dotimes(_i, _rep) for (typeof(_rep) _i = 0; _i != (_rep); ++_i)
// loop through a range
#define dorange(_i, _beg, _end) for (typeof(_beg) _i = (_beg), __e = (_end); _i != __e; ++_i)

// elem in (set or map)
template <typename Key, typename Container>
inline bool ins(Key const &k, Container const &m) {
    return m.end() != m.find(k);
}

template <typename Sequence, typename Elem>
inline bool in(Elem const &e, Sequence const &cont) {
    return cont.end() != std::find(ALL(cont), e);
}

template <typename Elem>
inline bool in(Elem const &e, const char* cont) {
    return in(e, std::string(cont));
}

template <typename Key, typename Value>
inline Value const & mg(const std::map<Key, Value> &m, const Key& k) {
#ifdef RG_DEBUG
    if (m.end() == m.find(k)) {
        std::cerr << "rg::mg: Tried to reference a non existent key " << k << " in map " << m << std::endl;
        raise(SIGSEGV);
    }
#endif
    return m.find(k)->second;
}

template <typename Key, typename Value>
inline Value const & hmg(const __gnu_cxx::hash_map<Key, Value> &m, const Key& k) {
#ifdef RG_DEBUG
    if (m.end() == m.find(k)) {
        std::cerr << "rg::mg: Tried to reference a non existent key " << k << " in map " << m << std::endl;
        raise(SIGSEGV);
    }
#endif
    return m.find(k)->second;
}

template <typename Sequence>
void uniq(Sequence &s) {
    typename Sequence::iterator it = std::unique(ALL(s));
    s.erase(it, s.end());
}

// ------------------------------------------------- PRETTY PRINT
// ------------------------------------------------- PRETTY PRINT
// ------------------------------------------------- PRETTY PRINT
template <typename s>
std::ostream& repr(std::ostream &stream, const s &something) {
    stream << something;
    return stream;
}

std::ostream& repr(std::ostream &stream, const char &c) {
    if (c == '\'')
        stream << "'\''";
    else
        stream << "'" << c << "'";

    return stream;
}

std::ostream& repr(std::ostream &stream, const std::string &str) {
    std::string n;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        switch (*it) {
        case '\"': n += "\\\""; break;
        case '\n': n += "\\n"; break;
        default: n += *it;
        }
    stream << '"' << n << '"';
    return stream;
}

template <typename InputIterator>
std::ostream& repr(std::ostream &stream, char bc, char ec, InputIterator first, InputIterator last) {
    stream << bc;
    if (first == last) goto end;

    repr(stream, *first++);
    for (;first != last; ++first)
        repr(stream << ", ", *first);

 end:
    stream << ec;

    return stream;
}

template <typename s>
std::ostream& repr(std::ostream &stream, const std::vector<s> &v) {
    return repr(stream, '[', ']', ALL(v));
}

template <typename s>
std::ostream& repr(std::ostream &stream, const std::list<s> &v) {
    return repr(stream, '[', ']', ALL(v));
}

template <typename v>
std::ostream& repr(std::ostream &stream, const std::set<v> &s) {
    return repr(stream, '{', '}', ALL(s));
}

template <typename k, typename v>
std::ostream& repr(std::ostream &stream, const std::map<k, v> &m) {
    return repr(stream, '{', '}', ALL(m));
}

template <typename k, typename v>
std::ostream& repr(std::ostream &stream, const __gnu_cxx::hash_map<k, v> &m) {
    return repr(stream, '{', '}', ALL(m));
}

template <typename k, typename v>
std::ostream& repr(std::ostream &stream, const std::pair<k, v> &p) {
    stream << '(';
    repr(stream, p.first);
    stream << ", ";
    repr(stream, p.second);
    stream << ')';
    return stream;
}

template <typename one, typename other>
struct swap_pair : public std::unary_function<std::pair<one, other>, std::pair<other, one> > {
    std::pair<other, one> operator()(std::pair<one, other> input) {
        return std::pair<other, one>(input.second, input.first);
    }
};

}

// beteg ez a c++, nagyon beteg
// http://www.velocityreviews.com/forums/t285989-copy-map-to-ostream_iterator.html
namespace std {
template <typename s>
ostream& operator<<(ostream &stream, const vector<s> &v) {
    return rg::repr(stream, v);
}

template <typename k, typename v>
ostream& operator<<(ostream &stream, const pair<k, v> &p) {
    return rg::repr(stream, p);
}

template <typename v>
ostream& operator<<(ostream &stream, const set<v> &s) {
    return rg::repr(stream, s);
}

template <typename v>
ostream& operator<<(ostream &stream, const list<v> &s) {
    return rg::repr(stream, s);
}

template <typename k, typename v>
ostream& operator<<(ostream &stream, const map<k, v> &m) {
    return rg::repr(stream, m);
}

template <typename k, typename v>
ostream& operator<<(ostream &stream, const __gnu_cxx::hash_map<k, v> &m) {
    return rg::repr(stream, m);
}
};

namespace rg {
// ------------------------------------------------- BIPARTITE GRAPHS
// ------------------------------------------------- BIPARTITE GRAPHS
// ------------------------------------------------- BIPARTITE GRAPHS

// augmenting path in bipartite graphs to get a maximum matching:
//   assume this situation (---- is edge, ==== is edge in matching):
//
//   A  *=========*  1
//       \  -----/
//   B  *-x/
//         -------*  2
//
//   C  *---------*  3
//   D  *========/

// So A-1 and D-3 is matched;  B, C and 2 is non-matched.


// Wikipedia (Matching):
// Given a matching M,
//   * an alternating path is a path in which the edges belong alternatively to the matching and not to the matching.
//   * an augmenting path is an alternating path that starts from and ends on free (unmatched) vertices.
//   One can prove that a matching is maximum if and only if it does
//   not have any augmenting path. (This result is sometimes called
//   Berge's Lemma).

// So our example is not a maximum matching, because B->1->A->2 is a
// path which is alternating and starts, ends on a non-matched vertex
// (so it is an augmenting path).

// Algorithm: for every x in {A, B, C, ...} check if there is an
// augmenting path from x.  If there is, flip all of the edges on the
// path and restart the algorithm.  If there is none, we have finished.

// We can search the augmenting path with BFS.
template <typename T>
struct bigraph {
    std::map<T, std::list<T> > foreward, backward;
    typedef std::pair<char, T> vertex; // vertexes are identified by pairs.  E.g.: ('L', 1), ('R', 10)

    // or do nothing if already in
    void add(T left, T right) {
        foreward[left].push_back(right);
        backward[right].push_back(left);
    }
};

template <typename T>
struct matching {
    std::map<T, T> foreward;
    std::map<T, T> backward;

    void add(T left, T right) {
        //    clog << "ADD: " << left << right << endl;
        foreward[left] = right;
        backward[right] = left;
    }

    void remove(T left, T right) {
        //    clog << "REMOVE: " << left << right << endl;
        foreward.erase(left);
        backward.erase(right);
    }
};

template <typename T>
std::ostream& repr(std::ostream &stream, const struct matching<T> &m) {
    return repr(stream, m.foreward);
}

// Look for an augmenting path in a bipartite graph with a current
// matching.  If the matching can be augmented, true is returned and
// the augmenting is done on the match, if this is not possible, false
// is returned.
//
// In visited the set of vertexes which have been visited during the
// search is returned.  If the search failed then this value can be
// used for calculating the minimum vertex cover.
//
// Minimum vertex cover = (L \ T) \cup (R \cap T), where T is the
// returned visited set, L is the set of nodes on the left, R is nodes
// on the right.
template <typename T> bool
bigraph_augment(matching<T>& match, const bigraph<T>& graph, std::set<typename bigraph<T>::vertex> &visited) {
    // BFS
    typedef typename bigraph<T>::vertex vertex;
    std::queue<std::list<vertex> > q;
    visited.clear();
    std::list<vertex> result;
    FE(st, graph.foreward)
        if (!ins(st->first, match.foreward)) // not part of a matching
            q.push(std::list<vertex>(1, make_pair('L', st->first)));
    while (!q.empty()) {
        std::list<vertex> current = q.front(); q.pop();
        const vertex &curlast = current.back();
        visited.insert(curlast);
        if (curlast.first == 'R') {
            // it is a goal if on the right side AND not matched
            if (!ins(curlast.second, match.backward)) {
                result = current;
                break;
            }
            // if it is matched, we have to continue on the matching edge
            if (!ins(make_pair('L', match.backward[curlast.second]), visited)) { // if it is not already done
                current.push_back(make_pair('L', match.backward[curlast.second]));
                q.push(current);
            }
        } else { // 'L'
            //      clog << "L" << graph.foreward.find(curlast.second)->second << endl;
            //      clog << match.foreward << endl;
            FE(st, graph.foreward.find(curlast.second)->second) // go out to all of the possible right nodes
                if (!ins(curlast.second, match.foreward) || // vertex is either unmatched
                    match.foreward[curlast.second] != *st) // current out is not part of the matching
                    if (!ins(make_pair('R', *st), visited)) { // and not done
                        std::list<vertex> n(current);
                        n.push_back(make_pair('R', *st));
                        q.push(n);
                    }
        }
    }

    if (result.size() == 0) // no augmenting path found
        return false;

    // do the flipping
    for (typename std::list<vertex>::const_iterator
             cur = ++result.begin(),
             next = ++++result.begin(),
             end = result.end(); next != end; ++++cur, ++++next)
        match.remove(next->second, cur->second);

    for (typename std::list<vertex>::const_iterator
             cur = result.begin(),
             next = ++result.begin(),
             end = result.end(); cur != end; ++++cur, ++++next)
        match.add(cur->second, next->second);

    return true;
}

template <typename T> struct matching<T>
bigraph_match(const struct bigraph<T> &graph){
    std::set<std::pair<char, T> > visited;
    return bigraph_match(graph, visited);
}

template <typename T> struct matching<T>
bigraph_match(const struct bigraph<T> &graph, std::set<std::pair<char, T> > &visited) {
    matching<T> ret;
    while (bigraph_augment<T>(ret, graph, visited));
    return ret;
}

// Konig wikipedia: min vertex cover = (L \ T) \cup (R \cap T)
template <typename T> std::set<typename bigraph<T>::vertex>
bigraph_minvertex(const struct bigraph<T> &graph, const std::set<typename bigraph<T>::vertex > &t) {
    std::set<typename bigraph<T>::vertex > result;

    // l minus t
    FE(v, graph.foreward)
        if (!ins(make_pair('L', v->first), t))
            result.insert(make_pair('L', v->first));
    // r cap t
    FE(v, graph.backward)
        if (ins(make_pair('R', v->first), t))
            result.insert(make_pair('R', v->first));

    return result;
}

// ------------------------------------------------- TRIE (PREFIX TREE)
// ------------------------------------------------- TRIE (PREFIX TREE)
// ------------------------------------------------- TRIE (PREFIX TREE)
// usage example in 110302_waldorf
template <typename T>
T trieinit();

template <typename T>
struct trie {
    T data;
    std::map<char, trie> children;

    T& operator[](const char* str) {
        if (str[0] == 0)
            return data;
        else
            return children[str[0]][str+1];
    }

    trie() : data(trieinit<T>()) {
    }
};

template <typename T>
std::ostream& repr(std::ostream& str, const trie<T> &t) {
    if (t.data != -1) str << t.data;
    return repr(str, t.children);
}

// ------------------------------------------------- ASTAR
// ------------------------------------------------- ASTAR
// ------------------------------------------------- ASTAR

// astar with the possibility of having multiple optimal solutions
// bfs if heur == 0, costs == 1
template <typename T, typename Impl>
struct astar {
    std::set<T> closed_set;
    std::priority_queue<std::pair<int, T>, std::vector<std::pair<int, T> >, std::greater<std::pair<int, T> > > open_set;

    // back_pointers is list<T>, not just T, so if you need all of the
    // equal costed solutions, you can easily extract those with
    // reconstruct_paths
    std::map<T, std::list<T> > back_pointers;
    std::map<T, int> cost_map;  // g on Wikipedia
    T curnode;
    int curcost;

    void reached(T node, int cost) {
        if (ins(node, closed_set)) return;
        cost += curcost;
        if (!ins(node, cost_map) || cost_map[node] > cost) {
            cost_map[node] = cost;
            back_pointers[node] = std::list<T>(1, curnode);
            open_set.push(make_pair(cost + static_cast<Impl*>(this)->heur_fn(node), node));
        } else if (ins(node, cost_map) && cost_map[node] == cost)
            back_pointers[node].push_back(curnode);
    }

    std::list<T> reconstruct_path(T goal) const {
        std::list<T> ret = std::list<T>(1, goal);
        T next = mg(back_pointers, goal).front();
        while (goal != next) {
            ret.push_front(next);
            goal = next;
            next = mg(back_pointers, goal).front();
        }

        return ret;
    }

    std::vector<std::list<T> > reconstruct_paths(T to) const {
        std::list<T> next = hmg(back_pointers, to);
        if (next.front() == to) {
            if (next.size() != 1) {
                std::cerr << "More back_pointers for the start node!" << endl;
                CRASH();
            }

            return std::vector<std::list<T> >(1, std::list<T>(1, to));
        }

        std::vector<std::list<T> > ret;
        FE(i_next, next) {
            std::vector<std::list<T> > tomerge = reconstruct_paths(*i_next);
            FE(i_tomerge, tomerge)
                i_tomerge->push_back(to);

            copy(ALL(tomerge), std::back_insert_iterator<std::vector<std::list<T > > >(ret));
        }

        return ret;
    }

    bool run(T start) {
        cost_map.clear();
        back_pointers.clear();
        closed_set.clear();
        open_set =
            std::priority_queue<std::pair<int, T>,
                                std::vector<std::pair<int, T> >,
                                std::greater<std::pair<int, T> > >();

        curnode = start;
        curcost = 0;
        reached(start, 0);
        while (!open_set.empty()) {
            curnode = open_set.top().second; open_set.pop();
            if (ins(curnode, closed_set)) continue;
            closed_set.insert(curnode);
            curcost = cost_map[curnode];
            if (!static_cast<Impl*>(this)->node_handler()) return true;
        }

        return false;
    }
};

// example:
// struct graphstar : astar<int, graphstar> {
//     int heur_fn(char _) { return 1; }
//     bool node_handler() {
//         cout << curnode << " cost: " << curcost << endl;
//         if (curnode == 'E') {
//             cout << "E reached" << endl;
//             cout << curcost << endl;
//             return false;
//         }
//         switch (curnode) {
//         case 'A':
//             reached('B', 1);
//             reached('C', 5);
//             break;
//         case 'B':
//             reached('D', 1);
//             reached('C', 3);
//             break;
//         case 'C':
//             reached('A', 1);
//             reached('E', 1);
//             break;
//         case 'D':
//             reached('B', 2);
//             reached('C', 1);
//             reached('E', 5);
//             break;
//         }
//         return true;
//     }
// }
//
// in main:    graphstar().run('A');

// ------------------------------------------------- LOGGING
// ------------------------------------------------- LOGGING
// ------------------------------------------------- LOGGING
std::ofstream devnull("/dev/null");
void rginit() __attribute__ ((constructor));
void rginit() {
#ifdef RG_DEBUG
    if (getenv("NODEBUG"))
#endif
        std::clog.rdbuf(devnull.rdbuf());
}

void rgclose() __attribute__ ((destructor));
void rgclose() {
#ifndef RG_DEBUG
    if (!getenv("MEMDEBUG")) return;
#endif
    std::ifstream f("/proc/self/status");
    std::string line;
    while (std::getline(f, line))
        if (startswith(line, "VmPeak:"))
            std::cerr << line << std::endl;
}
};

using namespace std;
using namespace rg;
using namespace __gnu_cxx;

map<int, int> corners;


void solve() {
    corners.clear();

    int C;
    cin >> C;

    dotimes (iC, C) {
        int corn, vend;
        cin >> corn >> vend;
        corners[corn] = vend;
    }

    int moves = 0;
    while (true) {
        int maxv = 0;
        int maxc = 0;
        FE(i, corners) {
            if (i->second > maxv) {
                maxv = i->second;
                maxc = i->first;
            }
        }

        if (maxv <= 1) break;

        corners[maxc] -= 2;
        corners[maxc-1] += 1;
        corners[maxc+1] += 1;
        ++moves;
    }

    //    cout << corners << endl;
    cout << moves << endl;
}

int main() {
    int t;

    cin >> t;
    dotimes (it, t) {
        cout << "Case #" << it+1 << ": ";
        solve();
    }
    return 0;
}
