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

using input_t = pair<st, vector<pair<u64, u64>>>;
using output_t = double;

istream& operator>>(istream& is, input_t& arg)
{
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
        st N, K;
        cin >> N >> K;
        a.second.resize(N);
        cin >> a;
        a.first = K;
    }

    auto solve = [](const input_t& in) -> output_t
    {
        const auto K = in.first;
        vector<pair<u64, double>> a(in.second.size());
        for (st i = 0; i < in.second.size(); ++i)
        {
            a[i] = {in.second[i].first, in.second[i].second * in.second[i].first * 2 * M_PI};
        }
        sort(a.begin(), a.end());
        double best_sum = 0;
        for (auto beg = a.rbegin(); beg < a.rend() - K + 1; ++beg)
        {
            double s = M_PI * pow(beg->first, 2) + beg->second;
            vector<pair<u64, double>> cp(beg + 1, a.rend());
            sort(cp.begin(), cp.end(), [](auto& a, auto& b) { return a.second > b.second; });
            s += accumulate(cp.begin(), cp.begin() + K - 1, 0., [](auto& x, auto& y) { return x + y.second; });
            best_sum = max(best_sum, s);
        }
        return best_sum;
    };

    vector<future<output_t>> pool(T);
    for (st i = 0; i < T; ++i)
        pool[i] = async(solve, ref(input[i]));
    for (st i = 0; i < T; ++i)
        printf("Case #%i: %.9f\n", i+1, pool[i].get());
}

