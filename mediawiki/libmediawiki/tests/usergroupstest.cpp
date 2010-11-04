/*
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
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
#include <QtTest/QSignalSpy>
#include <QtTest/QTest>
#include <KDE/KJob>

#include "libmediawikitest/fakeserver.h"

#include "mediawiki.h"
#include "usergroups.h"

using mediawiki::MediaWiki;
using mediawiki::UserGroups;

Q_DECLARE_METATYPE(QList<UserGroups::Result>)
Q_DECLARE_METATYPE(UserGroups *)

bool operator==(UserGroups::Result const & lhs, UserGroups::Result const & rhs) {
    return lhs.name == rhs.name && lhs.rights == rhs.rights;
}

UserGroups::Result makeResult(QString name, QList<QString> rights) {
    UserGroups::Result result = {name, rights};
    return result;
}

class UserGroupsTest : public QObject
{

    Q_OBJECT
    
public slots:
    
    void result(UserGroups * job, QList<UserGroups::Result> const & results) {
        ++m_count;
        m_job = job;
        m_results = results;
    }

private slots:
    
    void init() {
        m_count = 0;
        m_job = 0;
        m_results = QList<UserGroups::Result>();
    }

    void testResult() {
        //qRegisterMetaType< QList<UserGroups::Result> const >("QList<mediawiki::UserGroups::Result> const");
        //qRegisterMetaType< UserGroups * >("UserGroups *");
        
        QFETCH(QString, scenario);
        QFETCH(int, error);
        QFETCH(QList<UserGroups::Result>, results);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        UserGroups job(mediawiki);

        connect(&job, SIGNAL(result(UserGroups *, QList<UserGroups::Result> const &)), this, SLOT(result(UserGroups *,QList<UserGroups::Result> const &)));

        job.exec();

        QCOMPARE(m_count, 1);

        QCOMPARE(m_job, &job);
        QCOMPARE(m_job->error(), error);

        QCOMPARE(m_results, results);
        
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testResult_data() {           
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");
        QTest::addColumn< QList<UserGroups::Result> >("results");

        QTest::newRow("No group") << "<?xml version=\"1.0\"?><api><query><usergroups></usergroups></query></api>"
                                    << int(KJob::NoError)
                                    << QList<UserGroups::Result>();
        
        QTest::newRow("One group with no rights") << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"groupname\"/></usergroups></query></api>"
                                    << int(KJob::NoError)
                                    << (QList<UserGroups::Result>() << makeResult("groupname", QList<QString>()));
    }
    
private:
    
    int m_count;
    
    UserGroups * m_job;
    
    QList<UserGroups::Result> m_results;

};

QTEST_MAIN(UserGroupsTest);

#include "usergroupstest.moc"
