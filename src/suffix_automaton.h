#include <string>
#include <vector>

using namespace std;

class SuffixAutomaton {
public:
    static constexpr int ALPHA = 26;
    static constexpr size_t NULL_VERTEX = -1ul;
    vector<vector<size_t>> go;
    vector<size_t> suf, len;
    vector<bool> term;

    size_t new_vertex() {
        size_t v = go.size();
        go.emplace_back(ALPHA, NULL_VERTEX);
        suf.push_back(NULL_VERTEX);
        len.push_back(0);
        term.push_back(false);
        return v;
    }

    explicit SuffixAutomaton(const std::string &s) {
        size_t root, last;
        root = last = new_vertex();
        for (char e: s) {
            int c = e - 'a';
            size_t new_last = new_vertex();
            len[new_last] = len[last] + 1;
            size_t p = last;
            last = new_last;
            while (p != NULL_VERTEX && go[p][c] == NULL_VERTEX) {
                go[p][c] = last;
                p = suf[p];
            }
            if (p == NULL_VERTEX) {
                suf[last] = root;
                continue;
            }
            size_t q = go[p][c];
            if (len[q] == len[p] + 1) {
                suf[last] = q;
                continue;
            }
            size_t r = new_vertex();
            go[r] = go[q];
            len[r] = len[p] + 1;
            suf[r] = suf[q];
            suf[q] = r;
            suf[last] = r;
            while (p != NULL_VERTEX && go[p][c] == q) {
                go[p][c] = r;
                p = suf[p];
            }
        }
        while (last != NULL_VERTEX) {
            term[last] = true;
            last = suf[last];
        }
    }
};
