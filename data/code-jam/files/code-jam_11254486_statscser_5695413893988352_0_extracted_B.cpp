#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>

using namespace std;

string fill_string(string s, char c) {
    int l = s.length();
    for (int i = 0; i < l; ++i) {
        if (s[i] == '?')
            s[i] = c;
    }
    return s;
}

int main(int argc, const char * argv[]) {
    ifstream input("small_in.txt");
    ofstream output("small_out.txt");
    int t, l;
    string c_in, j_in, c_out, j_out;
    string line;
    input >>t;
    getline(input, line);
    for (int i = 0; i < t; ++i) {
        getline(input, line);
        stringstream ss(line);
        ss >>c_in >>j_in;
        l = c_in.length();
        c_out.clear();
        j_out.clear();
        for (int i = l - 1; i >= 0; --i) {
            if (i == l - 1) {
                if (c_in[i] == '?') {
                    if (j_in[i] == '?') {
                        c_out = "0";
                        j_out = "0";
                    } else {
                        c_out = j_in.substr(i, 1);
                        j_out = j_in.substr(i, 1);
                    }
                } else {
                    if (j_in[i] == '?') {
                        c_out = c_in.substr(i, 1);
                        j_out = c_in.substr(i, 1);
                    } else {
                        c_out = c_in.substr(i, 1);
                        j_out = j_in.substr(i, 1);
                    }
                }
            } else {
                c_out.insert(0, "0");
                j_out.insert(0, "0");
                if (c_in[i] == '?') {
                    if (j_in[i] == '?') {
                        int tmp, diff = abs(stoi(c_out) - stoi(j_out));
                        string s1, s2;
                        s1 = fill_string("0" + c_in.substr(i+1), '9');
                        s2 = fill_string("1" + j_in.substr(i+1), '0');
                        tmp = abs(stoi(s1) - stoi(s2));
                        if (tmp < diff) {
                            diff = tmp;
                            c_out = s1;
                            j_out = s2;
                        }
                        s1 = fill_string("1" + c_in.substr(i+1), '0');
                        s2 = fill_string("0" + j_in.substr(i+1), '9');
                        tmp = abs(stoi(s1) - stoi(s2));
                        if (tmp < diff) {
                            diff = tmp;
                            c_out = s1;
                            j_out = s2;
                        }
                    } else {
                        int tmp, diff = abs(stoi(c_out) - stoi(j_out));
                        string s1, s2;
                        c_out[0] = j_in[i];
                        j_out[0] = j_in[i];
                        if (j_in[i] > '0') {
                            s1 = fill_string(string(1, j_in[i] - 1) + c_in.substr(i+1), '9');
                            s2 = fill_string(j_in.substr(i), '0');
                            tmp = abs(stoi(s1) - stoi(s2));
                            if (tmp <= diff) {
                                diff = tmp;
                                c_out = s1;
                                j_out = s2;
                            }
                        }
                        if (j_in[i] < '9') {
                            s1 = fill_string(string(1, j_in[i] + 1) + c_in.substr(i+1), '0');
                            s2 = fill_string(j_in.substr(i), '9');
                            tmp = abs(stoi(s1) - stoi(s2));
                            if (tmp < diff) {
                                diff = tmp;
                                c_out = s1;
                                j_out = s2;
                            }
                        }
                    }
                } else {
                    if (j_in[i] == '?') {
                        int tmp, diff = abs(stoi(c_out) - stoi(j_out));
                        string s1, s2;
                        c_out[0] = c_in[i];
                        j_out[0] = c_in[i];
                        if (c_in[i] > '0') {
                            s1 = fill_string(c_in.substr(i), '0');
                            s2 = fill_string(string(1, c_in[i] - 1) + j_in.substr(i+1), '9');
                            tmp = abs(stoi(s1) - stoi(s2));
                            if (tmp <= diff) {
                                diff = tmp;
                                c_out = s1;
                                j_out = s2;
                            }
                        }
                        if (c_in[i] < '9') {
                            s1 = fill_string(c_in.substr(i), '9');
                            s2 = fill_string(string(1, c_in[i] + 1) + j_in.substr(i+1), '0');
                            tmp = abs(stoi(s1) - stoi(s2));
                            if (tmp < diff) {
                                diff = tmp;
                                c_out = s1;
                                j_out = s2;
                            }
                        }
                    } else {
                        if (c_in[i] == j_in[i]) {
                            c_out[i] = c_in[i];
                            j_out[i] = j_in[i];
                        } else if (c_in[i] < j_in[i]) {
                            c_out = fill_string(c_in.substr(i), '9');
                            j_out = fill_string(j_in.substr(i), '0');
                        } else {
                            c_out = fill_string(c_in.substr(i), '0');
                            j_out = fill_string(j_in.substr(i), '9');
                        }
                    }
                }
            }
        }
        output <<"Case #" <<i+1 <<": " <<c_out <<' ' <<j_out <<endl;
    }
    output.close();
    input.close();
    return 0;
}
