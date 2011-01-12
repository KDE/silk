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
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtTest/QTest>
#include <KDE/KJob>

#include "libmediawikitest/fakeserver.h"

#include "mediawiki.h"
#include "querysiteinfousergroups.h"

using mediawiki::MediaWiki;
using mediawiki::QuerySiteinfoUsergroups;

Q_DECLARE_METATYPE(QList<QuerySiteinfoUsergroups::Result>)

bool operator==(const QuerySiteinfoUsergroups::Result & lhs, const QuerySiteinfoUsergroups::Result & rhs) {
    return lhs.name() == rhs.name() &&
           lhs.rights() == rhs.rights() &&
           lhs.hasNumber() == rhs.hasNumber() &&
           lhs.hasNumber() /* && rhs.hasNumber */ ? lhs.number() == rhs.number() : true;
}

class QuerySiteinfoUsergroupsTest : public QObject
{

    Q_OBJECT
    
public slots:
    
    void usergroupsHandle(const QList<QuerySiteinfoUsergroups::Result> & usergroups) {
        ++usergroupsCount;
        usergroupsResults = usergroups;
    }

private slots:
    
    void init() {
        usergroupsCount = 0;
    }

    void testResult() {
        QFETCH(QString, scenario);
        QFETCH(bool, includeNumber);
        QFETCH(int, error);
        QFETCH(QList<QuerySiteinfoUsergroups::Result>, results);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QuerySiteinfoUsergroups * job = new QuerySiteinfoUsergroups(mediawiki, includeNumber);

        connect(job, SIGNAL(usergroups(const QList<QuerySiteinfoUsergroups::Result> &)), this, SLOT(usergroupsHandle(const QList<QuerySiteinfoUsergroups::Result> &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE(request.agent, mediawiki.userAgent());
        QCOMPARE(request.type, QString("GET"));
        if (includeNumber) {
            QCOMPARE(request.value, QString("?format=xml&action=query&meta=siteinfo&siprop=usergroups&sinumberingroup="));
        } else {
            QCOMPARE(request.value, QString("?format=xml&action=query&meta=siteinfo&siprop=usergroups"));
        }

        QCOMPARE(job->error(), error);

        QCOMPARE(usergroupsCount, 1);

        QCOMPARE(usergroupsResults, results);
        
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testResult_data() {           
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<bool>("includeNumber");
        QTest::addColumn<int>("error");
        QTest::addColumn< QList<QuerySiteinfoUsergroups::Result> >("results");

        QTest::newRow("No group")
                << "<?xml version=\"1.0\"?><api><query><usergroups></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << QList<QuerySiteinfoUsergroups::Result>();
        
        QTest::newRow("One group with no right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"/></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>()));

        QTest::newRow("One group with one right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>() << "permission_1_1"));

        QTest::newRow("One group with two rights")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission><permission>permission_1_2</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>() << "permission_1_1" << "permission_1_2"));

        QTest::newRow("Two groups with group one no right and group two no right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"></group><group name=\"name_2\" /></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>())
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>()));

        QTest::newRow("Two groups with group one no right and group two one right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"></group><group name=\"name_2\"><rights><permission>permission_2_1</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>())
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>() << "permission_2_1"));

        QTest::newRow("Two groups with group one no right and group two two rights")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"></group><group name=\"name_2\"><rights><permission>permission_2_1</permission><permission>permission_2_2</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>())
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>() << "permission_2_1" << "permission_2_2"));

        QTest::newRow("Two groups with group one one right and group two no right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission></rights></group><group name=\"name_2\"><rights></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>() << "permission_1_1")
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>()));

        QTest::newRow("Two groups with group one one right and group two one right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission></rights></group><group name=\"name_2\"><rights><permission>permission_2_1</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>() << "permission_1_1")
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>() << "permission_2_1"));

        QTest::newRow("Two groups with group one one right and group two two rights")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission></rights></group><group name=\"name_2\"><rights><permission>permission_2_1</permission><permission>permission_2_2</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>() << "permission_1_1")
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>() << "permission_2_1" << "permission_2_2"));

        QTest::newRow("Two groups with group one two rights and group two no right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission><permission>permission_1_2</permission></rights></group><group name=\"name_2\"><rights /></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>() << "permission_1_1" << "permission_1_2")
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>()));

        QTest::newRow("Two groups with group one two rights and group two one right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission><permission>permission_1_2</permission></rights></group><group name=\"name_2\"><rights><permission>permission_2_1</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>() << "permission_1_1" << "permission_1_2")
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>() << "permission_2_1"));

        QTest::newRow("Two groups with group one two rights and group two two rights")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission><permission>permission_1_2</permission></rights></group><group name=\"name_2\"><rights><permission>permission_2_1</permission><permission>permission_2_2</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>() << "permission_1_1" << "permission_1_2")
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>() << "permission_2_1" << "permission_2_2"));

        QTest::newRow("No group with include number")
                << "<?xml version=\"1.0\"?><api><query><usergroups></usergroups></query></api>"
                << true
                << int(KJob::NoError)
                << QList<QuerySiteinfoUsergroups::Result>();

        QTest::newRow("One group with include number")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\" number=\"0\"/></usergroups></query></api>"
                << true
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>(), 0));

        QTest::newRow("Two groups with include number")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\" number=\"41\"></group><group name=\"name_2\" number=\"12543\" /></usergroups></query></api>"
                << true
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>(), 41)
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>(), 12543));

        QTest::newRow("Three groups with rights and include number")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\" number=\"1781\"><rights><permission>permission_1_1</permission></rights></group><group name=\"name_2\" number=\"10989982\"><rights><permission>permission_2_1</permission><permission>permission_2_2</permission><permission>permission_2_3</permission></rights></group><group name=\"name_3\" number=\"6\"><rights><permission>permission_3_1</permission><permission>permission_3_2</permission></rights></group></usergroups></query></api>"
                << true
                << int(KJob::NoError)
                << (QList<QuerySiteinfoUsergroups::Result>()
                        << QuerySiteinfoUsergroups::Result("name_1", QList<QString>() << "permission_1_1", 1781)
                        << QuerySiteinfoUsergroups::Result("name_2", QList<QString>() << "permission_2_1" << "permission_2_2" << "permission_2_3", 10989982)
                        << QuerySiteinfoUsergroups::Result("name_3", QList<QString>() << "permission_3_1" << "permission_3_2", 6));

    }
    
private:
    
    int usergroupsCount;
    
    QList<QuerySiteinfoUsergroups::Result> usergroupsResults;

};

QTEST_MAIN(QuerySiteinfoUsergroupsTest);

#include "querysiteinfousergroupstest.moc"
