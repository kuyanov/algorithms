#include <vector>

using namespace std;

vector<int> hungarian(const vector<vector<int>> &a) {
    const int INF = 1'000'000'000;
    int n = (int) a.size();
    vector<vector<int>> b(n + 1, vector<int>(n + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            b[i + 1][j + 1] = a[i][j];
        }
    }
    vector<int> u(n + 1), v(n + 1), p(n + 1), way(n + 1);
    for (int i = 1; i <= n; i++) {
        p[0] = i;
        int j0 = 0;
        vector<int> minv(n + 1, INF);
        vector<bool> used(n + 1);
        do {
            used[j0] = true;
            int i0 = p[j0], delta = INF, j1;
            for (int j = 1; j <= n; j++) {
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
            for (int j = 0; j <= n; j++) {
                if (used[j]) {
                    u[p[j]] += delta, v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    vector<int> ans(n);
    for (int j = 1; j <= n; j++) {
        ans[p[j] - 1] = j - 1;
    }
    return ans;
}
