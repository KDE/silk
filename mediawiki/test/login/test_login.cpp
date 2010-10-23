#include "test_login.h"
#include <QDebug>


void test_login::initTestCase()
{
    this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
    this->m_server = new Server(0);
}

void test_login::loginConnect()
{
    QString senario("<?xml version=\"1.0\" encoding=\"utf-8\"?><api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>");
    m_server->addScenario(senario);
    m_server->startAndWait();
    this->m_login = new Login(*m_mediaWiki, "alexTest", "test");
    QSignalSpy spy(m_login, SIGNAL(finishedLogin(bool)));
    QEventLoop loop;
    connect(m_login, SIGNAL(finishedLogin(bool)),&loop,SLOT(quit()));
    loop.exec();
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.takeFirst().at(0).toBool() == true);
}


void test_login::cleanupTestCase()
{
    delete this->m_mediaWiki;
    delete this->m_server;
}

QTEST_MAIN(test_login)


