#include <string>
#include <utility>
#include <vector>

using namespace std;

pair<vector<ssize_t>, vector<ssize_t>> palindromes(const string &s) {
    size_t n = s.size();
    vector<ssize_t> d1(n);
    for (ssize_t i = 0, l = 0, r = -1; i < n; i++) {
        ssize_t k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
        while (k <= i && i + k < n && s[i - k] == s[i + k]) {
            k++;
        }
        d1[i] = k--;
        if (i + k > r) {
            l = i - k;
            r = i + k;
        }
    }
    vector<ssize_t> d2(n);
    for (ssize_t i = 0, l = 0, r = -1; i < n; i++) {
        ssize_t k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (k + 1 <= i && i + k < n && s[i - k - 1] == s[i + k]) {
            k++;
        }
        d2[i] = k--;
        if (i + k > r) {
            l = i - k - 1;
            r = i + k;
        }
    }
    return {d1, d2};
}
