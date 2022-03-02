#include <cstdlib>

using namespace std;

long long power(long long x, long long y, long long p) {
    long long res = 1 % p;
    while (y > 0) {
        if (y & 1) res = (res * x) % p;
        y >>= 1;
        x = (x * x) % p;
    }
    return res;
}

bool miller_test(long long d, long long n) {
    long long a = 2 + rand() % (n - 4);
    long long x = power(a, d, n);
    if (x == 1 || x == n - 1) return true;
    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;
        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime(long long n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    for (int i = 0; i < k; i++) {
        if (!miller_test(d, n)) {
            return false;
        }
    }
    return true;
}
