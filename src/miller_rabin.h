#include <ctime>
#include <random>

using namespace std;

__int128 power(__int128 x, __int128 y, __int128 n) {
    __int128 res = 1 % n;
    while (y > 0) {
        if (y & 1) res = (res * x) % n;
        y >>= 1;
        x = (x * x) % n;
    }
    return res;
}

bool miller_test(long long d, long long n) {
    static mt19937_64 rnd(clock());
    __int128 a = rnd() % (n - 4) + 2;
    __int128 x = power(a, d, n);
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
