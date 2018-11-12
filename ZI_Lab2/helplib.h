#ifndef HELPLIB_H
#define HELPLIB_H

#include <QPair>

class helpLib
{
public:
	helpLib();

	long long fPow(long long number, long long stepen, long long module);

	long long gcd(long long a, long long b);

	QPair<long long, QPair<long long, long long> > evklid(long long a, long long b);

    bool isPrime(long long p);
};

#endif // HELPLIB_H
