#include <string>
#include <vector>

using namespace std;

vector<int> z_function(const string &s) {
    int n = (int)s.size();
    vector<int> z(n);
    z[0] = n;
    int l = 0;
    int r = 0;
    for (int i = 1; i < n; i++) {
        if (i + z[i - l] <= r) {
            z[i] = z[i - l];
        } else {
            z[i] = max(r - i + 1, 0);
            while (i + z[i] < n && s[i + z[i]] == s[z[i]]) {
                z[i]++;
            }
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}
