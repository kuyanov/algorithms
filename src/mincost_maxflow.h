#include <algorithm>
#include <queue>
#include <set>
#include <utility>
#include <vector>

class MinCostMaxFlow {
public:
    struct edge {
        size_t u, v;
        int cap, cost, flow;
    };

    static constexpr int INF = 1'000'000'000;

    vector<edge> e;
    vector<vector<size_t>> g;
    vector<size_t> p;
    vector<int> w, d;

    explicit MinCostMaxFlow(size_t n) {
        g.resize(n);
        w.resize(n);
        d.resize(n);
        p.resize(n);
    }

    void add_edge(size_t u, size_t v, int cap, int cost) {
        g[u].push_back(e.size());
        e.push_back({u, v, cap, cost, 0});
        g[v].push_back(e.size());
        e.push_back({v, u, 0, -cost, 0});
    }

    void ford_bellman(size_t s) {
        size_t n = g.size();
        fill(w.begin(), w.end(), INF);
        w[s] = 0;
        vector<bool> used(n);
        used[s] = true;
        queue <size_t> q;
        q.push(s);
        while (!q.empty()) {
            size_t u = q.front();
            q.pop();
            used[u] = false;
            for (size_t ind: g[u]) {
                if (e[ind].flow < e[ind].cap) {
                    size_t v = e[ind].v;
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

    void dijkstra(size_t s) {
        size_t n = g.size();
        fill(d.begin(), d.end(), INF);
        d[s] = 0;
        vector<bool> used(n);
        set <pair<int, size_t>> st;
        st.emplace(d[s], s);
        fill(p.begin(), p.end(), -1ul);
        while (!st.empty()) {
            size_t u = st.begin()->second;
            st.erase(st.begin());
            if (used[u]) continue;
            used[u] = true;
            for (size_t ind: g[u]) {
                if (e[ind].flow < e[ind].cap) {
                    size_t v = e[ind].v;
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
        for (size_t i = 0; i < n; i++) {
            d[i] += w[i];
        }
    }

    pair<int, int> min_cost_max_flow(size_t s, size_t t) {
        ford_bellman(s);
        vector<size_t> path;
        int cost = 0, flow = 0;
        while (true) {
            dijkstra(s);
            if (p[t] == -1ul) {
                break;
            }
            path.clear();
            size_t v = t;
            while (v != s) {
                path.push_back(p[v]);
                v = e[p[v]].u;
            }
            int min_flow = INF;
            for (size_t ind: path) {
                min_flow = min(min_flow, e[ind].cap - e[ind].flow);
            }
            for (size_t ind: path) {
                e[ind].flow += min_flow;
                e[ind ^ 1].flow -= min_flow;
                cost += min_flow * e[ind].cost;
            }
            flow += min_flow;
            swap(d, w);
        }
        return {cost, flow};
    }
};
