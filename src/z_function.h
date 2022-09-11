#include <string>
#include <vector>

using namespace std;

vector<size_t> z_function(const string &s) {
    size_t n = s.size();
    vector<size_t> z(n);
    z[0] = n;
    size_t l = 0;
    size_t r = 0;
    for (size_t i = 1; i < n; i++) {
        if (i + z[i - l] <= r) {
            z[i] = z[i - l];
        } else {
            z[i] = r + 1 >= i ? r - i + 1 : 0;
            while (i + z[i] < n && s[i + z[i]] == s[z[i]]) {
                z[i]++;
            }
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}
