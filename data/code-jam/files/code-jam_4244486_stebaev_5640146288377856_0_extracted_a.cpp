#define _CRT_SECURE_NO_WARNINGS

#include <time.h>

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

int calc_bits(int x, int c)
{
    int counter = 0;
    for (int i = 0; i < c; ++i)
    {
        if (x & (1 << i))
        {
            ++counter;
        }
    }
    return counter;
}

int get_bit(int x, int i)
{
    return (x & (1 << i)) ? 1 : 0;
}

vector<vector<long long> > d;

//long long foo(int c, int w, int shooted, int answered)
//{
//    if (calc_bits(shooted & answered, c) == w)
//    {
//        return 0;
//    }
//    int answer = c;
//    for (int step = 0; step < c; ++step)
//    {
//        if (shooted & (1 << step) == 0)
//        {
//            int local_ans = 0;
//            local_ans = max();
//        }        
//    }
//    return 0;
//}

bool possible_miss_state(long long state, int c, int w)
{
    bool good = false;
    int start = 0;
    for (int i = 0; i < c && !good; ++i)
    {
        if (get_bit(state, i))
        {
            start = i + 1;
        }
        else
        {
            if (i - start + 1 >= w)
            {
                good = true;
            }
        }
    }
    return good;
}

long long foo(int state, int c, int w, vector<long long> &d_answer, const vector<vector<long long> > &d_hit)
{
    if (d_answer[state] >= -1)
    {
        return d_answer[state];
    }
    if (!possible_miss_state(state, c, w))
    {
        return d_answer[state] = -1; 
    }

    int answer = c;
    for (int shoot = 0; shoot < c; ++shoot)
    {
        if (get_bit(state, shoot))
        {
            continue;
        }

        int max_answer = max(d_hit[state][shoot], foo(state | (1 << shoot), c, w, d_answer, d_hit));

        if (max_answer >= 0)
        {
            answer = min(max_answer, answer);
        }
    }

    d_answer[state] = answer;

    return answer;
}

long long solve(int R, int C, int W)
{
    long long answer = (R - 1) * (C / W);

    long long last_row_answer = C;

    vector<vector<long long> > d_hit(1 << C, vector<long long>(C, -1));

    for (long long state_miss = 0; state_miss < (1 << C); ++state_miss)
    {        
        if (!possible_miss_state(state_miss, C, W))
        {
            continue;
        }
        // d_hit[state_miss].resize(C, -1);

        long long state_bit_count = calc_bits(state_miss, C);

        for (int hit = 0; hit < C; ++hit)
        {
            if (state_miss & (1 << hit))
            {
                continue;
            }

            int left = hit;
            for (int i = hit - 1; i >= 0; --i)
            {
                if (get_bit(state_miss, i))
                {
                    break;
                }
                else
                {
                    left = i;
                }
            }

            int right = hit;
            for (int i = hit + 1; i < C; ++i)
            {
                if (get_bit(state_miss, i))
                {
                    break;
                }
                else
                {
                    right = i;
                }
            }

            int space = right - left + 1;

            if (space < W)
            {
                continue;
            }

            long long local_answer = state_bit_count + 1;
            if (W > 1)
            {
                local_answer += W;

                if (space == W)
                {
                    local_answer -= 1;
                }
            }

            d_hit[state_miss][hit] = local_answer;

            last_row_answer = min(last_row_answer, local_answer);
        }
    }

    vector<long long> d_answer(1 << C, -2);

    return answer + foo(0, C, W, d_answer, d_hit);
}

long long slow_solve(vector<int> state, int w)
{
    vector<int> debug;
    debug.push_back(1);
    debug.push_back(1);
    debug.push_back(2);
    debug.push_back(1);

    /*if (debug == state)
    {
        cerr << "here" << endl;
    }*/

    bool hit_one = false;
    for (int i = 0; !hit_one && i < state.size(); ++i)
    {
        if (state[i] == 2)
        {
            hit_one = true;
        }
    }

    int most_left = -1, most_right;
    if (hit_one)
    {        
        for (int i = 0; i < state.size(); ++i)
        {
            if (state[i] == 2)
            {
                if (most_left < 0)
                {
                    most_left = i;
                }
                most_right = i;
            }
        }

        if (most_right - most_left + 1 > w)
        {
            return -1;
        }

        for (int i = 0; i < state.size(); ++i)
        {
            if (i >= most_left && i <= most_right && state[i] == 1)
            {
                return -1;
            }
        }

        int left_edge = most_left, right_edge = most_right;
        for (int i = most_left - 1; i >= 0; --i)
        {
            if (state[i] == 1)
            {
                break;
            }
            else
            {
                left_edge = i;
            }
        }
        for (int i = most_right + 1; i < state.size(); ++i)
        {
            if (state[i] == 1)
            {
                break;
            }
            else
            {
                right_edge = i;
            }
        }

        if (right_edge - left_edge + 1 < w)
        {
            return -1;
        }

        int hit_count = 0;
        for (int i = 0; i < state.size(); ++i)
        {
            if (state[i] == 2)
            {
                ++hit_count;
            }
        }
        if (hit_count == w)
        {
            return 0;
        }
    }
    else
    {
        long long encoded_state = 0;
        for (int i = 0; i < state.size(); ++i)
        {
            if (state[i] == 1)
            {
                encoded_state |= 1 << i;
            }
        }
        if (!possible_miss_state(encoded_state, state.size(), w))
        {
            return -1;
        }
    }

    int answer = state.size();
    for (int shoot = 0; shoot < state.size(); ++shoot)
    {
        if (state[shoot] != 0)
        {
            continue;
        }

        vector<int> state_hit = state, state_doesnt_hit = state;
        state_hit[shoot] = 2;
        state_doesnt_hit[shoot] = 1;

        int max_answer = max(slow_solve(state_hit, w), slow_solve(state_doesnt_hit, w));
        if (max_answer < 0)
        {
            continue;
        }

        answer = min(answer, max_answer + 1);
    }

    /*for (int i = 0; i < state.size(); ++i)
    {
        char c = '_';
        if (state[i] == 1)
            c = '0';
        if (state[i] == 2)
            c = 'x';
        cerr << c;
    }
    cerr << " - " << answer << endl;*/

    return answer;
}

long long slow_solve(int r, int c, int w)
{
    return (r - 1) * (c / w) + slow_solve(vector<int>(c, 0), w);
}

void test()
{
    /*cerr << "Time tests:" << endl;
    double worst_time = 0;
    for (int r = 1; r <= 20; ++r)
    {
        cerr << "r = " << r << endl;
        for (int c = 1; c <= 20; ++c)
        {
            for (int w = 1; w <= c; ++w)
            {
                double time = clock();
                solve(r, c, w);
                time = (clock() - time) / CLOCKS_PER_SEC;
                if (time > worst_time)
                {
                    worst_time = time;
                    cerr << "Worst time is " << worst_time << "sec" << endl;
                }
            }
        }
    }*/

    cerr << "Correct tests:" << endl;
    double worst_time = 0;
    for (int r = 1; r <= 20; ++r)
    {
        // cerr << "r = " << r << endl;
        for (int c = 1; c <= 20; ++c)
        {
            cerr << "c = " << c << endl;
            for (int w = 1; w <= c; ++w)
            {                
                long long ans1 = solve(r, c, w);
                long long ans2 = slow_solve(r, c, w);
                
                if (ans1 != ans2)
                {
                    cout << "WA!!!" << endl;
                    cout << r << ' ' << c << ' ' << w << endl;
                    cout << "Got: " << ans1 << endl;
                    cout << "Correct: " << ans2 << endl;
                    return;
                }
            }
        }
    }

}

int main(int argc, char* argv[])
{
    // cerr << slow_solve(1, 4, 2);
    // return 0;

    srand(13);
    ios_base::sync_with_stdio(false);

    // test();

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int T;
    cin >> T;
    for (int test = 1; test <= T; ++test)
    {
        double time = clock();
        int r, c, w;
        cin >> r >> c >> w;
        cout << "Case #" << test << ": " << solve(r, c, w) << endl;
        time = (clock() - time) / CLOCKS_PER_SEC;
        cerr << "Time is " << time << "ms" << endl;
    }

	return 0;
}

