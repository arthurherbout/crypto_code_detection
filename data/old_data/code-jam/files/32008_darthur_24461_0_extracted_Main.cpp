#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

typedef __int64 ll;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Parsing
vector<string> Split(const string &s, const string &delimiters = " ") {
  vector<string> result;
  string temp;
  for (int i = 0; i < (int)s.size(); i++) {
    if (delimiters.find(s[i]) == string::npos) {
      temp += s[i];
    } else {
      if (temp.size() > 0)
        result.push_back(temp);
      temp = "";
    }
  }
  if (temp.size() > 0)
    result.push_back(temp);
  return result;
}
vector<string> SplitDelimiters(const string &s, const string &delimiters = " ") {
  vector<string> result;
  string temp;
  for (int i = 0; i < (int)s.size(); i++) {
    if (delimiters.find(s[i]) == string::npos) {
      if (temp.size() > 0 || result.size() == 0)
        result.push_back(temp);
      temp = "";
    } else {
      temp += s[i];
    }
  }
  result.push_back(temp);
  return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Formatting
int gPrecision = -1;
string ToString(bool x) {return (x? "1" : "0");}
string ToString(char ch) {string s; s += ch; return s;}
string ToString(double x) {
  ostringstream out;
  if (gPrecision != -1) {out.precision(gPrecision); out << fixed;}
  out << x;
  string temp = out.str();
  if (temp[0] != '-') // Never return "-0.0000"
    return temp;
  for (int i = 1; i < (int)temp.size(); i++)
    if (temp[i] != '.' && temp[i] != '0')
      return temp;
  return temp.substr(1);
}
string ToString(float x) {return ToString(double(x));}
string ToString(int n) {ostringstream out; out << n; return out.str();}
string ToString(ll n) {ostringstream out; out << n; return out.str();}
string ToString(const string &s) {return s;}
template <class S, class T> string ToString(const pair<S, T> &x) {
  return "(" + ToString(x.first) + "," + ToString(x.second) + ")";
}
template <class S, class T> string ToString(const map<S, T> &x, const string &delim = ",") {
  string s = "{";
  for (map<S, T>::const_iterator it = x.begin(); it != x.end(); ++it)
    s += (it == x.begin()? "" : delim) + ToString(it->first) + "->" + ToString(it->second);
  return s + "}";
}
template <class T> string ToString(const set<T> &x, const string &delim = ",") {
  string s = "{";
  for (set<T>::const_iterator it = x.begin(); it != x.end(); ++it)
    s += (it == x.begin()? "" : delim) + ToString(*it);
  return s + "}";
}
template <class T> string ToString(const vector<T> &x, const string &delim = ",") {
  string s = "(";
  for (int i = 0; i < (int)x.size(); i++)
    s += (i == 0? "" : delim) + ToString(x[i]);
  return s + ")";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Tools
///////////////////////////////////////////////////////////////////////////////////////////////////

ifstream in("in.txt");
ofstream out("out.txt");
void SolveCase();
int main() {
  int num_cases;
  in >> num_cases;
  for (int i = 0; i < num_cases; i++) {
    out << "Case #" << (i+1) << ": ";
    SolveCase();
    cout << "Done case #" << (i+1) << " of " << num_cases << endl;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

const int kP = 10007;

bool Calculate(int n, vector<int> &c, int exp_value) {
  int result = 0;
  for (int i = 0; i < (int)c.size(); i++) {
    int bm = (1 << i);
    if ((n & bm) > 0) {
      if (c[i] == -1)
        c[i] = (exp_value - result + kP) % kP;
      result = (result + c[i] + kP) % kP;
    }
  }
  return (result == exp_value);
}

int Calculate2(int n, const vector<int> &c) {
  int result = 0;
  for (int i = 0; i < (int)c.size(); i++) {
    int bm = (1 << i);
    if ((n & bm) > 0) {
      if (c[i] == -1)
        return -1;
      result = (result + c[i] + kP) % kP;
    }
  }
  return result;
}

void SolveCase() {
  int n;
  in >> n;
  vector<int> v(n);
  for (int i = 0; i < n; i++)
    in >> v[i];

  int answer = -1;
  for (int border = 0; border < 2048; border++) {
    vector<int> c(15, -1);
    for (int ci = 0; ; ci++) {
      int index = (2048 - border) % (1 << (ci+1));
      int diff = (1 << ci);
      if (index+diff >= (int)v.size())
        break;
      c[ci] = (v[index+diff] - v[index] + kP) % kP;
    }
    bool is_bad = false;
    for (int i = 0; i < n; i++)
      if (!Calculate(i+border, c, v[i]))
        is_bad = true;
    if (is_bad)
      continue;
    int z = Calculate2(border+(int)v.size(), c);
    if (z == -1 || (answer != -1 && z != answer)) {
      out << "UNKNOWN" << endl;
      return;
    }
    answer = z;
  }
  out << answer << endl;
}
