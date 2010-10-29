#ifndef TEST_LOGIN_H
#define TEST_LOGIN_H

#include <QObject>
#include <QtTest/QtTest>
#include "mediawiki/mediawiki.h"
#include "mediawiki/login.h"
#include "mediawiki/tests/fake_server/server.h"

class test_login : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
        this->m_server = new Server(0);
    }

    void loginConnect()
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


    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
        delete this->m_server;
    }
private:
    Login* m_login;
    MediaWiki* m_mediaWiki;
    Server* m_server;
};

#endif // TEST_LOGIN_H



QTEST_MAIN(test_login)
#include "test_login.moc"


