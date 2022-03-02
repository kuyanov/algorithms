#include <numeric>

using namespace std;

long long pollard(long long n) {
    if (n == 1) return n;
    if (n % 2 == 0) return 2;
    long long x = (rand() % (n - 2)) + 2;
    long long y = x;
    long long c = (rand() % (n - 1)) + 1;
    long long d = 1;
    while (d == 1) {
        x = (x * x + c + n) % n;
        y = (y * y + c + n) % n;
        y = (y * y + c + n) % n;
        d = gcd(abs(x - y), n);
        if (d == n) return pollard(n);
    }
    return d;
}
