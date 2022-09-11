#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

namespace FFT {
    struct base {
        double x, y;

        explicit base(double _x = 0, double _y = 0) : x(_x), y(_y) {};
    };

    inline base operator*(const base &p1, const base &p2) {
        return base(p1.x * p2.x - p1.y * p2.y, p1.x * p2.y + p2.x * p1.y);
    }

    inline void operator/=(base &p1, size_t k) {
        p1.x /= (double) k, p1.y /= (double) k;
    }

    inline base operator-(const base &p1, const base &p2) {
        return base(p1.x - p2.x, p1.y - p2.y);
    }

    inline base operator+(const base &p1, const base &p2) {
        return base(p1.x + p2.x, p1.y + p2.y);
    }

    const int lg = 20;

    vector<base> roots[lg];
    vector<int> rev[lg];

    void precalc() {
        rev[0].resize(1);
        for (int i = 0; i < lg - 1; i++) {
            int len = 1 << i;
            rev[i + 1].resize(2 * len);
            for (int j = 0; j < len; j++) {
                rev[i + 1][j] = rev[i][j] << 1;
                rev[i + 1][j | len] = rev[i + 1][j] + 1;
            }
        }
        for (int i = 0; i < lg; i++) {
            int len = 1 << i;
            base w{cos(M_PI / len), sin(M_PI / len)};
            roots[i].resize(len);
            roots[i][0] = base(1.0);
            for (int j = 1; j < len; j++) {
                roots[i][j] = roots[i][j - 1] * w;
            }
        }
    }

    void fft(vector<base> &a) {
        size_t n = a.size();
        int lg2 = (int) log2(n);
        for (size_t i = 0; i < n; i++) {
            if (i < rev[lg2][i]) swap(a[i], a[rev[lg2][i]]);
        }
        for (int lvl = 0; lvl < lg2; lvl++) {
            int len = 1 << lvl;
            for (size_t st = 0; st < n; st += 2 * len) {
                for (size_t i = st; i < st + len; i++) {
                    base b = roots[lvl][i - st] * a[len + i], t = a[i];
                    a[i] = t + b, a[len + i] = t - b;
                }
            }
        }
    }

    void fft_rev(vector<base> &a) {
        size_t n = a.size();
        fft(a);
        for (size_t i = 0; i < n; i++) a[i] /= n;
        reverse(a.begin() + 1, a.end());
    }

    vector<double> multiply(const vector<double> &a, const vector<double> &b) {
        size_t n = 1;
        while (n < a.size() + b.size() - 1) n <<= 1;
        vector<base> c(n);
        for (size_t i = 0; i < max(a.size(), b.size()); i++) {
            c[i] = base(i < a.size() ? a[i] : 0, i < b.size() ? b[i] : 0);
        }
        fft(c);
        for (size_t i = 0; i <= n / 2; i++) {
            size_t j = i > 0 ? n - i : 0;
            double a1 = (c[i].x + c[j].x) / 2.0;
            double b1 = (c[i].y - c[j].y) / 2.0;
            double c1 = (c[i].y + c[j].y) / 2.0;
            double d1 = (-c[i].x + c[j].x) / 2.0;
            c[i] = base(a1 * c1 - b1 * d1, a1 * d1 + b1 * c1);
            c[j] = base(a1 * c1 - b1 * d1, -a1 * d1 - b1 * c1);
        }
        fft_rev(c);
        vector<double> res(n);
        for (size_t i = 0; i < n; i++) res[i] = c[i].x;
        return res;
    }
}
