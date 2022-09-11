#include <vector>

using namespace std;

vector<size_t> hungarian(const vector<vector<int>> &a) {
    static constexpr int INF = 1'000'000'000;
    size_t n = a.size();
    vector<vector<int>> b(n + 1, vector<int>(n + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            b[i + 1][j + 1] = a[i][j];
        }
    }
    vector<int> u(n + 1), v(n + 1);
    vector<size_t> p(n + 1), way(n + 1);
    for (size_t i = 1; i <= n; i++) {
        p[0] = i;
        size_t j0 = 0;
        vector<int> minv(n + 1, INF);
        vector<bool> used(n + 1);
        do {
            used[j0] = true;
            int delta = INF;
            size_t i0 = p[j0], j1;
            for (size_t j = 1; j <= n; j++) {
                if (!used[j]) {
                    int cur = b[i0][j] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur, way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j], j1 = j;
                    }
                }
            }
            for (size_t j = 0; j <= n; j++) {
                if (used[j]) {
                    u[p[j]] += delta, v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            size_t j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    vector<size_t> ans(n);
    for (size_t j = 0; j < n; j++) {
        ans[p[j + 1] - 1] = j;
    }
    return ans;
}
