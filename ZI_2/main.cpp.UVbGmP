#include <iostream>
#include <vector>
#include <fstream>
#include "funcs.hpp"

void eg_encrypt(string);
void eg_decrypt(string);
void rsa_encrypt(string);
void rsa_decrypt(string);
void vern_encrypt(string);
void vern_decrypt(string);
void shamir(long long);

int main()
{
    srand(time(NULL));

    for(;;) {
        system("cls");
        int k = 0, mode = 0;
        cout << "1 - encrypt" << endl
             << "2 - decrypt" << endl
             << "3 - Shamir"  << endl
             << "other - exit" << endl;

        cin >> k;

        if(k == 1 || k == 2){
            cout << "Select mode:"  << endl
                 << "1 - El-Gamal"  << endl
                 << "2 - RSA"       << endl
                 << "3 - Vernam"    << endl
                 << "other - return"<< endl;
            cin >> mode;
            if(mode >= 1 && mode <= 3){
                string filename;
                cout << "filename: ";
                cin >> filename;

                switch (mode) {
                case 1:{
                    cout << "El-Gamal" << endl;
                    if(k == 1)
                        eg_encrypt(filename);
                    else if(k == 2)
                        eg_decrypt(filename);
                    system("PAUSE");
                }break;
                case 2:{
                    cout << "RSA" << endl;
                    if(k == 1)
                        rsa_encrypt(filename);
                    else if(k == 2)
                        rsa_decrypt(filename);
                    system("PAUSE");
                }break;
                case 3:{
                    cout << "Vernam" << endl;
                    if(k == 1)
                        vern_encrypt(filename);
                    else if(k == 2)
                        vern_decrypt(filename);
                    system("PAUSE");
                }break;
                }
            }
        }
        else if(k == 3){
            cout << "Enter numeric messange: ";
            long long msg;
            cin >> msg;
            shamir(msg);
            system("PAUSE");
        }
        else{
            return 0;
        }
    }
    return 0;
}

void eg_encrypt(string filename) {
    vector<string> names = split(filename,'.');

    ifstream in;
    in.open(filename.c_str(), ios_base::binary);
    if(!in.is_open()){
        cout << "Can't open " << filename << endl;
        return;
    }

    ofstream out, setting;
    out.open((names[0] + ".code_" + names[1]).c_str(), ios_base::binary);
    setting.open((names[0] + ".setting").c_str(), ios_base::binary);

    ll p, g, c, d;

    p = generate_num(1000000);

    do{
        g = rand() % (p-1);
    }while(fPow(g,(p-1)/2,p) == 1);

    c = rand() % (p-1);
    d = fPow(g,c,p);
    setting << p <<" "<< c;
    setting.close();

    cout << "P = " << p << endl
         << "g = " << g << endl
         << "c = " << c << endl
         << "d = " << d << endl;



    int length = len(in);
    char x;
    for(int i=0; i<length; i++) {
        in.read(&x,sizeof(x));
        ll k = rand() % (p-1);
        ll r = fPow(g,k,p);
        ll e = (x * fPow(d,k,p)) % p;
        out << r << " " << e << " ";
    }

    cout << endl << "Done.." << endl;

    in.close();
    out.close();
}

void eg_decrypt(string filename) {
    vector<string> names = split(filename,'.');
    vector<string> rrr = split(names[1],'_');

    ifstream in, setting;
    in.open(filename.c_str(), ios_base::in);
    if(!in.is_open()){
        cout << "Can't open " << filename << endl;
        return;
    }
    setting.open((names[0] + ".setting").c_str(), ios_base::binary);
    if(!setting.is_open()){
        cout << "Can't open settings " << names[0] << ".setting" << endl;
        return;
    }

    ofstream out;
    out.open((names[0] + "_decr." + rrr[1]).c_str(), ios_base::binary);

    ll p,c;
    setting >> p >> c;
    setting.close();

    cout << "P = " << p << endl
         << "c = " << c << endl;

    ll r,e;
    while(in >> r >> e) {
        ll m = (e * fPow(r,p-1-c,p)) % p;
        out << (char)m;
    }
    in.close();
    out.close();
    cout << "Done.." << endl;

}

void rsa_encrypt(string filename) {
    vector<string> names = split(filename,'.');

    ifstream in;
    in.open(filename.c_str(), ios_base::binary);
    if(!in.is_open()){
        cout << "Can't open " << filename << endl;
        return;
    }

    ofstream out, setting;
    out.open((names[0] + ".code_" + names[1]).c_str(), ios_base::binary);
    setting.open((names[0] + ".setting").c_str(), ios_base::binary);

    ll P, Q, N, phi, c, d;

    P = generate_num_rsa(10000);
    Q = generate_num_rsa(10000);

    N = P*Q;
    phi = (P-1) * (Q-1);

    ll* temp;
    int i = 0;
    do{
        if(i) delete [] temp;
        d = rand() % phi;
        temp = evklid(phi,d);
        i++;
        if(temp[0] != 1) continue;

        c = temp[2];
        if(c < 0) c += phi;
    }while(d >= phi || c >= phi);

    delete [] temp;

    setting << N <<" "<< c;
    setting.close();

    cout << "P = " << P << endl
         << "Q = " << Q << endl
         << "N = " << N << endl
         << "phi = " << phi << endl
         << "c = " << c << endl
         << "d = " << d << endl;



    int length = len(in);
    char x;
    for(int i=0; i<length; i++) {
        in.read(&x,sizeof(x));
        out << fPow(x,d,N) << " ";
    }

    cout << "Done.." << endl;

    in.close();
    out.close();
}

void rsa_decrypt(string filename) {
    vector<string> names = split(filename,'.');
    vector<string> rrr = split(names[1],'_');

    ifstream in, setting;
    in.open(filename.c_str(), ios_base::in);
    if(!in.is_open()){
        cout << "Can't open " << filename << endl;
        return;
    }
    setting.open((names[0] + ".setting").c_str(), ios_base::binary);
    if(!setting.is_open()){
        cout << "Can't open settings " << names[0] << ".setting" << endl;
        return;
    }

    ofstream out;
    out.open((names[0] + "_decr." + rrr[1]).c_str(), ios_base::binary);

    ll N,c;
    setting >> N >> c;
    setting.close();

    cout << "N = " << N << endl
         << "c = " << c << endl;

    ll e;
    while(in >> e) {
        ll m = fPow(e,c,N);
        out << (char)m;
    }
    in.close();
    out.close();
    cout << "Done.." << endl;

}

void vern_encrypt(string filename) {
    vector<string> names = split(filename,'.');

    ifstream in;
    in.open(filename.c_str(), ios_base::binary);
    if(!in.is_open()){
        cout << "Can't open " << filename << endl;
        return;
    }

    ofstream out, key;
    out.open((names[0] + ".code_" + names[1]).c_str(), ios_base::binary);
    key.open((names[0] + ".code_key").c_str(), ios_base::binary);

    int length = len(in);
    char x;
    for(int i=0; i<length; i++) {
        in.read(&x,sizeof(x));
        char k = 1 + rand() % 253;
        char c = x ^ k;
        key << k;
        out << c;
    }

    cout << "Done.." << endl;

    in.close();
    out.close();
    key.close();
}

void vern_decrypt(string filename) {
    vector<string> names = split(filename,'.');
    vector<string> rrr = split(names[1],'_');

    ifstream in, key;
    in.open(filename.c_str(), ios_base::in);
    if(!in.is_open()){
        cout << "Can't open " << filename << endl;
        return;
    }
    key.open((names[0] + ".code_key").c_str(), ios_base::binary);
    if(!key.is_open()){
        cout << "Can't open key " << names[0] << ".code_key" << endl;
        return;
    }

    ofstream out;
    out.open((names[0] + "_decr." + rrr[1]).c_str(), ios_base::binary);

    int length = len(in);
    char x,k;
    for(int i=0; i<length; i++) {
        in.read(&x, sizeof(x));
        key.read(&k, sizeof(k));
        out << (char)(x ^ k);
    }

    in.close();
    out.close();
    key.close();
    cout << "Done.." << endl;
}

void shamir(long long m){
    ll Ca, Cb, Da, Db, p, base = 100000000;
    ll* temp;

    do{
        p = base + rand() % (base * 10);
    }while(!isPrime(p));

    int i = 0;
    do {
        if(i) delete [] temp;
        Ca = generate_num(p-1);
        temp = evklid(p-1, Ca);
        i++;
    }while(Ca > p - 1 && temp[0] != 1);

    Da = temp[2];
    if (Da < 0) Da += p-1;
    delete [] temp;

    i = 0;
    do {
        if(i) delete [] temp;
        Cb = generate_num(p-1);
        temp = evklid(p-1, Cb);
        i++;
    }while(Cb > p - 1 && temp[0] != 1);

    Db = temp[2];
    if (Db < 0) Db += p-1;
    delete [] temp;

    m = fPow(m, Ca, p); cout << "X1: " << m << endl;
    m = fPow(m, Cb, p); cout << "X2: " << m << endl;
    m = fPow(m, Da, p); cout << "X3: " << m << endl;
    m = fPow(m, Db, p); cout << "X4: " << m << endl;
}
