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

template <typename S, typename T>
ostream& operator <<(ostream& out, const pair<S, T>& p) {
    out << "{" << p.first << ", " << p.second << "}";
    return out;
}

template <typename T>
ostream& operator <<(ostream& out, const vector<T>& v) {
    out << "[";
    for (int i = 0; i < (int)v.size(); i++) {
        out << v[i];
        if (i != (int)v.size()-1) out << ", ";
    }
    out << "]";
    return out;
}

#define INF 1<<30
#define eps 1e-9

#if DEBUG && !ONLINE_JUDGE
    #define dbg_var(x) clog << #x  << ": " << x << endl;
    #define dbg_array(x, len) clog << #x << ": " << print_iterable(x, x+len, -1) << endl;
#else
    #define dbg_var(x) 
    #define dbg_array(x, len)
#endif

///////////////////////////////////////////////////////////////////////////
//////////////////// DO NOT TOUCH BEFORE THIS LINE ////////////////////////
///////////////////////////////////////////////////////////////////////////

const int MAXN = 1010;
int N;
vector<int> aa[MAXN];
vector<int> orig[MAXN];
vector<int> transposed[MAXN];
vector<int> val[MAXN];

bool present[5000000];
const int zero = 2500000;
int FindMissingValue(const vector<int>& val) {
    for (int x : val) present[zero + x] = true;
    int res = 0;
    for (int i = 1; i < 2500000; i++) {
        if (!present[zero+i]) {
            res = i;
            break;
        }
        if (!present[zero-i]) {
            res = -i;
            break;
        }
    }
    for (int x : val) present[zero + x] = false;

    return res;
}

bool visited[MAXN];
bool FindPath(vector<pair<int,int>>& path, int v, int source, int sink, int disallowed) {
    if (v == sink) return true;
    visited[v] = true;
    for (int i = 0; i < (int)aa[v].size(); i++) {
        int a = aa[v][i];
        if (i == disallowed && v == source) continue;
        if (visited[a]) continue;
        path.push_back({v, i});
        if (FindPath(path, a, source, sink, disallowed)) {
            return true;
        }
        path.pop_back();
    }
    return false;
}

vector<pair<int, int>> FindCycle(int v, int edge_id) {
    vector<pair<int,int>> res;
    bool found = FindPath(res, aa[v][edge_id], aa[v][edge_id], v, transposed[v][edge_id]);
    for (int v = 0; v < N; v++) visited[v] = false;

    if (!found) return {};
    res.push_back({v, edge_id});
    return res;
}

void CleanOut() {
    for (int i = 0; i < N; i++) {
        aa[i].clear();
        orig[i].clear();
        transposed[i].clear();
        val[i].clear();
    }
}

void SolveProblem(ifstream& in, ofstream& out) {
    int M;

    in >> N >> M;
    
    for (int i = 0; i < M; i++) {
        int a, b;
        in >> a >> b;
        a--, b--;
        aa[a].push_back(b);
        orig[a].push_back(i);
        transposed[a].push_back(aa[b].size());
        val[a].push_back(0);

        aa[b].push_back(a);
        orig[b].push_back(-1);
        transposed[b].push_back(aa[a].size()-1);
        val[b].push_back(0);
    }

    for (int v = 0; v < N; v++) {
        for (int i = 0; i < (int)aa[v].size(); i++) {
            if (val[v][i] != 0) continue;
            vector<pair<int, int>> cycle = FindCycle(v, i); // from v to aa[v][i] without using that edge!
            if (cycle.empty()) {
                CleanOut();
                out << "IMPOSSIBLE\n";
                return;
            }
            vector<int> edge_values;
            for (const auto& edge : cycle) {
                edge_values.push_back(val[edge.first][edge.second]);
            }
            int c = FindMissingValue(edge_values);
            for (const auto& edge : cycle) {
                val[edge.first][edge.second] -= c;
                int u = aa[edge.first][edge.second];
                val[u][transposed[edge.first][edge.second]] += c;
            }
        }
    }

    vector<int> res(M);
    for (int v = 0; v < N; v++) {
        for (int i = 0; i < (int)aa[v].size(); i++) {
            if (orig[v][i] != -1) {
                res[orig[v][i]] = val[v][i];
                assert(abs(val[v][i]) <= N*N);
            }
        }
    }
    for (int i = 0; i < M; i++) out << res[i] << " ";
    out << "\n";

    CleanOut();
}

const bool SET_THIS_TO_TRUE_FOR_OFFICIAL_SUBMISSION = true;
const string official_input_name = "B-large.in";

const string download_folder = "/home/dario2994/Scaricati/";

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
