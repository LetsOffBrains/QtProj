#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <map>
#include <sstream>

#define ll long long

using namespace std;

ll fPow(ll a, ll x, ll p){
    ll  y = 1, s = a;

    if(x == 1) return a;

    while(x){
        if(x & 1){
            y = (y * s) % p;
        }
        s = (s * s) % p;
        x = x >> 1;
    }
    return y;
}

ll gcd(ll a, ll b){
    ll r;
    while(b != 0){
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

 ll* evklid(ll a, ll b){
    ll V[3] = {b, 0 ,1}, T[3], q;
    ll* U = new ll[3];
    U[0] = a; U[1] = 1; U[2] = 0;

    while(V[0] != 0){
        q = U[0] / V[0];
        T[0] = U[0] % V[0];
        T[1] = U[1] - q * V[1];
        T[2] = U[2] - q * V[2];
        for(ll i = 0; i < 3; ++i){
            U[i] = V[i];
            V[i] = T[i];
        }
    }
    return U;
}

bool isPrime(ll p){
   if (p <= 1) return false;

   ll b = pow(p, 0.5);

   if ((p % 2) == 0) return false;

   for(ll i = 3; i <= b; i+=2){
      if ((p % i) == 0) return false;
   }
   return true;
}

pair<ll, ll> diffy_hellman(){
    ll g, p, q, xa, xb, ya, yb, zab, zba, m = 100000, n = 10000000;

    do{
        q = m * (1 + rand() % 9) + rand() % m;
        p = 2 * q + 1;
    }
    while(!isPrime(q) || !isPrime(p));

    do{
        g = rand() % (p - 3) + 2;
    }
    while(fPow(g, q, p) == 1);

    xa = n * (1 + rand() % 9 ) + rand() % n;
    xb = n * (1 + rand() % 9 ) + rand() % n;

    ya = fPow(g, xa, p);
    yb = fPow(g, xb, p);

    zab = fPow(yb, xa, p);
    zba = fPow(ya, xb, p);

    return make_pair(zab, zba);
}

ll dLog(ll a, ll g, ll p){
    ll m, b, V, i, j;
    map<ll, ll> U;

    m = sqrt(p);

    b = fPow(g, m, p);

    for(i = m - 1; i >= 0; --i){
        ll tmp = fPow(b, i, p);
        U[tmp] = i;
    }

    for(j = 1; j < m + 1; ++j){
        V = ((a % p) * fPow(g, j, p)) % p;

        if(U.find(V) != U.end()){
            i = U[V];
            break;
        }
    }

    return (m * i - j) % p;
}

ll generate_num(ll p) {
    while(1) {
        ll a = rand() % (p/2);
        if(!isPrime(a)) continue;
        a = 2*a + 1;
        if(isPrime(a)) return a;
    }

    return -1;
}

ll generate_num_rsa(ll p) {
    while(1) {
        ll a = rand() % p;
        if(isPrime(a)) return a;
    }

    return -1;
}

vector<string> split(string s, char del) {
    stringstream ss;
    ss.str(s);
    string item;
    vector<string> out;
    while (getline(ss, item, del)) {
        out.push_back(item);
    }

    return out;
}

int len(ifstream& ifs) {
    ifs.seekg (0, ifs.end);
    int length = ifs.tellg();
    ifs.seekg (0, ifs.beg);
    return length;
}


