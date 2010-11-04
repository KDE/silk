#ifndef TEST_GENERAL_H
#define TEST_GENERAL_H

#include <QObject>
#include <QtTest/QtTest>
#include "mediawiki.h"
#include "general.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::General;

class GeneralTest : public QObject
{
    Q_OBJECT
private slots:

    void initTestCase()
{
    this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
    this->m_server = new FakeServer(0);
}
    void generalTestConnectTrue()
{
    QString senario("<api><query><general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" sitename=\"Wikipedia\" generator=\"MediaWiki 1.16wmf4\" phpversion=\"5.2.4-2ubuntu5.12wm1\" phpsapi=\"apache2handler\" dbtype=\"mysql\" dbversion=\"5.1.46-facebook-r3489-log\" rev=\"75268\" case=\"first-letter\" rights=\"Creative Commons Attribution-Share Alike 3.0 Unported\" lang=\"en\" fallback8bitEncoding=\"windows-1252\" writeapi=\"\" timezone=\"UTC\" timeoffset=\"0\" articlepath=\"/wiki/$1\" scriptpath=\"/w\" script=\"/w/index.php\" variantarticlepath=\"\" server=\"http://en.wikipedia.org\" wikiid=\"enwiki\" time=\"2010-10-24T19:53:13Z\"/></query></api>");
    m_server->addScenario(senario);
    m_server->startAndWait();
    this->m_general = new General(*m_mediaWiki);
    QSignalSpy spy(m_general, SIGNAL(finished(bool)));
    QEventLoop loop;
    connect(m_general, SIGNAL(finished(bool)),&loop,SLOT(quit()));
    loop.exec();
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.takeFirst().at(0).toBool()  == true);

}
    void generalTestAttribute()
{
    QString senario("<api><query><general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" sitename=\"Wikipedia\" generator=\"MediaWiki 1.16wmf4\" phpversion=\"5.2.4-2ubuntu5.12wm1\" phpsapi=\"apache2handler\" dbtype=\"mysql\" dbversion=\"5.1.46-facebook-r3489-log\" rev=\"75268\" case=\"first-letter\" rights=\"Creative Commons Attribution-Share Alike 3.0 Unported\" lang=\"en\" fallback8bitEncoding=\"windows-1252\" writeapi=\"\" timezone=\"UTC\" timeoffset=\"0\" articlepath=\"/wiki/$1\" scriptpath=\"/w\" script=\"/w/index.php\" variantarticlepath=\"\" server=\"http://en.wikipedia.org\" wikiid=\"enwiki\" time=\"2010-10-24T19:53:13Z\"/></query></api>");
    m_server->addScenario(senario);
    m_server->startAndWait();
    this->m_general = new General(*m_mediaWiki);
    QSignalSpy spy(m_general, SIGNAL(finished(bool)));
    QEventLoop loop;
    connect(m_general, SIGNAL(finished(bool)),&loop,SLOT(quit()));
    loop.exec();
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.takeFirst().at(0).toBool()  == true);

    QVERIFY(this->m_general->getMainpage()              == QString("Main Page"));
    QVERIFY(this->m_general->getBase()                  == QString("http://en.wikipedia.org/wiki/Main_Page"));
    QVERIFY(this->m_general->getSitename()              == QString("Wikipedia"));
    QVERIFY(this->m_general->getGenerator()             == QString("MediaWiki 1.16wmf4"));
    QVERIFY(this->m_general->getPhpversion()            == QString("5.2.4-2ubuntu5.12wm1"));
    QVERIFY(this->m_general->getPhpsapi()               == QString("apache2handler"));
    QVERIFY(this->m_general->getDbtype()                == QString("mysql"));
    QVERIFY(this->m_general->getDbversion()             == QString("5.1.46-facebook-r3489-log"));
    QVERIFY(this->m_general->getRev()                   == QString("75268"));
    QVERIFY(this->m_general->getCase()                  == QString("first-letter"));
    QVERIFY(this->m_general->getRights()                == QString("Creative Commons Attribution-Share Alike 3.0 Unported"));
    QVERIFY(this->m_general->getLang()                  == QString("en"));
    QVERIFY(this->m_general->getFallback8bitencoding()  == QString("windows-1252"));
    QVERIFY(this->m_general->getWriteapi()              == QString(""));
    QVERIFY(this->m_general->getTimezone()              == QString("UTC"));
    QVERIFY(this->m_general->getTimeoffset()            == QString("0"));
    QVERIFY(this->m_general->getArticlepath()           == QString("/wiki/$1"));
    QVERIFY(this->m_general->getScriptpath()            == QString("/w"));
    QVERIFY(this->m_general->getScript()                == QString("/w/index.php"));
    QVERIFY(this->m_general->getVariantarticlepath()    == QString(""));
    QVERIFY(this->m_general->getServer()                == QString("http://en.wikipedia.org"));
    QVERIFY(this->m_general->getWikiid()                == QString("enwiki"));
    QVERIFY(this->m_general->getTime()                  == QString("2010-10-24T19:53:13Z"));
}
    void generalTestConnectFalseXML()
{
    QString senario("<api><query<general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" sitename=\"Wikipedia\" generator=\"MediaWiki 1.16wmf4\" phpversion=\"5.2.4-2ubuntu5.12wm1\" phpsapi=\"apache2handler\" dbtype=\"mysql\" dbversion=\"5.1.46-facebook-r3489-log\" rev=\"75268\" case=\"first-letter\" rights=\"Creative Commons Attribution-Share Alike 3.0 Unported\" lang=\"en\" fallback8bitEncoding=\"windows-1252\" writeapi=\"\" timezone=\"UTC\" timeoffset=\"0\" articlepath=\"/wiki/$1\" scriptpath=\"/w\" script=\"/w/index.php\" variantarticlepath=\"\" server=\"http://en.wikipedia.org\" wikiid=\"enwiki\" time=\"2010-10-24T19:53:13Z\"/>");
    m_server->addScenario(senario);
    m_server->startAndWait();
    this->m_general = new General(*m_mediaWiki);
    QSignalSpy spy(m_general, SIGNAL(finished(bool)));
    QEventLoop loop;
    connect(m_general, SIGNAL(finished(bool)),&loop,SLOT(quit()));
    loop.exec();
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.takeFirst().at(0).toBool()  == false);

}
    void generalTestConnectFalseXMLCuted()
{
    QString senario("<api><query<general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" ");
    m_server->addScenario(senario);
    m_server->startAndWait();
    this->m_general = new General(*m_mediaWiki);
    QSignalSpy spy(m_general, SIGNAL(finished(bool)));
    QEventLoop loop;
    connect(m_general, SIGNAL(finished(bool)),&loop,SLOT(quit()));
    loop.exec();
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.takeFirst().at(0).toBool()  == false);
}
    void cleanupTestCase()
{
    delete this->m_mediaWiki;
    delete this->m_server;
}
private:
    General* m_general;
    MediaWiki* m_mediaWiki;
    FakeServer* m_server;
};
QTEST_MAIN(GeneralTest);
#include "generaltest.moc"

#endif // TEST_GENERAL_H

