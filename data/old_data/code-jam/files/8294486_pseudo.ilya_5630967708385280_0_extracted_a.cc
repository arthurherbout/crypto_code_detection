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

using input_t = pair<pair<double, u64>, vector<pair<double, double>>>;
using output_t = double;

istream& operator>>(istream& is, input_t& arg)
{
    is >> arg.first.first >> arg.first.second;
    arg.second.resize(arg.first.second);
    for (auto& x : arg.second)
        is >> x.first >> x.second;
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
        // ans is the mean velocity of the first horse ahead of Anna's
        auto D = in.first.first;
        auto N = in.first.second;
        auto& horses = in.second;
        // horses.push_back({0, numeric_limits<double>::max()});
        sort(horses.begin(), horses.end());
        for (auto it = horses.end() - 1; it != horses.begin(); --it)
        {
            auto& a = *(it - 1);
            auto& b = *it;
            auto t = (a.first - b.first) / (b.second - a.second);
            // cerr << "t = " << t << nl;
            auto x = a.second * t + a.first;
            // cerr << "x = " << x << nl;
            if (x < D && t > 0)
            {
                // cerr << "adjusted " << (it - horses.begin()) << nl;
                a.second = (D - a.first) / (t + (D - x) / b.second);
                // cerr << "new velo is " << a.second << nl;
            }
        }

        // output_t out = horses.front().second;
        auto t = (D - horses.front().first) / horses.front().second;
        // auto out = D * horses.front().second / (D - horses.front().first);
        auto out = D / t;
        return out;
    };

    vector<future<output_t>> pool(T);
    for (st i = 0; i < T; ++i)
        pool[i] = async(solve, ref(input[i]));
    for (st i = 0; i < T; ++i)
        // cout << setprecision(10) << "Case #" << i+1 << ": " << pool[i].get() << nl;
        printf("Case #%zu: %.10f\n", i+1, pool[i].get());
}

