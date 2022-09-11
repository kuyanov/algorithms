#include <vector>

using namespace std;

class Dinic {
public:
    struct edge {
        size_t u, v;
        int cap, flow;
    };

    static constexpr int INF = 1'000'000'000;
    static constexpr int lg = 30;

    vector<edge> e;
    vector<vector<size_t> > g;
    vector<size_t> first;
    vector<int> dist;

    explicit Dinic(size_t n) {
        g.resize(n);
    }

    void add_edge(size_t u, size_t v, int cap) {
        g[u].push_back(e.size());
        e.push_back({u, v, cap, 0});
        g[v].push_back(e.size());
        e.push_back({v, u, 0, 0});
    }

    bool bfs(size_t s, size_t t, int c) {
        dist.assign(g.size(), INF);
        first.assign(g.size(), 0);
        vector<size_t> q;
        q.push_back(s);
        dist[s] = 0;
        for (size_t i = 0; i < q.size(); i++) {
            size_t u = q[i];
            for (size_t ind: g[u]) {
                size_t v = e[ind].v;
                int cap = e[ind].cap, flow = e[ind].flow;
                if (cap - flow >= c && dist[v] > dist[u] + 1) {
                    dist[v] = dist[u] + 1;
                    q.push_back(v);
                }
            }
        }
        return dist[t] != INF;
    }

    int dfs(size_t u, size_t t, int c, int cur) {
        if (u == t) {
            return cur;
        }
        for (; first[u] < g[u].size(); first[u]++) {
            size_t ind = g[u][first[u]];
            size_t v = e[ind].v;
            int cap = e[ind].cap, flow = e[ind].flow;
            if (cap - flow >= c && dist[v] == dist[u] + 1) {
                int d = dfs(v, t, c, min(cur, cap - flow));
                if (d > 0) {
                    e[ind].flow += d;
                    e[ind ^ 1].flow -= d;
                    return d;
                }
            }
        }
        return 0;
    }

    int max_flow(size_t s, size_t t) {
        dist.assign(g.size(), INF), first.assign(g.size(), 0);
        for (int c = (1 << lg); c > 0; c >>= 1) {
            while (bfs(s, t, c)) {
                while (dfs(s, t, c, INF));
            }
        }
        int flow = 0;
        for (size_t ind: g[s]) {
            flow += e[ind].flow;
        }
        return flow;
    }
};
