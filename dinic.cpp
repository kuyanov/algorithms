#include <vector>
#include <algorithm>

using namespace std;

namespace Dinic {
    const int INF = 1'000'000'000;
    const int lg = 30;

    struct edge {
        int u, v, cap, flow;

        edge() : u(-1), v(-1), cap(0), flow(0) {}

        edge(int _u, int _v, int _cap, int _flow) : u(_u), v(_v), cap(_cap), flow(_flow) {}
    };

    vector<edge> e;
    vector<vector<int> > g;
    vector<int> first, dist;

    void init(int n) {
        g.resize(n);
    }

    void add_edge(int u, int v, int cap) {
        g[u].push_back((int) e.size());
        e.emplace_back(u, v, cap, 0); // straight edge
        g[v].push_back((int) e.size());
        e.emplace_back(v, u, 0, 0); // back edge, cap for an undirected graph
    }

    bool bfs(int s, int t, int c) {
        dist.assign(g.size(), INF);
        first.assign(g.size(), 0);
        vector<int> q;
        q.push_back(s);
        dist[s] = 0;
        for (int i = 0; i < (int) q.size(); i++) {
            int u = q[i];
            for (int ind : g[u]) {
                int v = e[ind].v, cap = e[ind].cap, flow = e[ind].flow;
                if (cap - flow >= c && dist[v] > dist[u] + 1) {
                    dist[v] = dist[u] + 1;
                    q.push_back(v);
                }
            }
        }
        return dist[t] != INF;
    }

    int dfs(int u, int t, int c, int cur) {
        if (u == t) {
            return cur;
        }
        for (; first[u] < (int) g[u].size(); first[u]++) {
            int ind = g[u][first[u]];
            int v = e[ind].v, cap = e[ind].cap, flow = e[ind].flow;
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

    int max_flow(int s, int t) {
        dist.assign(g.size(), INF), first.assign(g.size(), 0);
        for (int c = (1 << lg); c > 0; c >>= 1) {
            while (bfs(s, t, c)) {
                while (dfs(s, t, c, INF));
            }
        }
        int flow = 0;
        for (int ind : g[s]) {
            flow += e[ind].flow;
        }
        return flow;
    }
}
