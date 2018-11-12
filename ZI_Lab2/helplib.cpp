#include "helplib.h"
#include "cmath"

helpLib::helpLib()
{
}

long long helpLib::fPow(long long number, long long stepen, long long module){
	long long  y = 1, s = number;

	if(stepen == 1) return number;

	while(stepen){
		if(stepen & 1){
			y = (y * s) % module;
		}
		s = (s * s) % module;
		stepen = stepen >> 1;
	}
	return y;
}

long long helpLib::gcd(long long a, long long b)
{
	long long r;
	while(b != 0){
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

QPair<long long, QPair<long long, long long> > helpLib::evklid(long long a, long long b)
{
	long long V[3] = {b, 0 ,1}, U[3] = {a,1,0}, T[3], q;
	while(V[0] != 0){
		q = U[0] / V[0];
		T[0] = U[0] % V[0];
		T[1] = U[1] - q * V[1];
		T[2] = U[2] - q * V[2];
		for(long long i = 0; i < 3; ++i){
			U[i] = V[i];
			V[i] = T[i];
		}
	}
	return qMakePair(U[0], qMakePair(U[1], U[2]));
}

bool helpLib::isPrime(long long p){
   if (p <= 1) return false;

   long long b = (long long)pow(p, 0.5);

   if ((p % 2) == 0) return false;

   for(long long i = 3; i <= b; i+=2){
      if ((p % i) == 0) return false;
   }
   return true;
}
