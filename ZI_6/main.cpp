#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <ctime>

using namespace std;

set< pair<int,int> > get_ism( set< pair<int,int> > edges, vector<int> perm) {
	set< pair<int,int> > out;

	for(auto t: edges) {
		out.insert(make_pair(perm[t.first], perm[t.second]));
	}

	return out;
}

vector<int> get_r_perm(vector<int> perm) {
	vector<int> out(perm.size());

	for(int i = 1; i < perm.size(); i++) {
		out[perm[i]] = i;
	}

	return out;
}

bool duplicates(vector<int> vec) {
	vec.pop_back();
	sort(vec.begin(), vec.end());

	for(int i = 0; i < vec.size() - 1; i++) {
		if(vec[i] == vec[i+1])
			return true;
	}

	return false;
}

void print_graph(set< pair<int,int> > graph) {
	for(auto t: graph) {
		cout << t.first << " <--> " << t.second << endl;
	}
	cout << endl << endl;
}

void print_vec(vector<int> vec) {
	for(auto t: vec)
		cout << t << " ";
	cout << endl << endl;
}

void print_perm(vector<int> perm) {
	for(int i = 1; i < perm.size(); i++) {
		cout << i << "->" << perm[i] << "\t";
	}
	cout << endl << endl;
}

ifstream in;

class Executor{
private:
	set< pair<int,int> > edges;
	int n, m;

	vector<int> h_cycle;

	vector<int> perm;


public:
	Executor(set< pair<int,int> > ed, int n, int m) {
		edges = ed;
		this->n = n;
		this->m = m;

		for(int i = 0; i < n + 1; i++) {
			int t;
			in >> t;
			h_cycle.push_back(t);
		}
		in.close();

		cout << "Hamiltonian cycle: " << endl;
		print_vec(h_cycle);
//		perm.resize(n + 1);
	}

	set< pair<int,int> > get_ism_graph() {
		perm.clear();
//		perm.push_back(0);
		for(int i = 0; i <= n; i++)
			perm.push_back(i);

		for(int i = 1; i <= n; i++)
			swap(perm[i], perm[rand() % n + 1]);

		return get_ism(edges, perm);
	}

	vector<int> get_perm() {
		return perm;
	}

	vector<int> get_p_cycle() {
		vector<int> out;
		for(auto t: h_cycle) {
			out.push_back(perm[t]);
		}

		return out;
	}

	vector<int> get_h_cycle() {
		return h_cycle;
	}
};

class Customer{
private:
	set< pair<int,int> > graph;
	int n, m;
	set< pair<int,int> > ism_graph;

public:
	Customer(string filename) {
		in.open(filename.c_str());

		in >> n >> m;
		cout << "n: " << n << "\t m: " << m << endl;
		for(int i=0; i<m; i++) {
			int v,u;
			in >> v >> u;
			graph.insert(make_pair(v,u));
		}

		cout << "Initial graph: " << endl;
		print_graph(graph);
	}

	set< pair<int,int> > get_graph() {
		return graph;
	}

	int getn(){
		return n;
	}

	int getm() {
		return m;
	}

	void set_ism_graph(set< pair<int,int> > gr) {
		ism_graph = gr;
		cout << "Isomorphic graph: " << endl;
		print_graph(ism_graph);
	}

	bool check_ism(vector<int> perm) {
		cout << "Perm:" << endl;
		print_perm(perm);

		auto rperm = get_r_perm(perm);

		cout << "Rperm:" << endl;
		print_perm(rperm);

		auto temp_graph = get_ism(ism_graph, rperm);

		cout << "Temp graph:" << endl;
		print_graph(temp_graph);

		if(temp_graph.size() == graph.size()) {
			if(equal(graph.begin(), graph.end(), temp_graph.begin()))
				return true;
		}
		return false;
	}

	bool check_cycle(vector<int> cycle) {
		if(cycle.size() != n + 1) {
			cout << "Wrong size!" << endl;
			return false;
		}
		if(duplicates(cycle)) {
			cout << "There are duplicates!" << endl;
			return false;
		}
		if(cycle[0] != cycle[cycle.size() - 1]) {
			cout << "This is no cycle!" << endl;
			return false;
		}
		for(int i = 0; i < cycle.size() - 1; i++) {
			if(!ism_graph.count(make_pair(cycle[i], cycle[i+1])) &&
					!ism_graph.count(make_pair(cycle[i+1], cycle[i]))) {
				cout << "There is no such edge in the graph!" << endl;
				return false;
			}
		}
		return true;
	}
};

int main() {
//	srand(time(NULL));

	Customer Bob("graph.txt");
	Executor Alice( Bob.get_graph(), Bob.getn(), Bob.getm() );

	cout << "=======================================================" << endl;

	for(int i = 0; i < 3; i++) {
		Bob.set_ism_graph( Alice.get_ism_graph() );

		if(rand() % 2) {
			cout << "Isomorphism test" << endl;
			cout << "Swapping: ";
			print_perm( Alice.get_perm() );
			if( Bob.check_ism(Alice.get_perm()) )
				cout << "Graph is isomorphic, Alice did not lie" << endl;
			else
				cout << "Alice lied, graph is not isomorphic" << endl;
		} else {
			cout << "Cycle test" << endl;
			cout << "Isomorphic cycle: " << endl;
			print_vec(Alice.get_p_cycle());
			if( Bob.check_cycle(Alice.get_p_cycle()) )
				cout << "The cycle is right, Alice did not lie" << endl;
			else
				cout <<"Alice lied, the cycle is not right" << endl;
		}
		cout << "=======================================================" << endl;
	}
	cout << "====================== END ============================" << endl;
	return 0;
}
