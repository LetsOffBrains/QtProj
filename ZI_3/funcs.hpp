#pragma once
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <map>
#include <sstream>
#include "sha256.cpp"

#define ll long long
#define ull unsigned long long

using namespace std;

ll pow(ll a, ll x, ll p) {
    ll sum = 1;
    ll temp = a %p;
    
    for(; x; x>>=1) {
        if(x & 1) sum = 1LL*(sum * temp) % p;
        
        temp =  1LL*(temp * temp) % p;
    }
    
    return sum;
}

ull random() {
    ull out = rand();
    for(int i=0; i<10; i++) {
        out = out * rand();
    }
    return out;
}

string readFile(const string& fileName) {
	ifstream in;
    in.open(fileName.c_str(), ios_base::binary);
    
    in.seekg (0, in.end);
	ll length = in.tellg();
    in.seekg (0, in.beg);
	string out = "";
    
    char x;
    for(int i=0; i<length; i++) {
    	in.read(&x,sizeof(x));
    	out.push_back(x);
	}
	
	return out;
}

bool is_prime(ll n, ll k) {
	if(n < 2 || (n != 2 && n % 2 == 0))
		return false;
		
	ll t = n - 1;
	ll s = 0;
	
	while(s % 2 == 0){
		t /= 2;
		++s;
	}
	
	for(ll i = 0; i < k; i++) {
		ll a, temp = t;
		do{
			a = rand(); // % (n-1) + 1;
		}while(a < 2 || a >= n - 2);
		ll x = pow(a,temp,n);
		
		if (x == 1 || x == n - 1)
			continue;
		// повторить s - 1 раз
		for (int r = 1; r < s; r++)
		{
			x = pow(x, 2, n);
			// если x == 1, то вернуть "составное"
			if (x == 1)
				return false;
			// если x == n -1, то перейти на следующую итерацию внешнего цикла
			if (x == n - 1)
				break;
		}
		
		/*
		while( temp != n - 1 && x != 1 && x != n - 1) {
			x = pow(x,2,n);
			temp *= 2;
		}
		*/
		if(x != n - 1){// && temp % 2 == 0) {
			return false;
		}
	}
	
	return true;
}

ll random(ll p) {
    return rand() % p;;
}

ll generate_num_rsa(ll p) {
	while(1) {
		ll a = random(p);
		if(is_prime(a, 7)) return a;
	}
}

ll random_bin(int num) {
	ll out = 1 << (num - 1);

	out += random(out); 
	return out;
}

ll* nod(ll a, ll b) {
	ll *U = new ll[3]{a,1,0}, *V = new ll[3]{b,0,1};
	
	while(V[0] != 0) {
		ll q = U[0] / V[0];
		ll *T = new ll[3]{U[0] - q*V[0], U[1] - q*V[1], U[2] - q*V[2]};
		delete [] U;
		U = V;
		V = T;
	}
	
	return U;
}

ull hashFile(string fileName) {
	ifstream in;
    in.open(fileName.c_str(),ios::in | ios_base::binary);
    
    in.seekg (0, in.end);
	ll length = in.tellg();
    in.seekg (0, in.beg);
	ull out = 0;
    
    unsigned char x; 
	ull p = 257; 
	for(int i=0; i < length; i++, p *= p) {
    	in.read((char*)&x,sizeof(x));
    	out += x * p;
	}
	
	return out;
}

ll generate_num(ll p) {

	while(1) {
		ll a = random(p/2);
		if(!is_prime(a, 5)) continue;
		a = 2*a + 1;
		if(is_prime(a, 7)) return a;
	}	
}

ull generate_num() {
	while(1) {
		ull a = random();
		if(!is_prime(a, 5)) continue;
		a = 2*a + 1;
		if(is_prime(a, 7)) return a;
	}	
}

tuple<ll,ll,ll> gcd(ll a, ll b) {
	auto U = make_tuple(a,1,0), V = make_tuple(b,0,1);
	
	while(get<0>(V) != 0) {
		ll q = get<0>(U) / get<0>(V);
		auto T = make_tuple(get<0>(U) - q*get<0>(V), get<1>(U) - q*get<1>(V), get<2>(U) - q*get<2>(V));
		U = V;
		V = T;
	}
	
	return U;
}

ll inverse(ll x, ll p) {
	auto T = nod(p,x);//gcd
	
	auto x_m = T[2];//get<2>(T);
	if(x_m < 0) x_m += p;
	
	return x_m;
}

ull sha256_file(string filename) { // эта функция хеширует 
    string file = readFile(filename); // считываем файл и представляем его как строку (любой файл считается, байты переводятся в символы)
    string hash = sha256(file);  // sha256 - это уже хеш-функция, нашел в инете, как работает не надо разбиратся
    
    //cout << "string hash: " << hash << endl;
    // результат sha256 это строка, нам нужно её как то в число перевести
    // в hash 32 символа
    ull out = 0; 
    for(int i = 0; i < hash.length();) {
    	ull temp  = 0;
    	
    	for(int k = 0; k < 8; i++, k++) { // считываем 8 символов и заносим во временную переменную temp(это число)
    		temp <<= 8;                    // просто берем символ в двоичном виде и записываем temp при этом сдвигая temp на 8 бит влево 
    		int tt = hash[i] - '0';
    		if (tt < 0) tt += 256;
    		temp += tt;
		}
		if(out == 0) out = temp; // считали 8 символов и сделали XOR с out, если в out'е 0 то просто присваеваем
		else out ^= temp;
	}
	
	//cout << "num hash: " << out << endl;
	
	return out;
}

pair<ll,ll> randcd(ll phi) {
	while(1) {
		ll d = random(phi);
		auto T = gcd(phi,d);
		if(get<0>(T) != 1) continue;
		ll c = get<2>(T);
		if(c < 0) c += phi;
		if(c >= phi) continue;

		return make_pair(c,d);
	}
}
