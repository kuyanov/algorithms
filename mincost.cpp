#include <algorithm>
#include <queue>
#include <set>
#include <vector>

namespace MinCost {
    const int INF = 1'000'000'000;

    struct edge {
        int u, v, cap, cost, flow;

        edge() : u(-1), v(-1), cap(0), cost(0), flow(0) {}

        edge(int _u, int _v, int _cap, int _cost, int _flow) : u(_u), v(_v), cap(_cap), cost(_cost), flow(_flow) {}
    };

    vector<edge> e;
    vector<vector<int>> g;
    vector<int> p;
    vector<int> w, d;

    void init(int n) {
        g.resize(n);
        w.resize(n);
        d.resize(n);
        p.resize(n);
    }

    void add_edge(int u, int v, int cap, int cost) {
        g[u].push_back((int) e.size());
        e.emplace_back(u, v, cap, cost, 0);
        g[v].push_back((int) e.size());
        e.emplace_back(v, u, 0, -cost, 0);
    }

    void ford_bellman(int s) {
        int n = (int) g.size();
        fill(w.begin(), w.end(), INF);
        w[s] = 0;
        vector<bool> used(n);
        used[s] = true;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            used[u] = false;
            for (int ind: g[u]) {
                if (e[ind].flow < e[ind].cap) {
                    int v = e[ind].v;
                    if (w[v] > w[u] + e[ind].cost) {
                        w[v] = w[u] + e[ind].cost;
                        if (!used[v]) {
                            used[v] = true;
                            q.push(v);
                        }
                    }
                }
            }
        }
    }

    void dijkstra(int s) {
        int n = (int) g.size();
        fill(d.begin(), d.end(), INF);
        d[s] = 0;
        vector<bool> used(n);
        set <pair<int, int>> st;
        st.emplace(d[s], s);
        fill(p.begin(), p.end(), -1);
        while (!st.empty()) {
            int u = (*st.begin()).second;
            st.erase(st.begin());
            if (used[u]) continue;
            used[u] = true;
            for (int ind: g[u]) {
                if (e[ind].flow < e[ind].cap) {
                    int v = e[ind].v;
                    int cost = e[ind].cost + w[u] - w[v];
                    if (d[v] > d[u] + cost) {
                        d[v] = d[u] + cost;
                        p[v] = ind;
                        st.emplace(d[v], v);
                    }
                }
            }
            used[u] = true;
        }
        for (int i = 0; i < n; i++) {
            d[i] += w[i];
        }
    }

    pair<int, int> min_cost_max_flow(int s, int t) {
        ford_bellman(s);
        vector<int> path;
        int cost = 0, flow = 0;
        while (true) {
            dijkstra(s);
            if (p[t] == -1) {
                break;
            }
            path.clear();
            int v = t;
            while (v != s) {
                path.push_back(p[v]);
                v = e[p[v]].u;
            }
            int min_flow = INF;
            for (int ind: path) {
                min_flow = min(min_flow, e[ind].cap - e[ind].flow);
            }
            for (int ind: path) {
                e[ind].flow += min_flow;
                e[ind ^ 1].flow -= min_flow;
                cost += min_flow * e[ind].cost;
            }
            flow += min_flow;
            swap(d, w);
        }
        return {cost, flow};
    }
}
