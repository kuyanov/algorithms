#include <string>
#include <vector>

using namespace std;

namespace SuffixAutomaton {
    const int ALPHA = 30;

    vector<vector<int> > go;
    vector<int> suf, len;
    vector<bool> term;

    int new_vertex() {
        go.emplace_back(ALPHA, -1);
        suf.push_back(-1);
        len.push_back(0);
        term.push_back(false);
        return (int) go.size() - 1;
    }

    void build_automaton(const string &s) {
        int root, last;
        root = last = new_vertex();
        for (char e : s) {
            int c = e - 'a';
            int new_last = new_vertex();
            len[new_last] = len[last] + 1;
            int p = last;
            last = new_last;
            while (p != -1 && go[p][c] == -1) {
                go[p][c] = last;
                p = suf[p];
            }
            if (p == -1) {
                suf[last] = root;
                continue;
            }
            int q = go[p][c];
            if (len[q] == len[p] + 1) {
                suf[last] = q;
                continue;
            }
            int r = new_vertex();
            go[r] = go[q];
            len[r] = len[p] + 1;
            suf[r] = suf[q];
            suf[q] = r;
            suf[last] = r;
            while (p != -1 && go[p][c] == q) {
                go[p][c] = r;
                p = suf[p];
            }
        }
        while (last != -1) {
            term[last] = true;
            last = suf[last];
        }
    }
}
