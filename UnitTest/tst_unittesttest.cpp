#include <QtCore/QString>
#include <QtTest/QtTest>

class UnitTestTest : public QObject
{
    Q_OBJECT
    
public:
    UnitTestTest();
    
private Q_SLOTS:
    void testCase1();
    void testCase1_data();
};

UnitTestTest::UnitTestTest()
{
}

void UnitTestTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void UnitTestTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(UnitTestTest)

#include "tst_unittesttest.moc"
