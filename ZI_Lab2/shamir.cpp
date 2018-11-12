#include "shamir.h"

Shamir::Shamir()
{
}

QString Shamir::encode(QString txt, long long p)
{
    QByteArray str = txt.toAscii();
//    qDebug() << "PreEncoded: " << str;

	do{
		Ca = 1 + rand() % (p - 1);
	}while(hlib.gcd(Ca, p) != 1);

	QPair< long long, QPair< long long, long long > > evk = hlib.evklid(p, Ca);
	Cb = evk.second.second;
	if(Cb < 0) Cb += p;
	if((Ca * Cb) % p != 1){
		qDebug() << "Something wrong!";
        return QString();
	}

    QByteArray result("");

//    while(!str.isEmpty()){
//        char c = str.at(0);
//        str.remove(0, 1);
//        c = hlib.fPow(c, Ca, p);
//        c = hlib.fPow(c, Cb, p);
//        result.append(c);
//    }

    while(!str.isEmpty()){
        long long c = str.at(0); // 1 out of 8
        str.remove(0, 1);
        int i = 1;
        while(i < 8 && str.size() > 0){
            c = (c << 1) ^ str.at(0);
            str.remove(0, 1);
            ++i;
        }

        qDebug() << sizeof(long long) << sizeof(char);
        c = hlib.fPow(c, Ca, p);
        c = hlib.fPow(c, Cb, p);

        while(c){
            char ch = c & 1;
            c = c >> 1;
            result.append(ch);
        }
    }

    qDebug() << "Encoded: " << result;
    return QString::fromAscii(result.data());
}

QString Shamir::decode(QString txt, long long p)
{
    QByteArray str = txt.toAscii();
//    qDebug() << "PreDecoded: " << str;

    do{
        Da = 1 + rand() % (p - 1);
    }while(hlib.gcd(Da, p) != 1);

    QPair< long long, QPair< long long, long long > > evk = hlib.evklid(p, Da);
    Db = evk.second.second;
    if(Db < 0) Db += p;
    if((Da * Db) % p != 1){
        qDebug() << "Something wrong!";
        return QString();
    }

    QByteArray result;

    while(!str.isEmpty()){
        char c = str.at(0);
        str.remove(0, 1);
        c = hlib.fPow(c, Da, p);
        c = hlib.fPow(c, Db, p);
        result.append(c);
    }

    qDebug() << "Decoded: " << result;
    return QString::fromAscii(result.data());
}

long long Shamir::test(long long m)
{
    long long p;
    do{
        int n = 1000000;
        p = n + rand() % (n * 10);
    }while(!hlib.isPrime(p));

    //Ca, Da
    do{
        Ca = 3 + rand() % (p - 4);
    }while(hlib.gcd(Ca, p - 1) != 1);

    QPair< long long, QPair< long long, long long > > evk = hlib.evklid(p - 1, Ca);
    Da = evk.second.second;
    if(Da < 0) Da += p;

    if((Ca * Da) % (p - 1) != 1){
        qDebug() << "Something wrong!";
        qDebug() << "Ca = " << Ca;
        qDebug() << "Da = " << Da;
        qDebug() << "(Ca * Da) % (p - 1) = " << (Ca * Da) % (p - 1);
        return 0;
    }

    //Cb, Db
    do{
        Cb = 3 + rand() % (p - 4);
    }while(hlib.gcd(Cb, p - 1) != 1);

    QPair< long long, QPair< long long, long long > > evk2 = hlib.evklid(p - 1, Cb);
    Db = evk2.second.second;
    if(Db < 0) Db += p;

    if((Cb * Db) % (p - 1) != 1){
        qDebug() << "Something wrong!";
        qDebug() << "Cb = " << Ca;
        qDebug() << "Db = " << Da;
        qDebug() << "(Cb * Db) % (p - 1) = " << (Cb * Db) % (p - 1);
        return 0;
    }

    qDebug() << "0) " << m;
    m = hlib.fPow(m, Ca, p);
    qDebug() << "1) " << m;
    m = hlib.fPow(m, Cb, p);
    qDebug() << "2) " << m;
    m = hlib.fPow(m, Da, p);
    qDebug() << "3) " << m;
    m = hlib.fPow(m, Db, p);
    qDebug() << "4) " << m;

    return m;
}




