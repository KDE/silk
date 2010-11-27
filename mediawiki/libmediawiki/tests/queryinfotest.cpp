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
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::QueryInfo;

Q_DECLARE_METATYPE(QList<QueryInfo::Result>)

bool operator==(QueryInfo::Result const & lhs, QueryInfo::Result const & rhs) {
    bool protection = lhs.protections().size() == rhs.protections().size();

    if(protection) {
        for(int i = 0 ; i < lhs.protections().size() && protection; i++) {
            protection = lhs.protections().at(i).type() == rhs.protections().at(i).type() &&
                         lhs.protections().at(i).level() == rhs.protections().at(i).level() &&
                         lhs.protections().at(i).expiry() == rhs.protections().at(i).expiry() &&
                         lhs.protections().at(i).source() == rhs.protections().at(i).source();
        }
    }

 /*   qDebug()<<"protection "<< protection;
    qDebug()<<"protection "<< lhs.protections().size() << ", " << rhs.protections().size();
    qDebug()<<"m_pageid "<< (lhs.m_pageid == rhs.m_pageid) ;
    qDebug()<<"m_title "<<  (lhs.m_title == rhs.m_title) ;
    qDebug()<<"m_ns "<< (lhs.m_ns == rhs.m_ns );
    qDebug()<<"m_touched "<<(lhs.m_touched == rhs.m_touched );
    qDebug()<<"m_lastrevid "<< (lhs.m_lastrevid == rhs.m_lastrevid );
    qDebug()<<"m_counter "<< (lhs.m_counter == rhs.m_counter );
    qDebug()<<"m_length "<< (lhs.m_length == rhs.m_length );
    qDebug()<<"m_starttimestamp "<< (lhs.m_starttimestamp == rhs.m_starttimestamp );
    qDebug()<<"m_edittoken "<< (lhs.m_edittoken == rhs.m_edittoken );
    qDebug()<<"m_talkid "<< (lhs.m_talkid == rhs.m_talkid );
    qDebug()<<"m_fullurl "<< (lhs.m_fullurl == rhs.m_fullurl );
    qDebug()<<"m_editurl "<< (lhs.m_editurl == rhs.m_editurl );
    qDebug()<<"m_readable "<< (lhs.m_readable == rhs.m_readable );
    qDebug()<<"m_preload "<< (lhs.m_preload == rhs.m_preload );*/

    return lhs.m_pageid == rhs.m_pageid &&
            lhs.m_title == rhs.m_title &&
            lhs.m_ns == rhs.m_ns &&
            lhs.touched() == rhs.touched() &&
            lhs.m_lastrevid == rhs.m_lastrevid &&
            lhs.m_counter == rhs.m_counter &&
            lhs.m_length == rhs.m_length &&
            lhs.starttimestamp() == rhs.starttimestamp() &&
            lhs.m_edittoken == rhs.m_edittoken &&
            lhs.m_talkid == rhs.m_talkid &&
            lhs.m_fullurl == rhs.m_fullurl &&
            lhs.m_editurl == rhs.m_editurl &&
            lhs.m_readable == rhs.m_readable &&
            lhs.m_preload == rhs.m_preload &&
            protection;
}

class QueryInfoTest : public QObject
{
    Q_OBJECT

public slots:

    void queryInfoHandle(QList<QueryInfo::Result> const & infos) {
        ++queryInfoCount;
        queryInfoResults = infos;
    }

private slots:

    void init() {
        queryInfoCount = 0;
    }

    void testResult() {
        QFETCH(QString, scenario);
        QFETCH(int, error);
        QFETCH(QList<QueryInfo::Result>, results);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryInfo * job = new QueryInfo(mediawiki, 367741564, QueryInfo::revids, "edit");

        connect(job, SIGNAL(infos(QList<QueryInfo::Result> const &)), this, SLOT(queryInfoHandle(QList<QueryInfo::Result> const &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE(request.agent, mediawiki.userAgent());
        QCOMPARE(request.type, QString("GET"));
        QCOMPARE(request.value, QString("?format=xml&action=query&intoken=edit&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&revids=367741564"));

        QCOMPARE(job->error(), error);
        QCOMPARE(queryInfoCount, 1);
        QCOMPARE(queryInfoResults, results);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testResult_data() {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");
        QTest::addColumn< QList<QueryInfo::Result> >("results");

        QList<QueryInfo::Protection> protections;
        protections.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));
        protections.push_back(QueryInfo::Protection("move", "sysop", "infinity", ""));

        QueryInfo::Result page;
        page.m_pageid = 27697087;
        page.m_title = "API";
        page.m_ns = 0;
        page.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page.m_lastrevid = 367741756;
        page.m_counter = 0;
        page.m_length = 70;
        page.m_starttimestamp = QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page.m_edittoken = "+\\";
        page.m_talkid = 5477418;
        page.m_fullurl = QUrl("http://en.wikipedia.org/wiki/API");
        page.m_editurl = QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit");
        page.m_readable = "";
        page.m_preload = "";

        //QList<QueryInfo::Result> pages;
        //pages.push_back(page);

        QTest::newRow("No protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>() << page);

        page.setProtections(protections);
        QTest::newRow("One page with two protections")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page);

        QTest::newRow("Two pages with two protections")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page);
    }

private:

    int queryInfoCount;
    QList<QueryInfo::Result> queryInfoResults;
};


QTEST_MAIN(QueryInfoTest);
#include "queryinfotest.moc"
#endif // TEST_QUERYINFO_H
