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

map<string, int> precomputed_val[10];

void SolveProblem(ifstream& in, ofstream& out) {
    string s;
    in >> s;

    int l = s.size();

    if (precomputed_val[l].count(s)) out << precomputed_val[l][s] + 1 << "\n";
    else out << 1 << "\n";
}

const bool SET_THIS_TO_TRUE_FOR_OFFICIAL_SUBMISSION = true;
const string official_input_name = "A-small-attempt0.in";

const string download_folder = "/home/dario2994/Scaricati/";

LL counter_ops = 0;
string DecayString(const string& s) {
    counter_ops++;
    string res(s.size(), '0');
    for (char c : s) {
        if (c != '0') res[c-'1']++;
    }
    return res;
}

void FillGivenSum(string& s, int l, int sum) {
    if (sum > (l-(int)s.size()) * l) return;
    if ((int)s.size() == l) {
        if (sum != 0) return;
        const string& decayed = DecayString(s);
        if (decayed == s) return;
        if (precomputed_val[l].count(s)) precomputed_val[l][decayed] += precomputed_val[l][s] + 1;
        else precomputed_val[l][decayed] += 1;
        return;
    }
    for (int i = 0; i <= min(l, sum); i++) {
        s.push_back(char('0' + i));
        FillGivenSum(s, l, sum-i);
        s.pop_back();
    }
}

int main() {
    int MAXL = 5;
    
    // ofstream backup("backup_file.txt");
    // for (int l = 1; l <= MAXL; l++) {
        // cout << l << endl;
        // for (int s = l * l; s >= 1; s--) {
            // cout << s << endl;
            // dbg_var(counter_ops);
            // string initial_string = "";
            // FillGivenSum(initial_string, l, s);
        // }
        // backup << precomputed_val[l].size() << "\n";
        // for (const auto& x : precomputed_val[l]) {
            // backup << x.first << " " << x.second << "\n";
        // }
    // }
    // return 0;

    ifstream backup("backup_file.txt");
    for (int l = 1; l <= MAXL; l++) {
        int c;
        backup >> c;
        for (int i = 0; i < c; i++) {
            string s;
            int val;
            backup >> s >> val;
            precomputed_val[l][s] = val;
        }
    }
    
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
