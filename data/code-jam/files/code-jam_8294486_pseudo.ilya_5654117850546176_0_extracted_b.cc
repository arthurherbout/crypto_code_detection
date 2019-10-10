/* Don't overcomplicate. */
/* Don't construct unnecessary objects. */
/* Stack is for everything. */

#include <bits/stdc++.h>
#include <experimental/optional>
#include <experimental/any>
using namespace std;
using namespace std::experimental;

using i8  = int_fast8_t;
using i16 = int_fast16_t;
using i32 = int_fast32_t;
using i64 = int_fast64_t;
using u8  = uint_fast8_t;
using u16 = uint_fast16_t;
using u32 = uint_fast32_t;
using u64 = uint_fast64_t;
using st  = size_t;
const std::string nl = "\n";
// std::uniform_int_distribution<size_t> random{0, N};
// std::mt19937_64 rng{static_cast<u64>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count())};

using input_t = array<st, 7>;
using output_t = string;

istream& operator>>(istream& is, input_t& arg)
{
    for (auto& x : arg)
        cin >> x;
    return is;
}

int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(0), std::cin.tie(nullptr);

    st T;
    cin >> T;
    vector<input_t> input(T);
    for (auto& a : input)
    {
        cin >> a;
    }

    auto solve = [](input_t& in) -> output_t
    {
        string out;
        if (in[1])
            out = "R", --in[1];
        else if (in[3])
            out = "Y", --in[3];
        else 
            out = "B", --in[5];
        bool good = true;
        for (st i = 2; i < in.front() && good; ++i)
        {
            // cerr << out << nl;
            switch (out[out.length()-1])
            {
            case 'R':
                if (in[3])
                {
                    --in[3];
                    out += "Y";
                }
                else if (in[5])
                {
                    --in[5];
                    out += "B";
                }
                else
                {
                    out = "IMPOSSIBLE";
                    good = false;
                }
                break;
            case 'Y':
                if (in[1])
                {
                    --in[1];
                    out += "R";
                }
                else if (in[5])
                {
                    --in[5];
                    out += "B";
                }
                else
                {
                    out = "IMPOSSIBLE";
                    good = false;
                }
                break;
            case 'B':
                if (in[1])
                {
                    --in[1];
                    out += "R";
                }
                else if (in[3])
                {
                    --in[3];
                    out += "Y";
                }
                else
                {
                    out = "IMPOSSIBLE";
                    good = false;
                }
                break;
            }
        }
        if (out == "IMPOSSIBLE")
            return out;

        char left;
        if (in[1])
            left = 'R';
        else if (in[3])
            left = 'Y';
        else
            left = 'B';

        auto validate = [](const string& s) -> bool
        {
            for (st i = 0; i < s.length(); ++i)
            {
                auto l = (i + s.length() - 1) % s.length();
                auto r = (i + 1) % s.length();
                if (s[l] == s[i] || s[i] == s[r])
                    return false;
            }
            return true;
        };

        out += left;
        for (st i = out.length()-1; i != -1; --i)
        {
            auto cp = out;
            cp[out.length()-1] = cp[i];
            cp[i] = left;
            if (validate(cp))
                return cp;
        }

        return "IMPOSSIBLE";
    };

    vector<future<output_t>> pool(T);
    for (st i = 0; i < T; ++i)
        pool[i] = async(solve, ref(input[i]));
    for (st i = 0; i < T; ++i)
        cout << "Case #" << i+1 << ": " << pool[i].get() << nl;
}

