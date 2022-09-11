#include <ctime>
#include <numeric>
#include <random>

using namespace std;

long long pollard(long long n, size_t iter = 20) {
    if (iter == 0 || n == 1) return n;
    if (n % 2 == 0) return 2;
    static mt19937_64 rnd(clock());
    __int128 x = (rnd() % (n - 2)) + 2;
    __int128 y = x;
    __int128 c = (rnd() % (n - 1)) + 1;
    __int128 d = 1;
    while (d == 1) {
        x = (x * x + c + n) % n;
        y = (y * y + c + n) % n;
        y = (y * y + c + n) % n;
        d = gcd(x < y ? y - x : x - y, n);
        if (d == n) return pollard(n, iter - 1);
    }
    return (long long) d;
}
