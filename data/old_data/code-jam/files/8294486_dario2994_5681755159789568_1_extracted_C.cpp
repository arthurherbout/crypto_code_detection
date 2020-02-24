#include <bits/stdc++.h>
#include <cstring>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;

#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;

template <typename T1, typename T2>
string print_iterable(T1 begin_iter, T2 end_iter, int counter) {
    bool done_something = false;
    stringstream res;
    res << "[";
    for (; begin_iter != end_iter and counter; ++begin_iter) {
        done_something = true;
        counter--;
        res << *begin_iter << ", ";
    }
    string str = res.str();
    if (done_something) {
        str.pop_back();
        str.pop_back();
    }
    str += "]";
    return str;
}

vector<int> SortIndex(int size, std::function<bool(int, int)> compare) {
    vector<int> ord(size);
    for (int i = 0; i < size; i++) ord[i] = i;
    sort(ord.begin(), ord.end(), compare);
    return ord;
}

template <typename T>
bool MinPlace(T& a, const T& b) {
    if (a > b) {
        a = b;
        return true;
    }
    return false;
}

template <typename T>
bool MaxPlace(T& a, const T& b) {
    if (a < b) {
        a = b;
        return true;
    }
    return false;
}

#define INF 1<<30
#define eps 1e-9

#if DEBUG && !ONLINE_JUDGE
    #define dbg_var(x) clog << #x  << ": " << x << endl;
    #define dbg_vec(x) clog << #x << ": " << print_iterable(x.begin(), x.end(), -1) << endl;
    #define dbg_array(x, len) clog << #x << ": " << print_iterable(x, x+len, -1) << endl;
#else
    #define dbg_var(x) 
    #define dbg_vec(x)
    #define dbg_array(x, len)
#endif

///////////////////////////////////////////////////////////////////////////
//////////////////// DO NOT TOUCH BEFORE THIS LINE ////////////////////////
///////////////////////////////////////////////////////////////////////////

const bool SET_THIS_TO_TRUE_FOR_OFFICIAL_SUBMISSION = true;
const string official_input_name = "C-large.in";

const string download_folder = "/home/dario2994/Scaricati/";

const int MAXN = 150;
vector<int> aa[MAXN];
vector<LL> ww[MAXN];
vector<int> aa2[MAXN];
vector<double> ww2[MAXN];
int N;

// Returns the distance from first_vertex to last_vertex.
// If you are interested in the distance of every vertex from first_vertex,
// just set last_vertex = -1.
// The value of -1 means it's impossible to reach that vertex from first_vertex.
LL dijkstra_dist[MAXN];
LL dijkstra(int first_vertex, int last_vertex) {
    for (int i = 0; i < N; i++) dijkstra_dist[i] = -1; // -1 means INF
    priority_queue <pair<LL, int> > coda;
    dijkstra_dist[first_vertex] = 0;
    coda.push(make_pair(0, first_vertex));
    while(!coda.empty()) {
        auto pp = coda.top();
        coda.pop();
        int v = pp.second;
        LL d = -pp.first;
        
        if (v == last_vertex) return d;
        if (d > dijkstra_dist[v]) continue;
        for (int i = 0; i < (int) aa[v].size(); i++) {
            int a = aa[v][i];
            LL w = ww[v][i];
            if (dijkstra_dist[a] == -1 or dijkstra_dist[a] > d + w) {
                dijkstra_dist[a] = d+w;
                coda.push(make_pair(-d-w, a));
            }
        }
    } 
    return -1; // It is not possible to arrive at last_vertex from first_vertex.
}

// Returns the distance from first_vertex to last_vertex.
// If you are interested in the distance of every vertex from first_vertex,
// just set last_vertex = -1.
// The value of -1 means it's impossible to reach that vertex from first_vertex.
double dijkstra_dist2[MAXN];
double dijkstra2(int first_vertex, int last_vertex) {
    for (int i = 0; i < N; i++) dijkstra_dist2[i] = -1; // -1 means INF
    priority_queue <pair<double, int> > coda;
    dijkstra_dist2[first_vertex] = 0;
    coda.push(make_pair(0, first_vertex));
    while(!coda.empty()) {
        auto pp = coda.top();
        coda.pop();
        int v = pp.second;
        double d = -pp.first;
        
        if (v == last_vertex) return d;
        if (d > dijkstra_dist2[v]) continue;
        for (int i = 0; i < (int) aa2[v].size(); i++) {
            int a = aa2[v][i];
            double w = ww2[v][i];
            if (dijkstra_dist2[a] < -0.5 or dijkstra_dist2[a] > d + w) {
                dijkstra_dist2[a] = d+w;
                coda.push(make_pair(-d-w, a));
            }
        }
    } 
    return -1; // It is not possible to arrive at last_vertex from first_vertex.
}

LL dd[MAXN][MAXN];

void SolveProblem(ifstream& in, ofstream& out) {
    // Read input
    int Q;
    in >> N >> Q;
    for (int i = 0; i < N; i++) {
        aa[i].clear();
        ww[i].clear();
        aa2[i].clear();
        ww2[i].clear();
    }

    vector<LL> E(N);
    vector<LL> S(N);
    for (int i = 0; i < N; i++) in >> E[i] >> S[i];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            LL d;
            in >> d;
            if (d == -1) continue;
            aa[i].push_back(j);
            ww[i].push_back(d);
        }
    }

    for (int i = 0; i < N; i++) {
        dijkstra(i, -1);
        for (int j = 0; j < N; j++) {
            if (dijkstra_dist[j] == -1) dd[i][j] = 1ll<<50;
            else dd[i][j] = dijkstra_dist[j];
        }
        // dbg_var(i);
        // dbg_array(dd[i], N);
    }
    

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j == i) continue;
            if (dd[i][j] > E[i]) continue;
            aa2[i].push_back(j);
            ww2[i].push_back(((double)dd[i][j]) / (double)S[i]);
        }
        // dbg_var(i);
        // dbg_vec(aa2[i]);
        // dbg_vec(ww2[i]);
    }

    // Solve problem

    for (int i = 0; i < Q; i++) {
        int U, V;
        in >> U >> V;
        U--; V--;

        out.precision(10);
        out << fixed << dijkstra2(U, V) << " ";
    }
    
    // Write output
    out << "\n";
}

int main() {
    int t;
    ios::sync_with_stdio(false);
    string input_filename = "";
    string output_filename = "";
    if (SET_THIS_TO_TRUE_FOR_OFFICIAL_SUBMISSION) {
        cout << "Execution of official Input" << endl;
        input_filename = download_folder + official_input_name;
        output_filename = "output.txt";
    } else {
        cout << "Reading SAMPLE test cases" << endl;
        input_filename = "input.txt";
        output_filename = "unofficial_output.txt";
    }
    ifstream in(input_filename);
    ofstream out(output_filename);
    in >> t;
    for (int testcase = 1; testcase <= t; testcase++) {
        out << "Case #" << testcase << ": ";  // Consider adding a new line.
        cout << "Case #" << testcase << endl; // Idea of the status.
        SolveProblem(in, out);
    }
    in.close();
    out.close();

    // Print output on screen.
    cout << endl;   
    ifstream generated_output(output_filename);
    cout << generated_output.rdbuf();
}
