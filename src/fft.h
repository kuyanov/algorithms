#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

namespace FFT {
    struct cd {
        double x, y;

        explicit cd(double _x = 0, double _y = 0) : x(_x), y(_y) {};
    };

    inline cd operator+(const cd &p1, const cd &p2) {
        return cd(p1.x + p2.x, p1.y + p2.y);
    }

    inline cd operator-(const cd &p1, const cd &p2) {
        return cd(p1.x - p2.x, p1.y - p2.y);
    }

    inline cd operator*(const cd &p1, const cd &p2) {
        return cd(p1.x * p2.x - p1.y * p2.y, p1.x * p2.y + p2.x * p1.y);
    }

    inline void operator/=(cd &p1, size_t k) {
        p1.x /= (double) k, p1.y /= (double) k;
    }

    void fft(vector<cd> &a, bool invert) {
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
            double ang = 2 * M_PI / len * (invert ? -1 : 1);
            cd wlen(cos(ang), sin(ang));
            for (size_t i = 0; i < n; i += len) {
                cd w(1);
                for (size_t j = 0; j < len / 2; j++) {
                    cd u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w = w * wlen;
                }
            }
        }
        if (invert) {
            for (cd &x: a) {
                x /= n;
            }
        }
    }

    vector<double> multiply(const vector<double> &a, const vector<double> &b) {
        size_t n = 1;
        while (n < a.size() + b.size() - 1) n <<= 1;
        vector<cd> c(n);
        for (size_t i = 0; i < max(a.size(), b.size()); i++) {
            c[i] = cd(i < a.size() ? a[i] : 0, i < b.size() ? b[i] : 0);
        }
        fft(c, false);
        for (size_t i = 0; i <= n / 2; i++) {
            size_t j = i > 0 ? n - i : 0;
            double a1 = (c[i].x + c[j].x) / 2;
            double b1 = (c[i].y - c[j].y) / 2;
            double c1 = (c[i].y + c[j].y) / 2;
            double d1 = (-c[i].x + c[j].x) / 2;
            c[i] = cd(a1 * c1 - b1 * d1, a1 * d1 + b1 * c1);
            c[j] = cd(a1 * c1 - b1 * d1, -a1 * d1 - b1 * c1);
        }
        fft(c, true);
        vector<double> res(n);
        for (size_t i = 0; i < n; i++) res[i] = c[i].x;
        return res;
    }
}
