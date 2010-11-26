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

        QString requestTrue = "?format=xml&action=query&prop=revisions&rvprop=ids|flags|timestamp|user|comment|size|content&titles=API|Main%20Page";

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QString title = "API|Main%20Page";

        FakeServer fakeserver;
        fakeserver.addScenarioFromFile("./queryrevisiontest.rc");
        fakeserver.startAndWait();

        QueryRevision * job = new QueryRevision(mediawiki, title);
        job->setRvProp( IDS|FLAGS|TIMESTAMP|USER|COMMENT|SIZE|CONTENT );

        connect(job, SIGNAL(revision(QList<QueryRevision::Result> const &)), this, SLOT(revisionHandle(QList<QueryRevision::Result> const &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE(request.type, QString("GET"));
        QCOMPARE(job->error(), (int)QueryRevision::NoError);
        QCOMPARE(revisionCount, 1);
        QCOMPARE(requestTrue, request.value);
        QCOMPARE(revisionResults.size(), 2);
        QueryRevision::Result resultone, resulttow;
        resultone.revid     = 367741756;
        resultone.parentId  = 367741564;
        resultone.size      = 70;
        resultone.minor     = "";
        resultone.user      = "Graham87";
        resultone.timeStamp = QDateTime::fromString("2010-06-13T08:41:17Z","yyyy-MM-ddThh:mm:ssZ");
        resultone.comment   = "Protected API: restore protection ([edit=sysop] (indefinite) [move=sysop] (indefinite))";
        resultone.content   = "#REDIRECT [[Application programming interface]]{{R from abbreviation}}";

        resulttow.revid     = 387545037;
        resulttow.parentId  = 387542946;
        resulttow.size      = 5074;
        resulttow.minor     = "";
        resulttow.user      = "Rich Farmbrough";
        resulttow.timeStamp = QDateTime::fromString("2010-09-28T15:21:07Z","yyyy-MM-ddThh:mm:ssZ");
        resulttow.comment   = "[[Help:Reverting|Reverted]] edits by [[Special:Contributions/Rich Farmbrough|Rich Farmbrough]] ([[User talk:Rich Farmbrough|talk]]) to last version by David Levy";
        resulttow.content   = QStringFromFile("./queryrevisiontest_content.rc");
        QCOMPARE(revisionResults[0], resultone);
        QCOMPARE(revisionResults[1], resulttow);
        
        QVERIFY(fakeserver.isAllScenarioDone());
    }


private:
    
    int revisionCount;
    
    QList<QueryRevision::Result> revisionResults;

};

QTEST_MAIN(QueryRevisionTest);

#include "queryrevisiontest.moc"
