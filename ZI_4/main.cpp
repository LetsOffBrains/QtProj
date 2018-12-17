#include <iostream>
#include <map>
#include <vector>
#include <time.h>
#include <algorithm>

#define ll long long
#define ull unsigned ll

using namespace std;

ull P = 0; // модуль

ull random() {
	ull out = rand();
	for(int i=0; i<10; i++) {
		out = out * rand();
	}
	return out;
}

ll pow(ll a, ll x, ll p) {
	ll sum = 1;
	ll temp = a %p;

	for(; x; x>>=1) {
		if(x & 1) sum = 1LL*(sum * temp) % p;

		temp =  1LL*(temp * temp) % p;
	}

	return sum;
}

bool is_prime(ull n, ll k) {
	if(n < 2 || (n != 2 && n % 2 == 0))
		return false;

	ll s = n - 1;

	while(s % 2 == 0)
		s /= 2;

	for(ll i = 0; i < k; i++) {
		ll a = rand() % (n-1) + 1, temp = s;
		ll mod = pow(a,temp,n);

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

ull generate_prime() {

	while(1) {
		ull a = random() % 10000000;
		if(is_prime(a, 7)) return a;
	}

	return -1;
}

pair<ll,ll> gcd(ll a, ll b) { // возвращает пару чисел, первое - НОД, второе - вторая неизвестная.
	ll U[] = {a,1,0}, V[] = {b,0,1};

	while(V[0] != 0) {
		ll q = U[0] / V[0];
		ll T[] = {U[0] - q*V[0], U[1] - q*V[1], U[2] - q*V[2]};
		U[0] = V[0]; U[1] = V[1]; U[2] = V[2];
		V[0] = T[0]; V[1] = T[1]; V[2] = T[2];
	}

	return make_pair(U[0], U[2]);
}

pair<ll,ll> randcd(ll phi) { // поиск c и d таких что c * d = 1 по модулю phi
	while(1) {
		ll d = random() % phi;
		pair<ll,ll> T = gcd(phi,d);
		if(T.first != 1) continue;
		ll c = T.second;
		if(c < 0) c += phi;

		return make_pair(c,d);
	}
}

struct card{
	ull id;

	card() {
		id = random();
	}
};

map<ull, string> cardNames;

struct Player{
	vector<card> cards;
	string name;
	ull c, d, P;

	Player(ull P) : cards(0), P(P) {
		name = "";
		pair<ll,ll> cd = randcd(P-1);
		c = cd.first;
		d = cd.second;
	}

	void print(){ // выводит c,d и карты игрока
		cout << name << "( c: " << c << ", d: " << d << "): ";

		for(int i=0; i < cards.size(); i++) {
			cout << cardNames[cards[i].id];
			if(i != cards.size() - 1) cout << ", ";
		}
	}
};

vector<card> generate_cards(ull P) {
	vector<card> cards;

	string suits[] = {"Spades", "Hearts", "Clubs", "Diamonds"};
	string names[] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Ace", "King", "Queen", "Jack"};

	for(int i=0; i < 4; i++) {
		for(int j=0; j < 13; j++) {
			card temp;
			temp.id %= P;
			if(cardNames.count(temp.id)){
				--j;
				continue;
			}
			cardNames[temp.id] = "the " + names[j] + " of " + suits[i];
			cards.push_back(temp);
		}
	}

	return cards;
}

vector<Player> generate_players(int num, ull P) {
	vector<Player> players;

	string names[] = {
	"Victor", "Ilya", "Alice",
	"Anton", "Dmitrii", "Oleg",
	"Daniil", "Evgenii", "Artem"
	};

	int k = 0;

	for(int i=0; i < num; i++) {
		Player temp(P);

		temp.name = names[k];
		k++;
		if(k == 9) k = 0;

		players.push_back(temp);
	}

	return players;
}

// функция находит карту по id
card find_card(vector<card> cards, ll id) {
	for(int i=0; i<cards.size(); i++)
		if(id == cards[i].id)
			return cards[i];
}

int main() {
	srand(time(NULL));
	ull P = generate_prime(); // Модуль - Большое Простое Число
	cout <<"P: "<<P << endl;

	int num_players;
	cout << "Input Number of Players: ";
	cin >> num_players;

	cout << "Generating Cards" << endl;
	vector<card> cards = generate_cards(P);

	cout << "Generating Players" << endl;
	vector<Player> players = generate_players(num_players, P);


	for(int i = 0; i < cards.size(); i++) {
		cout << cards[i].id << " - " << cardNames[cards[i].id] << endl;
		if((i + 1) % 13 == 0) cout << endl;
	}


	Player board(P);
	board.name = "Board";
	players.push_back(board);

	vector<ull> K;

	for(int i=0; i<cards.size(); i++) {
		K.push_back(cards[i].id);
	}

	for(int i=0; i < players.size(); i++) {

		for(int j=0; j<K.size(); j++) {
			K[j] = pow(K[j],players[i].c,P); // навешиваем "замок"
		}

		random_shuffle(K.begin(), K.end()); // Перемешиваем вектор в случайном порядке.
	}

	for(int i=0; i < players.size(); i++) {
		int N = 2; // устанавливаем сколько карт берет игрок
		if(players[i].name == "Board") N = 5; // Стол берет 5 карт


		for(int k = 0; k < N; k++) {
			int ind = rand() % K.size();
			ull temp = K[ind];
			K.erase(K.begin() + ind);

			for(int j = 0; j<players.size(); j++) { // другие игроки снимают свои замки. Возводят в степень d
				if(i != j) { // все кроме того кто взял карту
					temp = pow(temp,players[j].d,P);
				}
			}
			temp = pow(temp,players[i].d,P); // тот кто взял карту снимает свой ключ последним

			players[i].cards.push_back(find_card(cards, temp));
		}
	}

	cout << endl;

	vector<card> deck; // колода. пихаем все оставшиеся карты

	for(int i=0; i< K.size(); i++) {
		for(int j=0; j<players.size(); j++) { // все игроки снимают свои ключи
			K[i] = pow(K[i],players[j].d,P);
		}

		deck.push_back(find_card(cards, K[i]));
	}


	cout << "Deck: ";
	for(int i=0; i<deck.size(); i++) {
		cout << cardNames[deck[i].id];
		if(i != deck.size() - 1) cout << ", ";
	}
	cout << endl << endl;

	for(int i=0; i<players.size(); i++) {
		players[i].print();
		cout << endl;
	}

}
