#include <string>
#include <vector>

using namespace std;

vector<size_t> prefix_function(const string &s) {
    size_t n = s.size();
    vector<size_t> p(n);
    for (size_t i = 1; i < n; i++) {
        size_t l = p[i - 1];
        while (s[l] != s[i] && l > 0) {
            l = p[l - 1];
        }
        p[i] = s[l] == s[i] ? l + 1 : 0;
    }
    return p;
}
