#include <bits/stdc++.h>
#include "sha256.cpp"
#define ll long long
#define ull unsigned ll

using namespace std;

ull pow(ull a, ull x, ull p) {
	ull sum = 1;
	ull temp = a %p;

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

bool is_prime(ull n, ull k) {
	if(n < 2 || (n != 2 && n % 2 == 0))
		return false;

	ull s = n - 1;

	while(s % 2 == 0)
		s /= 2;

	for(ll i = 0; i < k; i++) {
		ull a = rand() % (n-1) + 1, temp = s;
		ull mod = pow(a,temp,n);

		while( temp != n - 1 && mod != 1 && mod != n - 1) {
			mod = pow(mod,2,n);
			temp *= 2;
		}

		if(mod != n - 1 && temp % 2 == 0) {
			return false;
		}
	}

	return true;
}

ull generate_num(ull p) {

	while(1) {
		ull a = random() % p;
		if(is_prime(a, 7)) return a;
	}

	return -1;
}

pair<ll,ll> gcd(ll a, ll b) {
		ll U[] = {a,1,0}, V[] = {b,0,1};

		while(V[0] != 0) {
			ll q = U[0] / V[0];
			ll T[] = {U[0] - q*V[0], U[1] - q*V[1], U[2] - q*V[2]};
			U[0] = V[0]; U[1] = V[1]; U[2] = V[2];
			V[0] = T[0]; V[1] = T[1]; V[2] = T[2];
		}

		return make_pair(U[0], U[2]);
	}

// инверсия числа x по модулю p // x^-1 % p
ll inverse(ll x, ll p) {
	pair<ll,ll> T = gcd(p,x);

	ll x_m = T.second;
	if(x_m < 0) x_m += p;

	return x_m;
}

ull hash_sha256(ull num) {
	stringstream ss;
	ss << num;
	string str = ss.str();
	string hash = sha256(str);

	ull out = 0;

	for(int i=0; i<hash.length();) {
		ull temp  = 0;

		for(int k = 0; k < 8; i++, k++) {
			temp <<=8;
			int tt = hash[i] - '0';
			if (tt < 0) tt += 256;
			temp += tt;
		}
		if(out == 0) out = temp;
		else out ^= temp;
	}

	return out;
}

pair<ll,ll> randcd(ll phi) {
		while(1) {
			ll d = random() % phi;
			pair<ll,ll> T = gcd(phi,d);
			if(T.first != 1) continue;
			ll c = T.second;
			if(c < 0) c += phi;

			return make_pair(c,d);
		}
	}

int values[] = {1,2,5,10,50,100,200,500,1000,2000,5000}; // номиналы

struct money{ // структура купюры
	ull id,value,sign; // id, номинал, подпись

	money(ull val, ull N) { // конструктор
		id = random(); // так как мы работаем с хешем от id, id может быть абсолютно любым ( больше N например) ( но хэш должен быть меньше N)
		value = val;
		sign = -1; // подписи первоначально нет
	}

	void print() {
		cout << "|== id: " << id << ", \tvalue: " << value << ", \tsign: " << sign << " ==|" << endl;
	}

	friend bool operator< (money const& lhs, money const& rhs) {
		if(lhs.value == rhs.value) return lhs.id < rhs.id;
		return lhs.value < rhs.value;
	}
};

struct client{ // структура клиента
	ull id;
	string name;

	client(){
		id = random();
	}
	friend bool operator< (client const& lhs, client const& rhs) {
		return lhs.id < rhs.id;
	}
};

struct bank{
	ull N,P,Q,phi;
	map<ull,ull> cv,dv; // для разного номинала разные c и d
	vector<ull> used; // вектор для хранения использованных номеров купюр
	map<client,ull> clients; // сколько денег на счету у клиента

	bank() {
		P = generate_num(10000);
		Q = generate_num(10000);
		N = P * Q;
		phi = (P-1) * (Q-1);

		for(auto i: values) {
			pair<ll,ll> cd = randcd(phi);
			cv[i] = cd.first;
			dv[i] = cd.second;
		}
	}

	void add_client(client clt, int sum) {
		clients[clt] = sum;
	}

	ull get_sign(client clt, ull hash_id, int value) { // подписываем то что нам передают клиенты
		if(clients[clt] < value) return -1;
		clients[clt] -= value;
		return pow(hash_id,cv[value],N); // возвращаем подпись
	}

	bool add_money(client clt, money mn) { // функция для проверки подписанной купюры и зачислении на счет другого клиента
		ull id = mn.id;
		ull value = mn.value;
		ull sign = mn.sign;

		if(find(used.begin(), used.end(), id) != used.end()) {
			cout << "Error! Money has already been used!" << endl;
			mn.print();
			system("pause");
			return false;
		}

		ull hash = hash_sha256(id) % N;

		if(sign != pow(hash,cv[value],N)) {
			cout << "Error! Money is bad!" << endl;
			mn.print();
			system("pause");
			return false;
		}

		clients[clt] += value;
		used.push_back(id); // добавляем к вектору использованных id, id купюры
		return true;
	}

	void print() { // вывод данных банка
		cout << "*=============================Bank=============================*" << endl;
		cout << "*\tN: " << N << endl;
		cout << "*\tP: " << P << endl;
		cout << "*\tQ: " << Q << endl;
		cout << "*\tphi: " << phi << endl;
		for(auto i: values) {
			cout << "*\tvalue: " << i << " c: " << cv[i] << " d: " << dv[i] << endl;
		}
		cout << "*\tused: " << endl;
		for(auto i: values) {
			cout << "*\t   " << i << endl;
		}
		cout << "*\tclients: " << endl;

		for(auto it = clients.begin(); it != clients.end(); it++) {
			cout << "*\t   " << it->first.name << " : " << it->second << "$" << endl;
		}
		cout << "*==============================================================*" << endl;
	}
};

bank Sberbank;
ull N;

// функция делит сумму на отдельные купюры
vector<int> get_vm(int sum) {
	vector<int> out;
	for(int i = 10; i >= 0;) {
		if(sum >= values[i]){
			out.push_back(values[i]);
			sum -= values[i];
		} else
			i--;
	}
	return out;
}

// функция переводит определенную сумму от одного клиента другому
bool transfer(client A, client B, int sum) {
	if(sum < 0) return transfer(B,A,-sum); // если сумма отрицательная то клиент B переводит клиенту A

	if(sum > Sberbank.clients[A]) {
		cout << "Not enough money!" << endl;
		system("pause");
		return false;
	}

	vector<int> temp = get_vm(sum);
	vector<money> mns;

	cout << "transfer: "; // выводим какие номиналы используем
	for(auto i: temp) {
		cout << i << " ";
	}
	cout << endl;

	for(auto i: temp) {
		money mn(i, N);
		ull hash_id = hash_sha256(mn.id) % N;

		ull r;
		while(1){
			r = random() % N; // рандомим r
			if(gcd(N,r).first == 1) break; // r должен быть взаимнопростым с N
		}

		ull hashr = hash_id * pow(r, Sberbank.dv[i], N) % N; // находим hash * (r ^ d) % N
		mn.sign = Sberbank.get_sign(A, hashr, i); // отправляем что нашли на подпись банку и получаем подпись

		cout << endl << "###########################################" << endl;
		cout << "# id: " << mn.id << endl;
		cout << "# value: " << mn.value << endl;
		cout << "# hash_id: " << hash_sha256(mn.id) << endl;
		cout << "# hash_id % N: " << hash_id << endl;
		cout << "# r: " << r << endl;
		cout << "# hash_id*r^d : " << hashr << endl;
		cout << "# ^s: " << mn.sign << endl;
		mn.sign = mn.sign * inverse(r,N) % N;
		// ту подпись что получили от банка умножаем на r^-1  и получаем нормальную подпись
		cout << "# s: " << mn.sign << endl;
		cout << "###########################################" << endl;
		mns.push_back(mn);
	}

	for(auto i: mns) {
		Sberbank.add_money(B,i);
		// а он отправляет каждую купюру на проверку Банку
		// и если все ок, то Банк сразу же начисляет деньги клиенту B
	}

	return true;
}


int main() {
	N = Sberbank.N;
	client A,B;
	A.name = "Alice";
	B.name = "Bob";
	Sberbank.add_client(A,10000);
	Sberbank.add_client(B,10000);

	while(1) {
		system("cls");
		Sberbank.print();
		cout << " \n\n> ";
		int sum;
		cout << "(sum > 0) -> Alice to Bob" << endl;
		cout << "(sum < 0) -> Bob to Alice" << endl;
		cout << "Input sum to transfer:";
		cin >> sum;
		transfer(A, B, sum);
		system("pause");
	}

}
