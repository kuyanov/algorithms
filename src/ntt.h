#include <vector>

using namespace std;

namespace NTT {
    using ll = long long;

    const int mod = 998244353;
    const int root_pw = 1 << 23; // mod = c * root_pw + 1
    const ll root = 15311432; // root ^ root_pw = 1

    ll pw(ll a, ll b) {
        if (b == 0) return 1;
        if (b % 2 == 1) {
            return a * pw(a, b - 1) % mod;
        }
        ll c = pw(a, b / 2);
        return c * c % mod;
    }

    ll inv(ll x) {
        return pw(x, mod - 2);
    }

    ll root_1 = inv(root);

    void fft(vector<ll> &a, bool invert) {
        size_t n = a.size();
        for (size_t i = 1, j = 0; i < n; i++) {
            size_t bit = n >> 1;
            for (; j & bit; bit >>= 1) {
                j ^= bit;
            }
            j ^= bit;
            if (i < j) {
                swap(a[i], a[j]);
            }
        }
        for (int len = 2; len <= n; len <<= 1) {
            ll wlen = invert ? root_1 : root;
            for (int i = len; i < root_pw; i <<= 1) {
                wlen = wlen * wlen % mod;
            }
            for (size_t i = 0; i < n; i += len) {
                ll w = 1;
                for (size_t j = 0; j < len / 2; j++) {
                    ll u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = (u + v) % mod;
                    a[i + j + len / 2] = (u - v) % mod;
                    w = w * wlen % mod;
                }
            }
        }
        if (invert) {
            ll n_1 = inv(n);
            for (ll &x: a) {
                x = x * n_1 % mod;
            }
        }
    }

    vector<ll> multiply(vector<ll> a, vector<ll> b) {
        size_t n = 1;
        while (n < a.size() + b.size() - 1) n <<= 1;
        a.resize(n);
        b.resize(n);
        fft(a, false);
        fft(b, false);
        vector<ll> c(n);
        for (size_t i = 0; i < n; i++) c[i] = a[i] * b[i] % mod;
        fft(c, true);
        for (size_t i = 0; i < n; i++) c[i] = (c[i] + mod) % mod;
        return c;
    }
}
