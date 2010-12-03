/*
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
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

using mediawiki::MediaWiki;
using mediawiki::QueryRevision;

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

Q_DECLARE_METATYPE(QList<QueryRevision::Result>)
Q_DECLARE_METATYPE(FakeServer::Request)
Q_DECLARE_METATYPE(QueryRevision*)

bool operator==(QueryRevision::Result const & lhs, QueryRevision::Result const & rhs) {
    return lhs.revid == rhs.revid &&
            lhs.parentId == rhs.parentId &&
            lhs.size == rhs.size &&
            lhs.minor == rhs.minor &&
            lhs.user == rhs.user &&
            lhs.timeStamp == rhs.timeStamp &&
            lhs.comment == rhs.comment &&
            lhs.content == rhs.content;
}

class QueryRevisionTest : public QObject
{

    Q_OBJECT
    
public slots:
    
    void revisionHandle(QList<QueryRevision::Result> const & revision) {
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
        QFETCH(int, rvprop);
        QFETCH(int, size);
        QFETCH(QList<QueryRevision::Result>, results);


        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));

        FakeServer fakeserver;
        fakeserver.addScenario(scenario);
        fakeserver.startAndWait();

        QueryRevision * job = new QueryRevision(mediawiki, title);
        job->setRvProp( rvprop );

        connect(job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QTest::addColumn<int>("rvprop");
        QTest::addColumn<int>("size");
        QTest::addColumn< QList<QueryRevision::Result> >("results");

        QTest::newRow("All rvprop enable")
                << QStringFromFile("./queryrevisiontest.rc")
                << FakeServer::Request("GET","","?format=xml&action=query&prop=revisions&rvprop=ids|flags|timestamp|user|comment|size|content&titles=API|Main%20Page")
                << QString("API|Main%20Page")
                << int(KJob::NoError)
                << int(IDS|FLAGS|TIMESTAMP|USER|COMMENT|SIZE|CONTENT)
                << 2
                << (QList<QueryRevision::Result>()
                        << QueryRevision::Result(367741756, 367741564, 70, "", "Graham87",
                                                           QDateTime::fromString("2010-06-13T08:41:17Z","yyyy-MM-ddThh:mm:ssZ"),
                                                           "Protected API: restore protection ([edit=sysop] (indefinite) [move=sysop] (indefinite))",
                                                           "#REDIRECT [[Application programming interface]]{{R from abbreviation}}")
                        << QueryRevision::Result(387545037, 387542946, 5074, "", "Rich Farmbrough",
                                                 QDateTime::fromString("2010-09-28T15:21:07Z","yyyy-MM-ddThh:mm:ssZ"),
                                                 "[[Help:Reverting|Reverted]] edits by [[Special:Contributions/Rich Farmbrough|Rich Farmbrough]] ([[User talk:Rich Farmbrough|talk]]) to last version by David Levy",
                                                 QStringFromFile("./queryrevisiontest_content.rc")));

        QTest::newRow("One title")
                << QStringFromFile("./queryrevisiontest_onetitle.rc")
                << FakeServer::Request("GET","","?format=xml&action=query&prop=revisions&rvprop=ids|flags|timestamp|user|comment|size|content&titles=API")
                << QString("API")
                << int(KJob::NoError)
                << int(IDS|FLAGS|TIMESTAMP|USER|COMMENT|SIZE|CONTENT)
                << 1
                << (QList<QueryRevision::Result>()
                        << QueryRevision::Result(367741756, 367741564, 70, "", "Graham87",
                                                 QDateTime::fromString("2010-06-13T08:41:17Z","yyyy-MM-ddThh:mm:ssZ"),
                                                 "Protected API: restore protection ([edit=sysop] (indefinite) [move=sysop] (indefinite))",
                                                 "#REDIRECT [[Application programming interface]]{{R from abbreviation}}"));

        QTest::newRow("Timestamp only")
                << QStringFromFile("./queryrevisiontest_timestamponly.rc")
                << FakeServer::Request("GET","","?format=xml&action=query&prop=revisions&rvprop=timestamp&titles=API|Main%20Page")
                << QString("API|Main%20Page")
                << int(KJob::NoError)
                << int(TIMESTAMP)
                << 2
                << (QList<QueryRevision::Result>()
                    << QueryRevision::Result(0, 0, 0, "", "",
                                             QDateTime::fromString("2010-06-13T08:41:17Z","yyyy-MM-ddThh:mm:ssZ"),
                                             "",
                                             "")
                    << QueryRevision::Result(0, 0, 0, "", "",
                                             QDateTime::fromString("2010-09-28T15:21:07Z","yyyy-MM-ddThh:mm:ssZ"),
                                             "",
                                             ""));
        QTest::newRow("User only")
                << QStringFromFile("./queryrevisiontest_useronly.rc")
                << FakeServer::Request("GET","","?format=xml&action=query&prop=revisions&rvprop=user&titles=API|Main%20Page")
                << QString("API|Main%20Page")
                << int(KJob::NoError)
                << int(USER)
                << 2
                << (QList<QueryRevision::Result>()
                    << QueryRevision::Result(0, 0, 0, "", "Graham87",
                                             QDateTime(),
                                             "",
                                             "")
                    << QueryRevision::Result(0, 0, 0, "", "Rich Farmbrough",
                                             QDateTime(),
                                             "",
                                             ""));

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

        QueryRevision * job = new QueryRevision(mediawiki, "NoTitle");
        job->setRvProp( SIZE|CONTENT );

        connect(job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
                << int(QueryRevision::RevIds);

        QTest::newRow("Multilple pages")
                << "<api><error code=\"rvmultpages\" info=\"\"/></api>"
                << int(QueryRevision::MultPages);

        QTest::newRow("Access Denied")
                << "<api><error code=\"rvaccessdenied\" info=\"\"/></api>"
                << int(QueryRevision::AccessDenied);

        QTest::newRow("Add Parameters")
                << "<api><error code=\"rvbadparams\" info=\"\"/></api>"
                << int(QueryRevision::AddParams);

        QTest::newRow("No Such Section")
                << "<api><error code=\"rvnosuchsection\" info=\"\"/></api>"
                << int(QueryRevision::NoSuchSection);

    }
    void testRvContinue()
    {
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer::Request requestSend("GET","","?format=xml&action=query&prop=revisions&rvcontinue=5555&titles=API");
        QueryRevision job(mediawiki, "API");
        job.setRvContinue(5555);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QueryRevision job(mediawiki, "API");
        job.setRvLimit(2);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QueryRevision job(mediawiki, "API");
        job.setRvStartId(2);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QueryRevision job(mediawiki, "API");
        job.setRvEndId(2);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QueryRevision job(mediawiki, "API");
        job.setRvStart(QDateTime::fromString("2010-09-28T15:21:07Z","yyyy-MM-ddThh:mm:ssZ"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QueryRevision job(mediawiki, "API");
        job.setRvEnd(QDateTime::fromString("2010-09-28T15:21:07Z","yyyy-MM-ddThh:mm:ssZ"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QueryRevision job(mediawiki, "API");
        job.setRvUser(QString("martine"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QueryRevision job(mediawiki, "API");
        job.setRvExcludeUser(QString("martine"));

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QueryRevision job(mediawiki, "API");
        job.setRvDir(QueryRevision::older);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

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
        QueryRevision job(mediawiki, "API");
        job.setRvDir(QueryRevision::newer);

        FakeServer fakeserver;
        fakeserver.startAndWait();

        connect(&job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].value, requestSend.value);
        QCOMPARE(requests[0].type, requestSend.type);
        QVERIFY(fakeserver.isAllScenarioDone());
    }
private:
    
    int revisionCount;
    
    QList<QueryRevision::Result> revisionResults;

};

QTEST_MAIN(QueryRevisionTest);

#include "queryrevisiontest.moc"
