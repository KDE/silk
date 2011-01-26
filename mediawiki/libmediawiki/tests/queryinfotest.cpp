/*
 *   Copyright 2010 by Alexandre Mendes <alex.mendes1988@gmail.com>
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

#ifndef TEST_QUERYINFO_H
#define TEST_QUERYINFO_H

#include <QObject>
#include <QtTest/QtTest>
#include <KJob>

#include "mediawiki.h"
#include "queryinfo.h"
#include "page.h"
#include "protection.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::QueryInfo;
using mediawiki::Page;
using mediawiki::Protection;

Q_DECLARE_METATYPE(Page)
Q_DECLARE_METATYPE(Protection)
Q_DECLARE_METATYPE(QueryInfo*)
Q_DECLARE_METATYPE( QVector <Protection> )


void debugPages(Page p)
{
    qDebug() << p.pageId();
    qDebug() << p.pageTitle();
    qDebug() << p.pageNs();
    qDebug() << p.pageLastRevId();
    qDebug() << p.pageCounter();
    qDebug() << p.pageLength();
    qDebug() << p.pageEditToken();
    qDebug() << p.pageTalkid();
    qDebug() << p.pageFullurl();
    qDebug() << p.pageEditurl();
    qDebug() << p.pageReadable();
    qDebug() << p.pagePreload();
    qDebug() << p.pageTouched();
    qDebug() << p.pageStarttimestamp();
}

void debugProtection(QVector <Protection> p)
{
    foreach(Protection pr, p)
    {
        qDebug() << pr.Type();
        qDebug() << pr.Level();
        qDebug() << pr.Expiry();
        qDebug() << pr.Source();
    }
}

class QueryInfoTest : public QObject
{
    Q_OBJECT

public slots:

    void queryInfoHandlePages(const Page& page) {
        ++queryInfoCount;
        queryInfoResultsPage = page;
    }

    void queryInfoHandleProtection(const QVector<Protection> & protection) {
        ++queryInfoCount;
        queryInfoResultsProtections = protection;
    }

private slots:

    void initTestCase() {
        queryInfoCount = 0;
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
    }

    void constructQuery()
    {
        QFETCH(QString, request);
        QFETCH(QueryInfo*, job);

        queryInfoCount = 0;
        FakeServer fakeserver;
        fakeserver.startAndWait();

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request requestServeur = requests[0];
        QCOMPARE(requestServeur.agent, m_mediaWiki->userAgent());
        QCOMPARE(requestServeur.type, QString("GET"));
        QCOMPARE(requestServeur.value, request);

    }
    void constructQuery_data()
    {
        QTest::addColumn<QString>("request");
        QTest::addColumn<QueryInfo*>("job");

        QueryInfo *j1 = new QueryInfo(*m_mediaWiki);
        j1->setPageName("API");

        QTest::newRow("Name pages")
                << QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&titles=API")
                << j1;

        QueryInfo *j2 = new QueryInfo(*m_mediaWiki);
        j2->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );

        QTest::newRow("Token")
                << QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&intoken=cecded1f35005d22904a35cc7b736e18+\\")
                << j2;

        QueryInfo *j3 = new QueryInfo(*m_mediaWiki);
        j3->setPageId(25255);

        QTest::newRow("Page Id")
                << QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&pageids=25255")
                << j3;

        QueryInfo *j4 = new QueryInfo(*m_mediaWiki);
        j4->setRevisionId(44545);

        QTest::newRow("Revision Id")
                << QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&revids=44545")
                << j4;


    }

    void parseData()
    {
        QFETCH(QString,scenario);
        QFETCH(Page ,page);
        QFETCH(QVector<Protection> ,protections);

        QueryInfo job(*m_mediaWiki);
        queryInfoCount = 0;
        FakeServer fakeserver;
        fakeserver.addScenario(scenario);
        fakeserver.startAndWait();
        connect(&job, SIGNAL(page(const Page &)),this,SLOT(queryInfoHandlePages(const Page&)));
        connect(&job, SIGNAL(protection(const QVector <Protection> &)),this,SLOT(queryInfoHandleProtection(const QVector<Protection> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        QCOMPARE(queryInfoCount, 2);
        QCOMPARE(queryInfoResultsPage, page);
        QCOMPARE(queryInfoResultsProtections, protections);
        QVERIFY(fakeserver.isAllScenarioDone());



    }
    void parseData_data()
    {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn< Page >("page");
        QTest::addColumn< QVector<Protection> >("protections");

        Protection pr1;
        pr1.setType("edit");
        pr1.setLevel("sysop");
        pr1.setExpiry("infinity");
        pr1.setSource("");

        Protection pr2;
        pr2.setType("move");
        pr2.setLevel("sysop");
        pr2.setExpiry("infinity");
        pr2.setSource("");

        Page page;
        page.setPageId(27697087);
        page.setTitle("API");
        page.setNs(0);
        page.setTouched( QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'") );
        page.setLastRevId(367741756);
        page.setCounter(0);
        page.setLength(70);
        page.setStarttimestamp(QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
        page.setEditToken("+\\");
        page.setTalkid(5477418);
        page.setFullurl(QUrl("http://en.wikipedia.org/wiki/API"));
        page.setEditurl(QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit"));
        page.setReadable("");
        page.setPreload("");

        QTest::newRow("No protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
                << page
                << QVector<Protection>();

        QTest::newRow("One pages and one protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << page
                << (QVector<Protection>() << pr1);

        QTest::newRow("One pages and two protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << page
                << (QVector<Protection>() << pr1 << pr2);

    }

    void error()
    {

    }
    void error_data()
    {

    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
    }
private:

    int queryInfoCount;
    Page queryInfoResultsPage;
    QVector <Protection> queryInfoResultsProtections;
    MediaWiki* m_mediaWiki;
};


QTEST_MAIN(QueryInfoTest);
#include "queryinfotest.moc"
#endif // TEST_QUERYINFO_H

//    void testRevidsResult() {
//        QFETCH(QString, scenario);
//        QFETCH(int, error);
//        QFETCH(Page, results);

//        FakeServer fakeserver;
//        fakeserver.setScenario(scenario);
//        fakeserver.startAndWait();

//        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
//        QueryInfo * job = new QueryInfo(mediawiki);
//        job->setRevisionId(367741564);
//        job->setToken("edit");

//        connect(job, SIGNAL(page(const Page &)), this, SLOT(queryInfoHandle(const Page&)));

//        job->exec();

//        QList<FakeServer::Request> requests = fakeserver.getRequest();
//        QCOMPARE(requests.size(), 1);

//        FakeServer::Request request = requests[0];
//        QCOMPARE(request.agent, mediawiki.userAgent());
//        QCOMPARE(request.type, QString("GET"));
//        QCOMPARE(request.value, QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&intoken=edit&revids=367741564"));

//        QCOMPARE(job->error(), error);
//        QCOMPARE(queryInfoCount, 1);
//        QCOMPARE(queryInfoResults, results);
//        QVERIFY(fakeserver.isAllScenarioDone());
//    }

//    void testRevidsResult_data() {
//        QTest::addColumn<QString>("scenario");
//        QTest::addColumn<int>("error");
//        QTest::addColumn< Page >("results");

//        Protection protections;
//        protection.setType("edit");
//        protection.setLevel("sysop");
//        protection.setExpiry("infinity");
//        protection.setSource("");

//        Page page;
//        page.setPageId(27697087);
//        page.setTitle("API");
//        page.pageNs(0);
//        page.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page.setLastRevId(367741756);
//        page.setCounter(0);
//        page.setLength(70);
//        page.setStarttimestamp(QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
//        page.setEditToken("+\\");
//        page.setTalkid(5477418);
//        page.setFullurl(QUrl("http://en.wikipedia.org/wiki/API"));
//        page.setEditurl(QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit"));
//        page.setReadable("");
//        page.setPreload("");


//        QTest::newRow("Revids: No protection")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        page.setProtections(protections);
//        QTest::newRow("Revids: One page with two protections")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        QTest::newRow("Revids: Two pages with two protections")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        QTest::newRow("Revids: Two pages with no protection for the first page and two protections for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        QTest::newRow("Revids: Two pages with two protections for the first page and no protection for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        page.setProtections(protections);
//        QTest::newRow("Revids: Two pages with one protection for the first page and two protections for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        QTest::newRow("Revids: Two pages with two protections for the first page and one protection for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;
//    }

//    void testTitlesResult() {
//        QFETCH(QString, scenario);
//        QFETCH(int, error);
//        QFETCH(Page, results);

//        FakeServer fakeserver;
//        fakeserver.setScenario(scenario);
//        fakeserver.startAndWait();

//        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
//        QueryInfo * job = new QueryInfo(mediawiki);
//        job->setPageName("API");
//        job->setToken("edit");

//        connect(job, SIGNAL(page(const Page &)), this, SLOT(queryInfoHandle(const Page&)));

//        job->exec();

//        QList<FakeServer::Request> requests = fakeserver.getRequest();
//        QCOMPARE(requests.size(), 1);

//        FakeServer::Request request = requests[0];
//        QCOMPARE(request.agent, mediawiki.userAgent());
//        QCOMPARE(request.type, QString("GET"));
//        QCOMPARE(request.value, QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&intoken=edit&titles=API"));

//        QCOMPARE(job->error(), error);
//        QCOMPARE(queryInfoCount, 1);
//        QCOMPARE(queryInfoResults, results);
//        QVERIFY(fakeserver.isAllScenarioDone());
//    }

//    void testTitlesResult_data() {
//        QTest::addColumn<QString>("scenario");
//        QTest::addColumn<int>("error");
//        QTest::addColumn< Page >("results");

//        Protection protections;
//        protection.setType("edit");
//        protection.setLevel("sysop");
//        protection.setExpiry("infinity");
//        protection.setSource("");

//        Page page;
//        page.setPageId(27697087);
//        page.setTitle("API");
//        page.pageNs(0);
//        page.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page.setLastRevId(367741756);
//        page.setCounter(0);
//        page.setLength(70);
//        page.setStarttimestamp(QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
//        page.setEditToken("+\\");
//        page.setTalkid(5477418);
//        page.setFullurl(QUrl("http://en.wikipedia.org/wiki/API"));
//        page.setEditurl(QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit"));
//        page.setReadable("");
//        page.setPreload("");


//        QTest::newRow("Title: No protection")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        page.setProtections(protections);
//        QTest::newRow("Title: One page with two protections")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        QTest::newRow("Title: Two pages with two protections")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        QTest::newRow("Title: Two pages with no protection for the first page and two protections for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        QTest::newRow("Title: Two pages with two protections for the first page and no protection for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        page.setProtections(protections);
//        QTest::newRow("Title: Two pages with one protection for the first page and two protections for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;

//        QTest::newRow("Title: Two pages with two protections for the first page and one protection for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << page;
//    }

//    void testIdsResult() {
//        QFETCH(QString, scenario);
//        QFETCH(int, error);
//        QFETCH(Page, results);

//        FakeServer fakeserver;
//        fakeserver.setScenario(scenario);
//        fakeserver.startAndWait();

//        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
//        QueryInfo * job = new QueryInfo(mediawiki);
//        job->setPageId(27697087);
//        job->setToken("edit");

//        connect(job, SIGNAL(infos(const QList<QueryInfo::Result> &)), this, SLOT(queryInfoHandle(const QList<QueryInfo::Result> &)));

//        job->exec();

//        QList<FakeServer::Request> requests = fakeserver.getRequest();
//        QCOMPARE(requests.size(), 1);

//        FakeServer::Request request = requests[0];
//        QCOMPARE(request.agent, mediawiki.userAgent());
//        QCOMPARE(request.type, QString("GET"));
//        QCOMPARE(request.value, QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&intoken=edit&pageids=27697087"));

//        QCOMPARE(job->error(), error);
//        QCOMPARE(queryInfoCount, 1);
//        QCOMPARE(queryInfoResults, results);
//        QVERIFY(fakeserver.isAllScenarioDone());
//    }

//    void testIdsResult_data() {
//        QTest::addColumn<QString>("scenario");
//        QTest::addColumn<int>("error");
//        QTest::addColumn< Page >("results");

//        QList<QueryInfo::Protection> protections;
//        protections.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));
//        protections.push_back(QueryInfo::Protection("move", "sysop", "infinity", ""));

//        QList<QueryInfo::Protection> protections1;
//        protections1.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));

//        Page page;
//        page.m_pageid = 27697087;
//        page.m_title = "API";
//        page.m_ns = 0;
//        page.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page.m_lastrevid = 367741756;
//        page.m_counter = 0;
//        page.m_length = 70;
//        page.m_starttimestamp = QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page.m_edittoken = "+\\";
//        page.m_talkid = 5477418;
//        page.m_fullurl = QUrl("http://en.wikipedia.org/wiki/API");
//        page.m_editurl = QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit");
//        page.m_readable = "";
//        page.m_preload = "";

//        Page page1;
//        page1.m_pageid = 27697087;
//        page1.m_title = "API";
//        page1.m_ns = 0;
//        page1.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page1.m_lastrevid = 367741756;
//        page1.m_counter = 0;
//        page1.m_length = 70;
//        page1.m_starttimestamp = QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page1.m_edittoken = "+\\";
//        page1.m_talkid = 5477418;
//        page1.m_fullurl = QUrl("http://en.wikipedia.org/wiki/API");
//        page1.m_editurl = QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit");
//        page1.m_readable = "";
//        page1.m_preload = "";

//        QTest::newRow("Title: No protection")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
//                << int(KJob::NoError)
//                << (QList<QueryInfo::Result>() << page);

//        page.setProtections(protections);
//        QTest::newRow("Title: One page with two protections")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << (QList<QueryInfo::Result>()
//                        << page);

//        QTest::newRow("Title: Two pages with two protections")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << (QList<QueryInfo::Result>()
//                        << page << page);

//        QTest::newRow("Title: Two pages with no protection for the first page and two protections for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << (QList<QueryInfo::Result>()
//                        << page1 << page);

//        QTest::newRow("Title: Two pages with two protections for the first page and no protection for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
//                << int(KJob::NoError)
//                << (QList<QueryInfo::Result>()
//                        << page << page1);

//        page1.setProtections(protections1);
//        QTest::newRow("Title: Two pages with one protection for the first page and two protections for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << (QList<QueryInfo::Result>()
//                        << page1 << page);

//        QTest::newRow("Title: Two pages with two protections for the first page and one protection for the second page")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
//                << int(KJob::NoError)
//                << (QList<QueryInfo::Result>()
//                        << page << page1);

//    }

//    void testWarningResult() {
//        QFETCH(QString, scenario);
//        QFETCH(int, error);
//        QFETCH(QString, warning);
//        QFETCH(QList<QueryInfo::Result>, results);

//        FakeServer fakeserver;
//        fakeserver.setScenario(scenario);
//        fakeserver.startAndWait();

//        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
//        QueryInfo * job = new QueryInfo(mediawiki);
//        job->setPageId(27697087);
//        job->setToken("edit|move|delete");

//        connect(job, SIGNAL(infos(const QList<QueryInfo::Result> &)), this, SLOT(queryInfoHandle(const QList<QueryInfo::Result> &)));

//        job->exec();

//        QList<FakeServer::Request> requests = fakeserver.getRequest();
//        QCOMPARE(requests.size(), 1);

//        FakeServer::Request request = requests[0];
//        QCOMPARE(request.agent, mediawiki.userAgent());
//        QCOMPARE(request.type, QString("GET"));
//        QCOMPARE(request.value, QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&intoken=edit|move|delete&pageids=27697087"));

//        QCOMPARE(job->error(), error);
//        QCOMPARE(job->warning(), warning);
//        QCOMPARE(queryInfoCount, 1);
//        QCOMPARE(queryInfoResults, results);
//        QVERIFY(fakeserver.isAllScenarioDone());
//    }

//    void testWarningResult_data() {
//        QTest::addColumn<QString>("scenario");
//        QTest::addColumn<int>("error");
//        QTest::addColumn<QString>("warning");
//        QTest::addColumn< QList<QueryInfo::Result> >("results");

//        QList<QueryInfo::Protection> protections;
//        protections.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));
//        protections.push_back(QueryInfo::Protection("move", "sysop", "infinity", ""));

//        QList<QueryInfo::Protection> protections1;
//        protections1.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));

//        QueryInfo::Result page;
//        page.m_pageid = 27697087;
//        page.m_title = "API";
//        page.m_ns = 0;
//        page.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page.m_lastrevid = 367741756;
//        page.m_counter = 0;
//        page.m_length = 70;
//        page.m_starttimestamp = QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page.m_edittoken = "+\\";
//        page.m_talkid = 5477418;
//        page.m_fullurl = QUrl("http://en.wikipedia.org/wiki/API");
//        page.m_editurl = QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit");
//        page.m_readable = "";
//        page.m_preload = "";

//        QueryInfo::Result page1;
//        page1.m_pageid = 27697087;
//        page1.m_title = "API";
//        page1.m_ns = 0;
//        page1.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page1.m_lastrevid = 367741756;
//        page1.m_counter = 0;
//        page1.m_length = 70;
//        page1.m_starttimestamp = QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
//        page1.m_edittoken = "+\\";
//        page1.m_talkid = 5477418;
//        page1.m_fullurl = QUrl("http://en.wikipedia.org/wiki/API");
//        page1.m_editurl = QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit");
//        page1.m_readable = "";
//        page1.m_preload = "";

//        page.setProtections(protections);
//        page1.setProtections(protections1);
//        QTest::newRow("Title: Two pages with two protections for the first page and one protection for the second page and a warning")
//                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query><warnings><info xml:space=\"preserve\">Action \'delete\' is not allowed for the current user Action \'move\' is not allowed for the current user</info></warnings></api>"
//                << int(KJob::NoError)
//                << QString("Action \'delete\' is not allowed for the current user Action \'move\' is not allowed for the current user")
//                << (QList<QueryInfo::Result>()
//                        << page << page1);

//    }
