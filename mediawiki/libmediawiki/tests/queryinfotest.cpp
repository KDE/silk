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

bool operator==(const QueryInfo::Result & lhs, const QueryInfo::Result & rhs) {
    bool protection = lhs.protections().size() == rhs.protections().size();

    if(protection) {
        for(int i = 0 ; i < lhs.protections().size() && protection; i++) {
            protection = lhs.protections().at(i).type() == rhs.protections().at(i).type() &&
                         lhs.protections().at(i).level() == rhs.protections().at(i).level() &&
                         lhs.protections().at(i).expiry() == rhs.protections().at(i).expiry() &&
                         lhs.protections().at(i).source() == rhs.protections().at(i).source();
        }
    }
/*
    qDebug()<<"protection "<< protection;
    qDebug()<<"protection "<< lhs.protections().size() << ", " << rhs.protections().size();
    qDebug()<<"m_pageid "<< (lhs.m_pageid == rhs.m_pageid) ;
    qDebug()<<"m_pageid "<< (lhs.m_pageid) ;
    qDebug()<<"m_pageid "<< (rhs.m_pageid) ;
    qDebug()<<"m_title "<<  (lhs.m_title == rhs.m_title) ;
    qDebug()<<"m_title "<<  (lhs.m_title) ;
    qDebug()<<"m_title "<<  (rhs.m_title) ;
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

    void queryInfoHandle(const QList<QueryInfo::Result> & infos) {
        ++queryInfoCount;
        queryInfoResults = infos;
    }

private slots:

    void init() {
        queryInfoCount = 0;
    }

    void testRevidsResult() {
        QFETCH(QString, scenario);
        QFETCH(int, error);
        QFETCH(QList<QueryInfo::Result>, results);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryInfo * job = new QueryInfo(mediawiki, 367741564, QueryInfo::RevisionId, "edit");

        connect(job, SIGNAL(infos(const QList<QueryInfo::Result> &)), this, SLOT(queryInfoHandle(const QList<QueryInfo::Result> &)));

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

    void testRevidsResult_data() {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");
        QTest::addColumn< QList<QueryInfo::Result> >("results");

        QList<QueryInfo::Protection> protections;
        protections.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));
        protections.push_back(QueryInfo::Protection("move", "sysop", "infinity", ""));

        QList<QueryInfo::Protection> protections1;
        protections1.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));

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

        QueryInfo::Result page1;
        page1.m_pageid = 27697087;
        page1.m_title = "API";
        page1.m_ns = 0;
        page1.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page1.m_lastrevid = 367741756;
        page1.m_counter = 0;
        page1.m_length = 70;
        page1.m_starttimestamp = QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page1.m_edittoken = "+\\";
        page1.m_talkid = 5477418;
        page1.m_fullurl = QUrl("http://en.wikipedia.org/wiki/API");
        page1.m_editurl = QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit");
        page1.m_readable = "";
        page1.m_preload = "";

        QTest::newRow("Revids: No protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>() << page);

        page.setProtections(protections);
        QTest::newRow("Revids: One page with two protections")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page);

        QTest::newRow("Revids: Two pages with two protections")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page);

        QTest::newRow("Revids: Two pages with no protection for the first page and two protections for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page1 << page);

        QTest::newRow("Revids: Two pages with two protections for the first page and no protection for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page1);

        page1.setProtections(protections1);
        QTest::newRow("Revids: Two pages with one protection for the first page and two protections for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page1 << page);

        QTest::newRow("Revids: Two pages with two protections for the first page and one protection for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page1);
    }

    void testTitlesResult() {
        QFETCH(QString, scenario);
        QFETCH(int, error);
        QFETCH(QList<QueryInfo::Result>, results);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryInfo * job = new QueryInfo(mediawiki, "API", "edit");

        connect(job, SIGNAL(infos(const QList<QueryInfo::Result> &)), this, SLOT(queryInfoHandle(const QList<QueryInfo::Result> &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE(request.agent, mediawiki.userAgent());
        QCOMPARE(request.type, QString("GET"));
        QCOMPARE(request.value, QString("?format=xml&action=query&intoken=edit&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&titles=API"));

        QCOMPARE(job->error(), error);
        QCOMPARE(queryInfoCount, 1);
        QCOMPARE(queryInfoResults, results);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testTitlesResult_data() {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");
        QTest::addColumn< QList<QueryInfo::Result> >("results");

        QList<QueryInfo::Protection> protections;
        protections.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));
        protections.push_back(QueryInfo::Protection("move", "sysop", "infinity", ""));

        QList<QueryInfo::Protection> protections1;
        protections1.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));

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

        QueryInfo::Result page1;
        page1.m_pageid = 27697087;
        page1.m_title = "API";
        page1.m_ns = 0;
        page1.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page1.m_lastrevid = 367741756;
        page1.m_counter = 0;
        page1.m_length = 70;
        page1.m_starttimestamp = QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page1.m_edittoken = "+\\";
        page1.m_talkid = 5477418;
        page1.m_fullurl = QUrl("http://en.wikipedia.org/wiki/API");
        page1.m_editurl = QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit");
        page1.m_readable = "";
        page1.m_preload = "";

        QTest::newRow("Title: No protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>() << page);

        page.setProtections(protections);
        QTest::newRow("Title: One page with two protections")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page);

        QTest::newRow("Title: Two pages with two protections")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page);

        QTest::newRow("Title: Two pages with no protection for the first page and two protections for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page1 << page);

        QTest::newRow("Title: Two pages with two protections for the first page and no protection for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page1);

        page1.setProtections(protections1);
        QTest::newRow("Title: Two pages with one protection for the first page and two protections for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page1 << page);

        QTest::newRow("Title: Two pages with two protections for the first page and one protection for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page1);
    }

    void testIdsResult() {
        QFETCH(QString, scenario);
        QFETCH(int, error);
        QFETCH(QList<QueryInfo::Result>, results);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryInfo * job = new QueryInfo(mediawiki, 27697087, QueryInfo::PageId, "edit");

        connect(job, SIGNAL(infos(const QList<QueryInfo::Result> &)), this, SLOT(queryInfoHandle(const QList<QueryInfo::Result> &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE(request.agent, mediawiki.userAgent());
        QCOMPARE(request.type, QString("GET"));
        QCOMPARE(request.value, QString("?format=xml&action=query&intoken=edit&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&pageids=27697087"));

        QCOMPARE(job->error(), error);
        QCOMPARE(queryInfoCount, 1);
        QCOMPARE(queryInfoResults, results);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testIdsResult_data() {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");
        QTest::addColumn< QList<QueryInfo::Result> >("results");

        QList<QueryInfo::Protection> protections;
        protections.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));
        protections.push_back(QueryInfo::Protection("move", "sysop", "infinity", ""));

        QList<QueryInfo::Protection> protections1;
        protections1.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));

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

        QueryInfo::Result page1;
        page1.m_pageid = 27697087;
        page1.m_title = "API";
        page1.m_ns = 0;
        page1.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page1.m_lastrevid = 367741756;
        page1.m_counter = 0;
        page1.m_length = 70;
        page1.m_starttimestamp = QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page1.m_edittoken = "+\\";
        page1.m_talkid = 5477418;
        page1.m_fullurl = QUrl("http://en.wikipedia.org/wiki/API");
        page1.m_editurl = QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit");
        page1.m_readable = "";
        page1.m_preload = "";

        QTest::newRow("Title: No protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>() << page);

        page.setProtections(protections);
        QTest::newRow("Title: One page with two protections")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page);

        QTest::newRow("Title: Two pages with two protections")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page);

        QTest::newRow("Title: Two pages with no protection for the first page and two protections for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page1 << page);

        QTest::newRow("Title: Two pages with two protections for the first page and no protection for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page1);

        page1.setProtections(protections1);
        QTest::newRow("Title: Two pages with one protection for the first page and two protections for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page1 << page);

        QTest::newRow("Title: Two pages with two protections for the first page and one protection for the second page")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << int(KJob::NoError)
                << (QList<QueryInfo::Result>()
                        << page << page1);

    }

    void testWarningResult() {
        QFETCH(QString, scenario);
        QFETCH(int, error);
        QFETCH(QString, warning);
        QFETCH(QList<QueryInfo::Result>, results);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryInfo * job = new QueryInfo(mediawiki, 27697087, QueryInfo::PageId, QString("edit|move|delete"));

        connect(job, SIGNAL(infos(const QList<QueryInfo::Result> &)), this, SLOT(queryInfoHandle(const QList<QueryInfo::Result> &)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE(request.agent, mediawiki.userAgent());
        QCOMPARE(request.type, QString("GET"));
        QCOMPARE(request.value, QString("?format=xml&action=query&intoken=edit|move|delete&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&pageids=27697087"));

        QCOMPARE(job->error(), error);
        QCOMPARE(job->warning(), warning);
        QCOMPARE(queryInfoCount, 1);
        QCOMPARE(queryInfoResults, results);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testWarningResult_data() {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");
        QTest::addColumn<QString>("warning");
        QTest::addColumn< QList<QueryInfo::Result> >("results");

        QList<QueryInfo::Protection> protections;
        protections.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));
        protections.push_back(QueryInfo::Protection("move", "sysop", "infinity", ""));

        QList<QueryInfo::Protection> protections1;
        protections1.push_back(QueryInfo::Protection("edit", "sysop", "infinity", ""));

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

        QueryInfo::Result page1;
        page1.m_pageid = 27697087;
        page1.m_title = "API";
        page1.m_ns = 0;
        page1.m_touched = QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page1.m_lastrevid = 367741756;
        page1.m_counter = 0;
        page1.m_length = 70;
        page1.m_starttimestamp = QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
        page1.m_edittoken = "+\\";
        page1.m_talkid = 5477418;
        page1.m_fullurl = QUrl("http://en.wikipedia.org/wiki/API");
        page1.m_editurl = QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit");
        page1.m_readable = "";
        page1.m_preload = "";

        page.setProtections(protections);
        page1.setProtections(protections1);
        QTest::newRow("Title: Two pages with two protections for the first page and one protection for the second page and a warning")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query><warnings><info xml:space=\"preserve\">Action \'delete\' is not allowed for the current user Action \'move\' is not allowed for the current user</info></warnings></api>"
                << int(KJob::NoError)
                << QString("Action \'delete\' is not allowed for the current user Action \'move\' is not allowed for the current user")
                << (QList<QueryInfo::Result>()
                        << page << page1);

    }

private:

    int queryInfoCount;
    QList<QueryInfo::Result> queryInfoResults;
};


QTEST_MAIN(QueryInfoTest);
#include "queryinfotest.moc"
#endif // TEST_QUERYINFO_H
