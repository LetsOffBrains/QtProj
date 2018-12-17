#include <iostream>
#include "funcs.hpp"
#include "sha256.h"

using namespace std;

void create_sign_eg(string filename);
void check_sign_eg(string filename);
void create_sign_rsa(string filename);
void check_sign_rsa(string filename);
void create_sign_gost(string filename);
void check_sign_gost(string filename);

int main()
{
	srand(time(NULL));

	for(;;) {
		system("cls");

		string filename;
		ifstream f;
		do{
			system("cls");
			cout << "FileName: ";
			cin >> filename;
			f.open(filename.c_str());
		}while(!f.good());
		f.close();

		int k = 0, mode = 0;
		cout << "1 - El-Gamal" << endl
			 << "2 - RSA" << endl
			 << "3 - GOST"  << endl
			 << "other - exit" << endl;

		cin >> k;

		cout << "1 - Create Sign" << endl
			 << "2 - Check Sign" << endl
			 << "other - return" << endl;

		cin >> mode;

		switch (k) {
		case 1:{
			if(mode == 1)
				create_sign_eg(filename);
			else if(mode == 2)
				check_sign_eg(filename);
			else
				continue;
		}break;
		case 2:{
			if(mode == 1)
				create_sign_rsa(filename);
			else if(mode == 2)
				check_sign_rsa(filename);
			else
				continue;
		}break;
		case 3:{
			if(mode == 1)
				create_sign_gost(filename);
			else if(mode == 2)
				check_sign_gost(filename);
			else
				continue;

		}break;
		default:{
			return 0;
		}
		}
		system("PAUSE");
	}
}

void create_sign_eg(string filename) {
	ll p,g,x,y;

	p = generate_num(1000000000);
	ll q = (p-1)/2;

	do{
		g = random(p-1) + 2;
		//cout << q << " ";
		//cout << g << " " << pow(g,q,p) << endl;
		if(pow(g,q,p) != 1 && g > 1 && g < (p-1)) break;
	}while(1);

	cout << "p: " << p << endl << "g: " << g << endl;

	x = random(p-3) + 2;
	y = pow(g,x,p);

	cout << "x: " << x << endl << "y: " << y << endl;

	ull h = sha256_file(filename) % p;

	cout << "h: " << h << endl;

	ll k;
	do{
		k = random(p-3) + 2;
		auto t = gcd(p-1, k);
		if(get<0>(t) == 1) break;
	}while(1);

	cout << "k: " << k << endl;

	ll r = pow(g,k,p);

	ll s = h - x * r % (p-1); //u = (h - x * r) % (p - 1)
	if(s < 0) s += p-1;
	//cout << s << endl;
	ll k_m = inverse(k,p-1);
	//cout << k_m << endl;
	s  = (s * k_m) % (p-1);

	cout << "r: " << r << endl << "s: " << s << endl;

	ofstream out(filename +".sign");
	out << p << " " << g << " " << y << " " << r << " " << s << endl;

	cout << "Signature created!" << endl;
	/*cout << y << " " << s << endl;

	ll a = pow(y,r,p);
	ll b = pow(r,s,p);
	ll f = pow(g,h,p);

	if((a * b % p) == f) {
		cout << "Signature '"+filename+"' is correct." << endl;
	}else{
		cout << "Wrong signature '" + filename + "'." << endl;
	}*/
}

void check_sign_eg(string filename) {
	ll p,g,y,r,s;

	ifstream in(filename + ".sign");

	in >> p >> g >> y >> r >> s;

	ull h = sha256_file(filename) % p;

	cout << "p: " << p << endl << "g: " << g << endl << "y: " << y << endl << "r: " << r << endl << "s: " << s << endl;
	cout << "h: " << h << endl;

	ll a = pow(y,r,p);
	ll b = pow(r,s,p);
	ll f = pow(g,h,p);
	//cout << a << " " << b << " " << (a * b % p) <<" "<< (a * b) % p<<" " << f<< endl;

	if((a * b % p) == f) {
		cout << "Signature '"+filename+"' is correct." << endl;
	}else{
		cout << "Wrong signature '" + filename + "'." << endl;
	}

}

void create_sign_rsa(string filename) {
	ll P, Q, N, phi, c, d;

	do{
		P = generate_num_rsa(32500);
		Q = generate_num_rsa(45000);

		N = P * Q;
	}while(N < 1000000000);

	phi = (P-1) * (Q-1);

	cout <<"P: "<< P << endl << "Q: " << Q << endl;
	cout <<"N: "<< N << endl;
	cout <<"phi: "<< phi << endl;

	auto cd = randcd(phi);
	c = cd.first;
	d = cd.second;
	cout << "c: " << c << endl <<"d: "<< d << endl;

	ull h = sha256_file(filename) % N;

	ll s = pow(h,c,N);

	ofstream out(filename +".sign");
	out << N << " " << d << " " << s << endl;

	//cout << y << " " << s << endl;
	cout << "Signature created!" << endl;
}

void check_sign_rsa(string filename) {
	ll N,d,s;

	ifstream in(filename + ".sign");

	in >> N >> d >> s;

	if(sha256_file(filename) % N == pow(s,d,N)) {
		cout << "Signature '"+filename+"' is correct." << endl;
	}else{
		cout << "Wrong signature '" + filename + "'." << endl;
	}

}

void create_sign_gost(string filename) {
	ll p,q;

	while(1) {
		q = random_bin(15);
		if(!is_prime(q,5)) continue;
		ll b = random_bin(16);
		p = q * b + 1;
		if((p >> 30) && is_prime(p,5) ) break;
	}

	cout << "p: " << p << endl << "q: " << q << endl;

	ll a = 1;
	do {
		ll g = random(p-1) + 1;
		a = pow(g, (p - 1) / q, p);
	} while (a <= 1);

	//cout << pow(a,q,p) << endl;
	cout << "a: " << a << endl;

	ll x = random(q-1) + 1;
	ll y = pow(a,x,p);

	cout << "x: " << x << endl << "y: " << y << endl;

	ull h = sha256_file(filename) % q;

	ll r,s;
	while(1) {
		ll k = random(q-1) + 1;

		r = pow(a,k,p) % q;
		if(r == 0) continue;

		s = ( k*h % q + x*r % q ) % q;
		if(s == 0) continue;

		break;
	}

	cout << "r: " << r << endl << "s: " << s << endl;


	ofstream out(filename +".sign");
	out << p <<" "<< q << " " << a << " " << y << " " << r << " " << s;
	//cout << y << " " << s << endl;
	cout << "Signature created!" << endl;
}

void check_sign_gost(string filename) {
	ll p,q,a,y,r,s;

	ifstream in(filename + ".sign");

	in >> p >> q >> a >> y >> r >> s;

	ull h = sha256_file(filename) % q;

	if(r <= 0 || r >= q || s <= 0 || s >= q){
		cout << "Wrong signature '" + filename + "'." << endl;
	}else{
		ll h_m = inverse(h,q);

		ll u1 = s * h_m % q;
		ll u2 = (q-r) * h_m % q;

		ll v = ( pow(a,u1,p) * pow(y,u2,p) % p ) % q;

		if(v == r) {
			cout << "Signature '"+filename+"' is correct." << endl;
		}else{
			cout << "Wrong signature '" + filename + "'." << endl;
		}
	}
}
