// Arup Guha
// 2/1/2014
// Example of fast modular exponentiation.

#include <stdio.h>

long long modPow(long long base, long long exp, long long mod);

int main() {
    printf("%lld\n", modPow(2, 10, 6));
    return 0;
}

// Calculates base^exp mod mod.
long long modPow(long long base, long long exp, long long mod) {

    // Base cases.
    if (exp == 0ll) return 1ll;
    if (exp == 1ll) return base%mod;

    // Time savings here - even exponent case.
    if (exp%2ll == 0ll) {
        long long tmp = modPow(base, exp/2, mod);
        return (tmp*tmp)%mod;
    }

    // Usual recursive breakdown for odd case.
    else {
        return (modPow(base, exp-1, mod)*base)%mod;
    }
}