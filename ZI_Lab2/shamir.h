#ifndef SHAMIR_H
#define SHAMIR_H

#include "helplib.h"
#include <QPair>
#include <QDebug>
#include <QByteArray>


class Shamir
{
public:
	Shamir();

	helpLib hlib;

    //QString text;
    long long Ca, Cb, Da, Db;

    QString encode(QString txt, long long p);
    QString decode(QString txt, long long p);
    long long test(long long m);
};

#endif // SHAMIR_H
