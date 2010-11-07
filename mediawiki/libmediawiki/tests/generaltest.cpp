#ifndef TEST_GENERAL_H
#define TEST_GENERAL_H

#include <QObject>
#include <QtTest/QtTest>
#include <KJob>


#include "mediawiki.h"
#include "general.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::General;

class GeneralTest : public QObject
{
    Q_OBJECT

public slots:

void generalHandle(KJob* job) {
    generalCount++;
    generalResults = ((General*)job)->getResult();
}
private slots:

void initTestCase()
{
    generalCount = 0;
    this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
    this->m_server = new FakeServer;
    this->request = "?format=xml&action=query&meta=siteinfo&siprop=general";
}
void generalTestConnectTrue()
{
    generalCount = 0;
    QString senario("<api><query><general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" sitename=\"Wikipedia\" generator=\"MediaWiki 1.16wmf4\" phpversion=\"5.2.4-2ubuntu5.12wm1\" phpsapi=\"apache2handler\" dbtype=\"mysql\" dbversion=\"5.1.46-facebook-r3489-log\" rev=\"75268\" case=\"first-letter\" rights=\"Creative Commons Attribution-Share Alike 3.0 Unported\" lang=\"en\" fallback8bitEncoding=\"windows-1252\" writeapi=\"\" timezone=\"UTC\" timeoffset=\"0\" articlepath=\"/wiki/$1\" scriptpath=\"/w\" script=\"/w/index.php\" variantarticlepath=\"\" server=\"http://en.wikipedia.org\" wikiid=\"enwiki\" time=\"2010-10-24T19:53:13Z\"/></query></api>");
    m_server->addScenario(senario);
    m_server->startAndWait();

    General general(*m_mediaWiki);

    connect(&general, SIGNAL(result(KJob* )),this, SLOT(generalHandle(KJob*)));
    general.exec();
    FakeServer::Request serverrequest = m_server->getRequest()[0];
    QCOMPARE(this->generalCount, 1);
    QCOMPARE(serverrequest.type, QString("GET"));
    QCOMPARE(serverrequest.value, this->request);
    QVERIFY(general.error() == General::NoError);

}
void generalTestAttribute()
{
    generalCount = 0;
    QString senario("<api><query><general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" sitename=\"Wikipedia\" generator=\"MediaWiki 1.16wmf4\" phpversion=\"5.2.4-2ubuntu5.12wm1\" phpsapi=\"apache2handler\" dbtype=\"mysql\" dbversion=\"5.1.46-facebook-r3489-log\" rev=\"75268\" case=\"first-letter\" rights=\"Creative Commons Attribution-Share Alike 3.0 Unported\" lang=\"en\" fallback8bitEncoding=\"windows-1252\" writeapi=\"\" timezone=\"UTC\" timeoffset=\"0\" articlepath=\"/wiki/$1\" scriptpath=\"/w\" script=\"/w/index.php\" variantarticlepath=\"\" server=\"http://en.wikipedia.org\" wikiid=\"enwiki\" time=\"2010-10-24T19:53:13Z\"/></query></api>");
    m_server->addScenario(senario);
    m_server->startAndWait();

    General general(*m_mediaWiki);

    connect(&general, SIGNAL(result(KJob* )),this, SLOT(generalHandle(KJob*)));
    general.exec();
    General::Result result = general.getResult();
    FakeServer::Request serverrequest = m_server->getRequest()[0];
    QCOMPARE(this->generalCount, 1);
    QCOMPARE(serverrequest.type, QString("GET"));
    QCOMPARE(serverrequest.value, this->request);
    QVERIFY(general.error() == General::NoError);
    QVERIFY(result.mainpage             == QString("Main Page"));
    QVERIFY(result.base                 == QString("http://en.wikipedia.org/wiki/Main_Page"));
    QVERIFY(result.sitename             == QString("Wikipedia"));
    QVERIFY(result.generator            == QString("MediaWiki 1.16wmf4"));
    QVERIFY(result.phpversion           == QString("5.2.4-2ubuntu5.12wm1"));
    QVERIFY(result.phpsapi              == QString("apache2handler"));
    QVERIFY(result.dbtype               == QString("mysql"));
    QVERIFY(result.dbversion            == QString("5.1.46-facebook-r3489-log"));
    QVERIFY(result.rev                  == QString("75268"));
    QVERIFY(result.cas                  == QString("first-letter"));
    QVERIFY(result.rights               == QString("Creative Commons Attribution-Share Alike 3.0 Unported"));
    QVERIFY(result.lang                 == QString("en"));
    QVERIFY(result.fallback8bitencoding == QString("windows-1252"));
    QVERIFY(result.writeapi             == QString(""));
    QVERIFY(result.timezone             == QString("UTC"));
    QVERIFY(result.timeoffset           == QString("0"));
    QVERIFY(result.articlepath          == QString("/wiki/$1"));
    QVERIFY(result.scriptpath           == QString("/w"));
    QVERIFY(result.script               == QString("/w/index.php"));
    QVERIFY(result.variantarticlepath   == QString(""));
    QVERIFY(result.server               == QString("http://en.wikipedia.org"));
    QVERIFY(result.wikiid               == QString("enwiki"));
    QVERIFY(result.time                 == QString("2010-10-24T19:53:13Z"));
}
void generalTestConnectFalseXML()
{
    generalCount = 0;
    QString senario("<api><query<general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" sitename=\"Wikipedia\" generator=\"MediaWiki 1.16wmf4\" phpversion=\"5.2.4-2ubuntu5.12wm1\" phpsapi=\"apache2handler\" dbtype=\"mysql\" dbversion=\"5.1.46-facebook-r3489-log\" rev=\"75268\" case=\"first-letter\" rights=\"Creative Commons Attribution-Share Alike 3.0 Unported\" lang=\"en\" fallback8bitEncoding=\"windows-1252\" writeapi=\"\" timezone=\"UTC\" timeoffset=\"0\" articlepath=\"/wiki/$1\" scriptpath=\"/w\" script=\"/w/index.php\" variantarticlepath=\"\" server=\"http://en.wikipedia.org\" wikiid=\"enwiki\" time=\"2010-10-24T19:53:13Z\"/>");
    m_server->addScenario(senario);
    m_server->startAndWait();

    General general(*m_mediaWiki);

    connect(&general, SIGNAL(result(KJob* )),this, SLOT(generalHandle(KJob*)));
    general.exec();
    FakeServer::Request serverrequest = m_server->getRequest()[0];
    QCOMPARE(this->generalCount, 1);
    QCOMPARE(serverrequest.type, QString("GET"));
    QCOMPARE(serverrequest.value, this->request);
    QVERIFY(general.error() == General::falsexml);

}
void generalTestConnectAbort()
{
    generalCount = 0;
    General general(mediawiki::MediaWiki(QUrl("http://127.0.0.2:910")));

    connect(&general, SIGNAL(result(KJob* )),this, SLOT(generalHandle(KJob*)));
    general.exec();
    QCOMPARE(this->generalCount, 1);
    QVERIFY(general.error() == General::connectionAbort);

}
void generalTestErrortIncludeAllDenied()
{
    generalCount = 0;
    QString senario("<api><error code=\"includeAllDenied\" info=\"\"/></api>");
    m_server->addScenario(senario);
    m_server->startAndWait();

    General general(*m_mediaWiki);

    connect(&general, SIGNAL(result(KJob* )),this, SLOT(generalHandle(KJob*)));
    general.exec();
    FakeServer::Request serverrequest = m_server->getRequest()[0];
    QCOMPARE(this->generalCount, 1);
    QCOMPARE(serverrequest.type, QString("GET"));
    QCOMPARE(serverrequest.value, this->request);
    QVERIFY(general.error() == General::includeAllDenied);

}
void cleanupTestCase()
{
    delete this->m_mediaWiki;
    delete this->m_server;
}
private:
    int generalCount;
    General::Result generalResults;
    QString request;
    MediaWiki* m_mediaWiki;
    FakeServer* m_server;
};
QTEST_MAIN(GeneralTest);
#include "generaltest.moc"

#endif // TEST_GENERAL_H

