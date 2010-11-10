/*
 *   Copyright 2010 by Guillaume Hormiere <hormiere.guillaume@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef TEST_LOGIN_H
#define TEST_LOGIN_H

#include <QObject>
#include <QtTest/QtTest>
#include <KJob>

#include "mediawiki.h"
#include "login.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::Login;

class LoginTest : public QObject
{
    Q_OBJECT

public slots:

    void loginHandle(KJob* job) {
        loginCount++;
        loginResults = ((Login*)job)->getResults();
    }

private slots:

    void initTestCase()
    {
        loginCount = 0;
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
        this->m_server = new FakeServer;
        this->request = "?format=xml&action=login&lgname=alexTest&lgpassword=test";
    }

    void loginTestConnectTrue()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->addScenario(senario, cookie);
        senario = "<api><login result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /></api>";
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QVERIFY(login.error() == Login::NoError);
    }

    void loginTestAttribute()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->addScenario(senario, cookie);
        senario = "<api><login result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /></api>";
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        Login::Result result = login.getResults();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QVERIFY(login.error()      == Login::NoError);
        QVERIFY(result.lgname      == QString("alexTest"));
        QVERIFY(result.lgpassword  == QString("test"));
        QVERIFY(result.lgsessionid == QString("17ab96bd8ffbe8ca58a78657a918558e"));
        QVERIFY(result.lgtoken     == QString("b5780b6e2f27e20b450921d9461010b4"));
    }

    void loginTestNoName()
    {
        loginCount = 0;
        QString senario("<api><error code=\"NoName\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::NoName);
    }

    void loginTestIllegal()
    {
        loginCount = 0;
        QString senario("<api><error code=\"Illegal\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::Illegal);
    }

    void loginTestNotExists()
    {
        loginCount = 0;
        QString senario("<api><error code=\"NotExists\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::NotExists);
    }

    void loginTestEmptyPass()
    {
        loginCount = 0;
        QString senario("<api><error code=\"EmptyPass\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::EmptyPass);
    }

    void loginTestWrongPass()
    {
        loginCount = 0;
        QString senario("<api><error code=\"WrongPass\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::WrongPass);
    }

    void loginTestWrongPluginPass()
    {
        loginCount = 0;
        QString senario("<api><error code=\"WrongPluginPass\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::WrongPluginPass);
    }

    void loginTestCreateBlocked()
    {
        loginCount = 0;
        QString senario("<api><error code=\"CreateBlocked\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::CreateBlocked);
    }

    void loginTestThrottled()
    {
        loginCount = 0;
        QString senario("<api><error code=\"Throttled\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::Throttled);
    }

    void loginTestBlocked()
    {
        loginCount = 0;
        QString senario("<api><error code=\"Blocked\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::Blocked);
    }

    void loginTestNeedToken()
    {
        loginCount = 0;
        QString senario("<api><error code=\"NeedToken\" info=\"\" /> </api>" );
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        //qDebug()<<"error"<<login.error();
        QCOMPARE(login.error(), (int)Login::NeedToken);
    }


    void loginTestConnectFalseXMLToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->addScenario(senario, cookie);
        senario = "<api><login result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"</api>";
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        Login::Result result = login.getResults();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QVERIFY(login.error() == Login::Falsexml);

    }

    void loginTestConnectFalseXMLLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->addScenario(senario, cookie);
        senario = "<api><login result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /></api>";
        m_server->addScenario(senario);
        m_server->startAndWait();

        Login login(*m_mediaWiki, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        Login::Result result = login.getResults();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QVERIFY(login.error() == Login::Falsexml);

    }

    void loginTestConnectAbort()
    {
        loginCount = 0;
        Login login(mediawiki::MediaWiki(QUrl("http://127.0.0.2:910")), "alexTest", "test");

        connect(&login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
        login.exec();
        QCOMPARE(this->loginCount, 1);
        QVERIFY(login.error() == Login::ConnectionAbort);

    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
        delete this->m_server;
    }

private:

    int loginCount;
    Login::Result loginResults;
    QString request;
    MediaWiki* m_mediaWiki;
    FakeServer* m_server;
};


QTEST_MAIN(LoginTest);
#include "logintest.moc"
#endif // TEST_LOGIN_H
