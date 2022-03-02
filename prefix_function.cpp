#include <string>
#include <vector>

using namespace std;

vector<int> prefix_function(const string &s) {
    int n = (int)s.size();
    vector<int> p(n);
    for (int i = 1; i < n; i++) {
        int l = p[i - 1];
        while (s[l] != s[i] && l > 0) {
            l = p[l - 1];
        }
        if (s[l] == s[i]) {
            p[i] = l + 1;
        } else {
            p[i] = 0;
        }
    }
    return p;
}
