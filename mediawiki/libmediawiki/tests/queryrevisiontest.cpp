/*
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
 *   Copyright 2010 by Guillaume Hormiere <hormiere.guillaume@gmail.com>
 *   Copyright 2011 by Manuel Campomanes <campomanes.manuel@gmail.com>
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

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtTest/QTest>
#include <KDE/KJob>
#include <QFile>
#include <QTextStream>

#include "libmediawikitest/fakeserver.h"

#include "mediawiki.h"
#include "queryrevision.h"
#include "revision.h"

using mediawiki::MediaWiki;
using mediawiki::QueryRevision;
using mediawiki::Revision;


Q_DECLARE_METATYPE(QList<Revision>)
Q_DECLARE_METATYPE(FakeServer::Request)
Q_DECLARE_METATYPE(QueryRevision*)
Q_DECLARE_METATYPE(QueryRevision::Props)

Revision constructRevision(int i,int p, int s, QString m, QString u, QDateTime t, QString cm, QString ct, QString pt, QString r){

    Revision rev;
    rev.setRevId(i);
    rev.setParentId(p);
    rev.setSize(s);
    rev.setMinor(m);
    rev.setUser(u);
    rev.setTimestamp(t);
    rev.setComment(cm);
    rev.setContent(ct);
    rev.setParseTree(pt);
    rev.setRollback(r);
    return rev;

}
void debugRev(const Revision &rev)
{
    qDebug() << rev.revId();
    qDebug() << rev.parentId();
    qDebug() << rev.size();
    //qDebug() << rev.minor();
    qDebug() << rev.user();
    qDebug() << rev.timestamp();
    qDebug() << rev.comment();
    qDebug() << rev.content();
    qDebug() << rev.parseTree();
    qDebug() << rev.rollback();
}

QString QStringFromFile( const QString &fileName )
{
  QFile file( fileName );
  file.open( QFile::ReadOnly );
  QTextStream in(&file);
  QString scenario;
  // When loading from files we never have the authentication phase
  // force jumping directly to authenticated state.

  while ( !in.atEnd() ) {
    scenario.append( in.readLine() );
  }
  file.close();
  return scenario;
}

class QueryRevisionTest : public QObject
{

    Q_OBJECT
    
public slots:
    
    void revisionHandle(const QList<Revision> & revision) {
        ++revisionCount;
        revisionResults = revision;
    }

private slots:

    void init() {
        revisionCount = 0;
    }

    void testrvpropall() {
        QFETCH(QString, scenario);
        QFETCH(FakeServer::Request, requestTrue);
        QFETCH(QString, title);
        QFETCH(int, error);
        QFETCH(QueryRevision::Props, rvprop);
        QFETCH(int, size);
        QFETCH(QList<Revision>, results);


        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer fakeserver;
        fakeserver.addScenario(scenario);
        fakeserver.startAndWait();

        QueryRevision * job = new QueryRevision(mediawiki);
        job->setProp( rvprop );
        job->setPageName(title);

        connect(job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE( requestTrue.type, request.type);
        QCOMPARE(job->error(), error);
        QCOMPARE(revisionCount, 1);
        QCOMPARE(requestTrue.value, request.value);
        QCOMPARE(revisionResults.size(), size);

        QCOMPARE(revisionResults, results);
        
        QVERIFY(fakeserver.isAllScenarioDone());
    }
    void testrvpropall_data() {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<FakeServer::Request>("requestTrue");
        QTest::addColumn<QString>("title");
        QTest::addColumn<int>("error");
        QTest::addColumn<QueryRevision::Props>("rvprop");
        QTest::addColumn<int>("size");
        QTest::addColumn< QList<Revision> >("results");

        QTest::newRow("All rvprop enable")
                << QStringFromFile("./queryrevisiontest.rc")
                << FakeServer::Request("GET","","?format=xml&action=query&prop=revisions&rvprop=ids|flags|timestamp|user|comment|size|content&titles=API|Main%20Page")
                << QString("API|Main%20Page")
                << int(KJob::NoError)
                << QueryRevision::Props(QueryRevision::Ids | QueryRevision::Flags | QueryRevision::Timestamp |QueryRevision::User | QueryRevision::Comment | QueryRevision::Size | QueryRevision::Content)
                << 2
                << (QList<Revision>()
                        << constructRevision(367741756, 367741564, 70, "", "Graham87",
                                                           QDateTime::fromString("2010-06-13T08:41:17Z","yyyy-MM-ddThh:mm:ssZ"),
                                                           "Protected API: restore protection ([edit=sysop] (indefinite) [move=sysop] (indefinite))",
                                                           "#REDIRECT [[Application programming interface]]{{R from abbreviation}}","","")
                        << constructRevision(387545037, 387542946, 5074, "", "Rich Farmbrough",
                                                 QDateTime::fromString("2010-09-28T15:21:07Z","yyyy-MM-ddThh:mm:ssZ"),
                                                 "[[Help:Reverting|Reverted]] edits by [[Special:Contributions/Rich Farmbrough|Rich Farmbrough]] ([[User talk:Rich Farmbrough|talk]]) to last version by David Levy",
                                                 QStringFromFile("./queryrevisiontest_content.rc"),"",""));

        QTest::newRow("One title")
                << QStringFromFile("./queryrevisiontest_onetitle.rc")
                << FakeServer::Request("GET","","?format=xml&action=query&prop=revisions&rvprop=ids|flags|timestamp|user|comment|size|content&titles=API")
                << QString("API")
                << int(KJob::NoError)
                << QueryRevision::Props(QueryRevision::Ids | QueryRevision::Flags | QueryRevision::Timestamp |QueryRevision::User | QueryRevision::Comment | QueryRevision::Size | QueryRevision::Content)
                << 1
                << (QList<Revision>()
                        << constructRevision(367741756, 367741564, 70, "", "Graham87",
                                                 QDateTime::fromString("2010-06-13T08:41:17Z","yyyy-MM-ddThh:mm:ssZ"),
                                                 "Protected API: restore protection ([edit=sysop] (indefinite) [move=sysop] (indefinite))",
                                                 "#REDIRECT [[Application programming interface]]{{R from abbreviation}}","",""));

        QTest::newRow("Timestamp only")
                << QStringFromFile("./queryrevisiontest_timestamponly.rc")
                << FakeServer::Request("GET","","?format=xml&action=query&prop=revisions&rvprop=timestamp&titles=API|Main%20Page")
                << QString("API|Main%20Page")
                << int(KJob::NoError)
                << QueryRevision::Props(QueryRevision::Timestamp)
                << 2
                << (QList<Revision>()
                    << constructRevision(-1, -1, -1, "", "",
                                             QDateTime::fromString("2010-06-13T08:41:17Z","yyyy-MM-ddThh:mm:ssZ"),
                                             "",
                                             "","","")
                    << constructRevision(-1, -1, -1, "", "",
                                             QDateTime::fromString("2010-09-28T15:21:07Z","yyyy-MM-ddThh:mm:ssZ"),
                                             "",
                                             "","",""));
        QTest::newRow("User only")
                << QStringFromFile("./queryrevisiontest_useronly.rc")
                << FakeServer::Request("GET","","?format=xml&action=query&prop=revisions&rvprop=user&titles=API|Main%20Page")
                << QString("API|Main%20Page")
                << int(KJob::NoError)
                << QueryRevision::Props(QueryRevision::User)
                << 2
                << (QList<Revision>()
                    << constructRevision(-1, -1, -1, "", "Graham87",
                                             QDateTime(),
                                             "",
                                             "","","")
                    << constructRevision(-1, -1, -1, "", "Rich Farmbrough",
                                             QDateTime(),
                                             "",
                                             "","",""));

    }

    void testerror()
    {
        QFETCH(QString, scenario);
        QFETCH(int, error);


        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer fakeserver;
        if(scenario != QString("error serveur"))
        {
            fakeserver.addScenario(scenario);
            fakeserver.startAndWait();
        }

        QueryRevision * job = new QueryRevision(mediawiki);
        job->setProp( QueryRevision::Size | QueryRevision::Content );
        job->setPageName("title");

        connect(job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job->exec();

        if(scenario != QString("error serveur"))
        {
            QList<FakeServer::Request> requests = fakeserver.getRequest();
            QCOMPARE(requests.size(), 1);
        }
        QCOMPARE(job->error(), error);
        QCOMPARE(revisionCount, 1);
        QCOMPARE(revisionResults.size(), 0);

        if(scenario != QString("error serveur"))
        {
            QVERIFY(fakeserver.isAllScenarioDone());
        }
    }
    void testerror_data()
    {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");

        QTest::newRow("XML")
                << QStringFromFile("./queryrevisiontest_cuted.rc")
                << int(QueryRevision::XmlError);

        QTest::newRow("Network")
                << "error serveur"
                << int(QueryRevision::NetworkError);

        QTest::newRow("Revision Ids")
                << "<api><error code=\"rvrevids\" info=\"\"/></api>"
                << int(QueryRevision::WrongRevisionId);

        QTest::newRow("Multilple pages")
                << "<api><error code=\"rvmultpages\" info=\"\"/></api>"
                << int(QueryRevision::MultiPagesNotAllowed);

        QTest::newRow("Access Denied")
                << "<api><error code=\"rvaccessdenied\" info=\"\"/></api>"
                << int(QueryRevision::TitleAccessDenied);

        QTest::newRow("Add Parameters")
                << "<api><error code=\"rvbadparams\" info=\"\"/></api>"
                << int(QueryRevision::TooManyParams);

        QTest::newRow("No Such Section")
                << "<api><error code=\"rvnosuchsection\" info=\"\"/></api>"
                << int(QueryRevision::SectionNotFound);

    }
    void testRvContinue()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvcontinue=5555&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setContinue(5555);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }
    void testRvLimit()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvlimit=2&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setLimit(2);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }
    void testRvStartId()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvstartid=2&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setStartId(2);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }
    void testRvEndId()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvendid=2&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setEndId(2);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testRvStart()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvstart=2010-09-28T15:21:07Z&titles=API");
        QueryRevision job(mediawiki);
         job.setPageName("API");
        job.setStart(QDateTime::fromString("2010-09-28T15:21:07Z","yyyy-MM-ddThh:mm:ssZ"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testRvEnd()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvend=2010-09-28T15:21:07Z&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setEnd(QDateTime::fromString("2010-09-28T15:21:07Z","yyyy-MM-ddThh:mm:ssZ"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testRvUser()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvuser=martine&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setUser(QString("martine"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testRvExcludeUser()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvexcludeuser=martine&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setExcludeUser(QString("martine"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testRvDirOlder()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvdir=older&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setDir(QueryRevision::Older);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testRvDirNewer()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvdir=newer&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setDir(QueryRevision::Newer);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }
    void testGenerateXML()
    {
        QString scenario = QStringFromFile("./queryrevisiontest_parsetree.rc");
        FakeServer::Request requestTrue("GET","","?format=xml&action=query&prop=revisions&rvgeneratexml=on&rvprop=timestamp|user|comment|content&titles=API");
        QString title = "API";
        int error = 0;
        QueryRevision::Props rvprop = QueryRevision::Timestamp |QueryRevision::User | QueryRevision::Comment | QueryRevision::Content;
        int size = 2;
        QList<Revision> results;

        Revision rev;
        rev.setUser("martine");
        rev.setTimestamp(QDateTime::fromString("2010-06-13T08:41:17Z","yyyy-MM-ddThh:mm:ssZ"));
        rev.setComment("Protected API: restore protection ([edit=sysop] (indefinite) [move=sysop] (indefinite))");
        rev.setContent("#REDIRECT [[Application programming interface]]{{R from abbreviation}}");
        rev.setParseTree("<root>#REDIRECT [[Application programming interface]]<template><title>R from abbreviation</title></template></root>");
        results << rev;
        Revision rev2;
        rev2.setUser("Graham87");
        rev2.setTimestamp(QDateTime::fromString("2010-06-13T08:41:17Z","yyyy-MM-ddThh:mm:ssZ"));
        rev2.setComment("Protected API: restore protection ([edit=sysop] (indefinite) [move=sysop] (indefinite))");
        rev2.setContent("#REDIRECT [[Application programming interface]]{{R from abbreviation}}");
        rev2.setParseTree("<root>#REDIRECT [[Application programming interface]]<template><title>R from abbreviation</title></template></root>");
        results << rev2;
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));

        FakeServer fakeserver;
        fakeserver.addScenario(scenario);
        fakeserver.startAndWait();

        QueryRevision * job = new QueryRevision(mediawiki);
        job->setProp( rvprop );
        job->setPageName(title);
        job->setGenerateXML(true);

        connect(job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE( requestTrue.type, request.type);
        QCOMPARE(revisionCount, 1);
        QCOMPARE(requestTrue.value, request.value);

        QCOMPARE(job->error(), error);
        QCOMPARE(revisionResults.size(), size);
//        debugRev(revisionResults[0]);
//        debugRev(results[0]);
//        debugRev(revisionResults[1]);
//        debugRev(results[1]);
        QCOMPARE(revisionResults, results);

        QVERIFY(fakeserver.isAllScenarioDone());
    }
    void testRvSection()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvsection=1&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setSection(1);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }
    void testRvToken()
    {
        QString scenario = QStringFromFile("queryrevisiontest_rollback.rc");
        FakeServer::Request requestTrue("GET","","?format=xml&action=query&prop=revisions&rvtoken=rollback&titles=API");
        QString title = "API";
        int error = 0;
        int size = 1;
        QList<Revision> results;
        results << constructRevision(-1, -1, -1, "", "",
                                           QDateTime(),
                                           "",
                                           "",
                                           ""
                                           ,"094a45ddbbd5e90d55d79d2a23a8c921+\\");


        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));

        FakeServer fakeserver;
        fakeserver.addScenario(scenario);
        fakeserver.startAndWait();

        QueryRevision * job = new QueryRevision(mediawiki);
        job->setPageName(title);

        job->setToken(QueryRevision::Rollback);

        connect(job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE( requestTrue.type, request.type);
        QCOMPARE(revisionCount, 1);
        QCOMPARE(requestTrue.value, request.value);

        QCOMPARE(job->error(), error);
        QCOMPARE(revisionResults.size(), size);
        QCOMPARE(revisionResults, results);

        QVERIFY(fakeserver.isAllScenarioDone());
    }
    void testRvExpandTemplates()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvexpandtemplates=on&titles=API");
        QueryRevision job(mediawiki);
        job.setPageName("API");
        job.setExpandTemplates(true);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }
    void testRvPageId(){
        FakeServer::Request requestTrue("GET","","?format=xml&action=query&prop=revisions&pageids=2993&rvprop=timestamp|user|comment|content");
        QueryRevision::Props rvprop = QueryRevision::Timestamp |QueryRevision::User | QueryRevision::Comment | QueryRevision::Content;
        int id= 2993;


        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        QueryRevision * job = new QueryRevision(mediawiki);
        job->setProp( rvprop );
        job->setPageId(id);

        connect(job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE( requestTrue.type, request.type);
        QCOMPARE(revisionCount, 1);
        QCOMPARE(requestTrue.value, request.value);

        QVERIFY(fakeserver.isAllScenarioDone());

    }

    void testRvRevisionId(){
        FakeServer::Request requestTrue("GET","","?format=xml&action=query&prop=revisions&revids=2993&rvprop=timestamp|user|comment|content");
        QueryRevision::Props rvprop = QueryRevision::Timestamp |QueryRevision::User | QueryRevision::Comment | QueryRevision::Content;
        int id= 2993;


        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        QueryRevision * job = new QueryRevision(mediawiki);
        job->setProp( rvprop );
        job->setRevisionId(id);

        connect(job, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE( requestTrue.type, request.type);
        QCOMPARE(revisionCount, 1);
        QCOMPARE(requestTrue.value, request.value);

        QVERIFY(fakeserver.isAllScenarioDone());

    }

private:
    
    int revisionCount;
    
    QList<Revision> revisionResults;

};

QTEST_MAIN(QueryRevisionTest);

#include "queryrevisiontest.moc"
