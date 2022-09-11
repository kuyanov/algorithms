#include <algorithm>
#include <cmath>
#include <ctime>
#include <optional>
#include <random>
#include <tuple>
#include <vector>

using namespace std;

namespace Geom2D {
    static constexpr int INF = 1'000'000'000;

    template<typename T>
    struct point {
        T x, y;

        point() : x(0), y(0) {}

        point(T _x, T _y) : x(_x), y(_y) {}

        friend bool operator==(const point &a, const point &b) {
            return tie(a.x, a.y) == tie(b.x, b.y);
        }

        friend bool operator!=(const point &a, const point &b) {
            return !(a == b);
        }

        friend bool operator<(const point &a, const point &b) {
            return tie(a.x, a.y) < tie(b.x, b.y);
        }

        friend bool operator>(const point &a, const point &b) {
            return b < a;
        }

        friend bool operator<=(const point &a, const point &b) {
            return !(b < a);
        }

        friend bool operator>=(const point &a, const point &b) {
            return !(a < b);
        }

        friend point operator+(const point &a, const point &b) {
            return point(a.x + b.x, a.y + b.y);
        }

        friend point operator-(const point &a, const point &b) {
            return point(a.x - b.x, a.y - b.y);
        }

        point operator*(T c) const {
            return point(x * c, y * c);
        }

        point operator/(T c) const {
            return point(x / c, y / c);
        }

        friend T operator*(const point &a, const point &b) {
            return a.x * b.x + a.y * b.y;
        }

        friend T operator^(const point &a, const point &b) {
            return a.x * b.y - b.x * a.y;
        }

        friend istream &operator>>(istream &in, point &a) {
            return in >> a.x >> a.y;
        }

        friend ostream &operator<<(ostream &out, const point &a) {
            return out << a.x << ' ' << a.y;
        }
    };

    template<typename T>
    T dist2(const point<T> &a) {
        return a.x * a.x + a.y * a.y;
    }

    template<typename T>
    double dist(const point<T> &a) {
        return sqrt((double) dist2(a));
    }

    template<typename T>
    T dist2(const point<T> &a, const point<T> &b) {
        return dist2(b - a);
    }

    template<typename T>
    double dist(const point<T> &a, const point<T> &b) {
        return dist(b - a);
    }

    template<typename T>
    int where(const point<T> &a) {
        return a.y < 0 ? -1 : a.y > 0 ? 1 : a.x < 0 ? -1 : a.x > 0 ? 1 : 0;
    }

    template<typename T>
    bool polar_cmp(const point<T> &a, const point<T> &b) {
        int w1 = where(a), w2 = where(b);
        if (w1 != w2) return w1 < w2;
        T v = (a ^ b);
        if (v != 0) return v > 0;
        return dist2(a) < dist2(b);
    }

    template<typename T>
    point<T> norm(const point<T> &a) {
        return a / dist(a);
    }

    template<typename T>
    double angle(const point<T> &a, const point<T> &b) {
        return atan2(a ^ b, a * b);
    }

    template<typename T>
    struct line {
        T a, b, c;

        line() : a(0), b(0), c(0) {}

        line(T _a, T _b, T _c) : a(_a), b(_b), c(_c) {}

        line(const point<T> &p1, const point<T> &p2) :
                a(p1.y - p2.y), b(p2.x - p1.x), c(p1.x * p2.y - p2.x * p1.y) {}
    };

    template<typename T>
    double dist(const point<T> &a, const line<T> &l) {
        T val = l.a * a.x + l.b * a.y + l.c;
        return sqrt((val * val) / double(l.a * l.a + l.b * l.b));
    }

    template<typename T>
    optional<point<T>> intersect(const line<T> &l1, const line<T> &l2) {
        T det = l1.a * l2.b - l2.a * l1.b;
        if (det == 0) return {};
        return point((l2.c * l1.b - l1.c * l2.b) / det, -(l2.c * l1.a - l1.c * l2.a) / det);
    }

    template<typename T>
    line<T> move(const line<T> &l, const point<T> &a) {
        return line(l.a, l.b, l.c - l.a * a.x - l.b * a.y);
    }

    template<typename T>
    point<T> get_point(const line<T> &l, T c) {
        if (l.a != 0) {
            return point((-l.c - l.b * c) / l.a, c);
        }
        if (l.b != 0) {
            return point(c, (-l.c - l.a * c) / l.b);
        }
        return {};
    }

    template<typename T>
    bool in_convex_polygon(const vector<point<T>> &a, const point<T> &p) {
        size_t n = a.size();
        point v = p - a[0];
        if ((v ^ (a[1] - a[0])) > 0 || (v ^ (a[n - 1] - a[0])) < 0) {
            return false;
        }
        size_t l = 1;
        size_t r = n - 1;
        while (r - l > 1) {
            size_t m = (l + r) / 2;
            point cur = a[m] - a[0];
            if ((cur ^ v) > 0) l = m;
            else r = m;
        }
        T val1 = ((a[r] - a[l]) ^ (a[0] - a[l]));
        T val2 = ((a[r] - a[l]) ^ (p - a[l]));
        return val1 == 0 || val2 == 0 || val1 / abs(val1) == val2 / abs(val2);
    }

    template<typename T>
    bool in_polygon(const vector<point<T>> &a, const point<T> &p) {
        size_t n = a.size();
        double ang = 0;
        for (size_t i = 0; i < n; i++) {
            point v1 = a[i] - p, v2 = a[(i + 1) % n] - p;
            if ((v1 ^ v2) == 0 && (v1 * v2) <= 0) return true;
            ang += angle(v1, v2);
        }
        return abs(ang) > 3;
    }

    template<typename T>
    vector<point<T>> convex_hull(vector<point<T>> a) {
        sort(a.begin(), a.end());
        a.resize(unique(a.begin(), a.end()) - a.begin());
        size_t n = a.size();
        point p0 = *(min_element(a.begin(), a.end()));
        vector<point<T>> from_p0;
        for (size_t i = 0; i < n; i++) {
            if (a[i] != p0) {
                from_p0.push_back(a[i] - p0);
            }
        }
        n = from_p0.size();
        sort(from_p0.begin(), from_p0.end(), polar_cmp<T>);
        vector<point<T>> st;
        st.emplace_back(T(0), T(0));
        for (size_t i = 0; i < n; i++) {
            point cur = from_p0[i];
            while (st.size() > 1) {
                point p1 = st[st.size() - 2], p2 = st[st.size() - 1];
                if (((p2 - p1) ^ (cur - p2)) > 0) break;
                st.pop_back();
            }
            st.push_back(cur);
        }
        vector<point<T>> ans(st.size());
        for (size_t i = 0; i < st.size(); i++) {
            ans[i] = p0 + st[i];
        }
        return ans;
    }

    template<typename T>
    double area(const vector<point<T>> &a) {
        size_t n = a.size();
        T ans = 0;
        for (size_t i = 1; i < n - 1; i++) {
            ans += (a[i + 1] - a[0]) ^ (a[i] - a[0]);
        }
        return abs(ans) / 2.0;
    }

    template<typename T>
    optional<point<T>> intersect(vector<line<T>> a) {
        static mt19937 rnd(clock());
        shuffle(a.begin(), a.end(), rnd);
        a.insert(a.begin(), line<T>(1, 0, INF));
        size_t n = a.size();
        point cur = point(T(-INF), T(0));
        for (size_t i = 1; i < n; i++) {
            if (a[i].a * cur.x + a[i].b * cur.y + a[i].c >= 0) continue;
            point dir1 = norm(point(-a[i].b, a[i].a));
            point q = get_point(a[i], T(0));
            optional<point<T>> cur1{}, cur2{};
            T val1 = -INF, val2 = INF;
            for (size_t j = 0; j < i; j++) {
                auto p = intersect(a[j], a[i]);
                if (!p.has_value()) {
                    if (a[j].a * q.x + a[j].b * q.y + a[j].c < 0) return {};
                    else continue;
                }
                point norm2 = point(a[j].a, a[j].b);
                T val = p.value() * dir1;
                if (dir1 * norm2 > 0 && val1 < val) cur1 = p, val1 = val;
                if (dir1 * norm2 < 0 && val2 > val) cur2 = p, val2 = val;
            }
            if (val1 > val2) return {};
            if (cur1.has_value()) cur = cur1.value();
            else if (cur2.has_value()) cur = cur2.value();
            else cur = q;
        }
        return cur;
    }
}
