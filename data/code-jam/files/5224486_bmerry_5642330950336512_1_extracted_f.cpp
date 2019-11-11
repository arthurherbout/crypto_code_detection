#include <bits/stdc++.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

/*** START OF TEMPLATE CODE ***/

typedef vector<int> vi;
typedef vector<string> vs;
typedef long long ll;
typedef complex<double> pnt;
typedef pair<int, int> pii;

#define RA(x) std::begin(x), std::end(x)
#define FE(i, x) for (auto i = std::begin(x); i != std::end(x); ++i)
#define SZ(x) ((int) (x).size())

template<class T>
vector<T> splitstr(const string &s)
{
    istringstream in(s);
    vector<T> out;
    copy(istream_iterator<T>(in), istream_iterator<T>(), back_inserter(out));
    return out;
}

static void solve_case(int cas, ostream &cout, unique_ptr<promise<void> > &&input_done);

namespace helpers
{

static sem_t thread_slots;

class sem_poster
{
private:
    sem_t *sem;
public:
    explicit sem_poster(sem_t *sem) : sem(sem) {}
    ~sem_poster() { sem_post(sem); }
    sem_poster(const sem_poster &) = delete;
    sem_poster &operator=(const sem_poster &) = delete;
};

static string run_case(int cas, unique_ptr<promise<void> > &&input_done)
{
    sem_poster poster(&thread_slots);
    ostringstream out;
    solve_case(cas, out, move(input_done));
    return out.str();
}

} // namespace helpers

int main(int argc, char * const *argv)
{
    using namespace helpers;
    ios::sync_with_stdio(false);

    char opt;
    int threads = thread::hardware_concurrency();
    bool threaded = false;
    while ((opt = getopt(argc, argv, "pt:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            threaded = true;
            break;
        case 't':
            {
                char *end;
                threads = strtol(optarg, &end, 0);
                if (end == optarg || *end || threads <= 0)
                {
                    cerr << "Invalid thread count\n";
                    return 2;
                }
            }
            break;
        case ':':
        case '?':
            return 2;
        }
    }

    if (optind < argc)
    {
        int fd = open(argv[optind], O_RDONLY);
        if (fd == -1) { perror(argv[optind]); exit(1); }
        if (-1 == dup2(fd, 0)) { perror(argv[optind]); exit(1); }
        if (-1 == close(fd)) { perror(argv[optind]); exit(1); }
    }
    if (optind + 1 < argc)
    {
        int fd = open(argv[optind + 1], O_WRONLY | O_CREAT, 0666);
        if (fd == -1) { perror(argv[optind + 1]); exit(1); }
        if (-1 == dup2(fd, 1)) { perror(argv[optind + 1]); exit(1); }
        if (-1 == close(fd)) { perror(argv[optind + 1]); exit(1); }
    }
    cin.exceptions(ios::failbit | ios::badbit);

    int cases;
    cin >> cases;
    if (!threaded)
    {
        for (int cas = 0; cas < cases; cas++)
            solve_case(cas, cout, unique_ptr<promise<void> >());
    }
    else
    {
        sem_init(&thread_slots, 0, threads);
        deque<future<string> > outputs;
        for (int cas = 0; cas < cases; cas++)
        {
            sem_wait(&thread_slots);
            // Flush any output we can
            while (!outputs.empty())
            {
                auto status = outputs[0].wait_for(std::chrono::seconds(0));
                if (status != future_status::ready)
                    break;
                cout << outputs[0].get() << flush;
                outputs.pop_front();
            }
            unique_ptr<promise<void> > input_done{new promise<void>()};
            auto input_done_future = input_done->get_future();
            outputs.push_back(async(launch::async, run_case, cas, move(input_done)));
            // Wait until it consumes input before continuing
            input_done_future.get();
        }
        while (!outputs.empty())
        {
            cout << outputs[0].get() << flush;
            outputs.pop_front();
        }
    }
    return 0;
}

/*** END OF TEMPLATE CODE ***/

typedef unsigned char u8;

static int moves(const string &code)
{
    return count(RA(code), 'b') + count(RA(code), 'f');
}

#define BIG (1LL << 40)

struct modifier
{
    vector<u8> delta;
    ll base;
    ll end;

    ll operator[](ll pos) const
    {
        pos -= base;
        pos &= BIG - 1;
        if (pos < SZ(delta))
            return delta[pos];
        else
            return 0;
    }
};

static modifier make_modifier(const string &code)
{
    modifier cur;
    cur.base = 0;
    cur.end = 0;
    ll &base = cur.base;
    ll &end = cur.end;
    deque<u8> delta;
    delta.push_back(0);
    for (char c : code)
    {
        switch (c)
        {
        case 'f':
            end++;
            if (end - base >= SZ(delta))
                delta.push_back(0);
            break;
        case 'b':
            end--;
            if (end < base)
            {
                delta.push_front(0);
                base--;
            }
            break;
        case 'u':
            delta[end - base] -= 1;
            break;
        case 'd':
            delta[end - base] += 1;
            break;
        default:
            assert(0);
        }
    }
    cur.delta = vector<u8>(RA(delta));
    return cur;
}

static ll wrap(ll idx)
{
    idx &= BIG - 1;
    if (idx >= BIG / 2)
        idx -= BIG;
    return idx;
}

// Undefined sign for negative inputs
template<typename T>
static T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }

// m must be positive
template<typename T>
static T mod(T a, T m)
{
    a %= m;
    if (a < 0)
        a += m;
    return a;
}

// a must be relatively prime to m
template<typename T>
static T inverse(T a, T m)
{
    a = mod(a, m);
    if (a == 1)
        return a;
    return mod((1 - inverse(m, a) * m) / a, m);
}

static void solve_case(int cas, ostream &cout, unique_ptr<promise<void> > &&input_done)
{
    // Load input
    string code;
    cin >> code;

    if (input_done) input_done->set_value();

    string prefix;
    string mid;
    string suffix;
    ll ans = moves(code);
    auto pos0 = code.find('(');
    auto pos1 = code.find(')');
    if (pos0 != string::npos)
    {
        prefix = code.substr(0, pos0);
        mid = code.substr(pos0 + 1, pos1 - pos0 - 1);
        suffix = code.substr(pos1 + 1);
        ll mmoves = moves(mid);
        modifier pmod = make_modifier(prefix);
        modifier mmod = make_modifier(mid);
        ll idx = mmod.end;
        u8 sum = mmod[mmod.end];
        ll reps = 1;
        while ((u8) (sum + pmod[idx + pmod.end]) != 0)
        {
            if (llabs(idx) >= 8000)
            {
                ll rep = (BIG - 13000) / abs(mmod.end);
                idx += mmod.end * rep;
                ans += mmoves * rep;
                idx = wrap(idx);
                assert(llabs(idx) < 8000 && llabs(idx) > 4000);
                reps += rep;
            }
            else
            {
                idx += mmod.end;
                sum += mmod[idx];
                ans += mmoves;
                reps++;
            }
        }

        pos0 = suffix.find('(');
        pos1 = suffix.find(')');
        if (pos0 != string::npos)
        {
            string prefix2 = suffix.substr(0, pos0);
            string mid2 = suffix.substr(pos0 + 1, pos1 - pos0 - 1);
            suffix = suffix.substr(pos1 + 1);
            modifier pmod2 = make_modifier(prefix2);
            modifier mmod2 = make_modifier(mid2);
            u8 sum = mmod2[mmod2.end];
            ll bias2 = pmod.end + reps * mmod.end;
            ll r = 1;
            ll s = mmod.end;
            ll inv_s;
            if (s != 0)
            {
                while (!(s & 1))
                {
                    s >>= 1;
                    r <<= 1;
                }
                inv_s = inverse(s, BIG / r);
            }
            ll rmod = BIG / r;
            auto orig = [&] (ll idx) -> u8
            {
                u8 ans = pmod[idx] + pmod2[idx - bias2];
                idx = wrap(idx - pmod.end);
                if (s == 0)
                    ans += reps * mmod[idx];
                else
                {
                    for (int x = mmod.base; x < SZ(mmod.delta) - mmod.base; x++)
                    {
                        u8 v = mmod[x - mmod.base];
                        ll d = idx - x;
                        if (d % r != 0)
                            continue;
                        d /= r;
                        ll k = (d * inv_s) & (rmod - 1);
                        ll hits = reps / rmod;
                        if (k < (reps & (rmod - 1)))
                            hits++;
                        ans += v * hits;
                    }
                }
                return ans;
            };

            const int pcbias = 10000;
            int fixed = 7500;
            if (mmod.end != 0)
                while (fixed % mmod.end)
                    fixed++;
            vector<u8> precalc(2 * pcbias + 1);
            for (int i = -pcbias; i <= pcbias; i++)
                precalc[pcbias + i] = orig(i);
            auto orig_pc = [&](ll idx) -> u8
            {
                idx = wrap(idx);
                if (llabs(idx) < pcbias)
                    return precalc[idx + pcbias];
                else if (mmod.end != 0)
                {
                    idx = mod(idx, mmod.end) + fixed;
                    return precalc[idx + pcbias];
                }
                else
                    return 0;
            };

            ll idx = wrap(bias2 + pmod2.end + mmod2.end);
            sum = mmod2[mmod2.end];
            ll mmoves2 = moves(mid2);
            ll mp = mmod.end ? llabs(mmod.end) : 1;
            ll period = llabs(mp * mmod2.end);
            const ll gap = 8000 + max(257L * llabs(mmod.end), period);
            while ((u8) (sum + orig_pc(idx)) != 0)
            {
                if (llabs(idx) >= gap)
                {
                    ll rep = (BIG - llabs(idx) - 5000) / period * mp;
                    idx += mmod2.end * rep;
                    ans += mmoves2 * rep;
                    idx = wrap(idx);
                    assert(llabs(idx) < gap && llabs(idx) > 4000);
                }
                else
                {
                    idx += mmod2.end;
                    sum += mmod2[idx - bias2 - pmod2.end];
                    ans += mmoves2;
                }
            }
        }
    }

    cout << "Case #" << cas + 1 << ": " << ans << "\n";
}
