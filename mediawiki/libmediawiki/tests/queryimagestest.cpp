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

#include <QtCore/QObject>
#include <QtTest/QTest>

#include "libmediawikitest/fakeserver.h"

#include "mediawiki.h"
#include "queryimages.h"

using mediawiki::MediaWiki;
using mediawiki::QueryImages;

Q_DECLARE_METATYPE(QList<QString>);
Q_DECLARE_METATYPE(QList<QueryImages::Image>);
Q_DECLARE_METATYPE(QList<QueryImages::Page>);
Q_DECLARE_METATYPE(QList<QList<QueryImages::Page> >);

QueryImages::Page constructsPage(unsigned int pageId,
                                 unsigned int namespaceId,
                                 QString const & title,
                                 QList<QueryImages::Image> const & images)
{
    QueryImages::Page const page(pageId, namespaceId, title, title, images, false);
    Q_ASSERT(!page.isMissing());
    Q_ASSERT(!page.isNormalized());
    return page;
}

QueryImages::Page constructsPageNormalized(unsigned int pageId,
                                           unsigned int namespaceId,
                                           QString const & title,
                                           QString const & titleNoNormalized,
                                           QList<QueryImages::Image> const & images)
{
    QueryImages::Page const page(pageId, namespaceId, title, titleNoNormalized, images, false);
    Q_ASSERT(!page.isMissing());
    Q_ASSERT(page.isNormalized());
    return page;
}

QueryImages::Page constructsPageMissing(unsigned int namespaceId,
                                        QString const & title)
{
    unsigned int pageId;
    QueryImages::Page const page(pageId, namespaceId, title, title, QList<QueryImages::Image>(), true);
    Q_ASSERT(page.isMissing());
    Q_ASSERT(!page.isNormalized());
    return page;
}

QueryImages::Page constructsPageMissingNormalized(unsigned int namespaceId,
                                                  QString const & title,
                                                  QString const & titleNoNormalized)
{
    unsigned int pageId;
    QueryImages::Page const page(pageId, namespaceId, title, titleNoNormalized, QList<QueryImages::Image>(), true);
    Q_ASSERT(page.isMissing());
    Q_ASSERT(page.isNormalized());
    return page;
}

bool operator==(QueryImages::Image const & lhs, QueryImages::Image const & rhs) {
    return lhs.namespaceId() == rhs.namespaceId() &&
           lhs.title() == rhs.title();
}

bool operator==(QueryImages::Page const & lhs, QueryImages::Page const & rhs) {
    return lhs.isNormalized() == rhs.isNormalized() &&
           lhs.title() == rhs.title() &&
           lhs.images() == rhs.images() &&
           lhs.isNormalized() == rhs.isNormalized() &&
           lhs.titleNoNormalized() == rhs.titleNoNormalized() &&
           lhs.isMissing() == rhs.isMissing() &&
           (lhs.isMissing() /* && rhs.isMissing() */ ? true : lhs.pageId() == rhs.pageId());
}

class QueryImagesTest : public QObject {

    Q_OBJECT

public slots:

    void pagesHandle(QList<QueryImages::Page> const & pages) {
        pagesReceivedList.push_back(pages);
    }

private slots:

    void init() {
        pagesReceivedList.clear();
    }

    void testConstructorTitleOneSignal() {
        QFETCH(QString, scenario);
        QFETCH(QString, title);
        QFETCH(QList<QueryImages::Page>, pagesExpected);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryImages * job = new QueryImages(mediawiki, title);

        connect(job, SIGNAL(pages(QList<QueryImages::Page> const &)), this, SLOT(pagesHandle(QList<QueryImages::Page> const &)));

        job->exec();

        QCOMPARE(job->error(), int(KJob::NoError));

        QList<FakeServer::Request> const requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request const request = requests[0];
        QCOMPARE(request.agent, mediawiki.userAgent());
        QCOMPARE(request.type, QString("GET"));
        QCOMPARE(request.value, QString("?format=xml&action=query&titles=" + title + "&prop=images&imlimit=10"));

        QCOMPARE(pagesReceivedList.size(), 1);

        QCOMPARE(pagesReceivedList[0], pagesExpected);

        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testConstructorTitleOneSignal_data() {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<QString>("title");
        QTest::addColumn<QList<QueryImages::Page> >("pagesExpected");

        QTest::newRow("Empty title")
                << "<?xml version=\"1.0\"?><api />"
                << ""
                << QList<QueryImages::Page>();

        QTest::newRow("Page with no image")
                << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"736\" ns=\"1\" title=\"Title-1\"></page></pages></query></api>"
                << "Title-1"
                << (QList<QueryImages::Page>()
                        << constructsPage(736u, 1u, "Title-1", QList<QueryImages::Image>()));
        
        QTest::newRow("Page with one image")
                << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"736\" ns=\"1\" title=\"Title-1\"><images><im ns=\"46\" title=\"File:Image-1-1\" /></images></page></pages></query></api>"
                << "Title-1"
                << (QList<QueryImages::Page>()
                        << constructsPage(736u, 1u, "Title-1", QList<QueryImages::Image>()
                                << QueryImages::Image(46u, "File:Image-1-1")));

        QTest::newRow("Page with two images")
                << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"736\" ns=\"1\" title=\"Title-1\"><images><im ns=\"46\" title=\"File:Image-1-1\" /><im ns=\"9997\" title=\"File:Image-1-2\" /></images></page></pages></query></api>"
                << "Title-1"
                << (QList<QueryImages::Page>()
                        << constructsPage(736u, 1u, "Title-1", QList<QueryImages::Image>()
                                << QueryImages::Image(46u, "File:Image-1-1")
                                << QueryImages::Image(9997u, "File:Image-1-2")));

        QTest::newRow("Page missing")
                << "<?xml version=\"1.0\"?><api><query><pages><page ns=\"0\" title=\"Title-Missing\" missing=\"\" /></pages></query></api>"
                << "Title-Missing"
                << (QList<QueryImages::Page>()
                        << constructsPageMissing(0u, "Title-Missing"));

        QTest::newRow("Normalize page with two images")
                << "<?xml version=\"1.0\"?><api><query><normalized><n from=\"Title_Normalized\" to=\"Title Normalized\" /></normalized><pages><page pageid=\"4\" ns=\"19282\" title=\"Title Normalized\"><images><im ns=\"46\" title=\"File:Image-1-1\" /><im ns=\"9997\" title=\"File:Image-1-2\" /></images></page></pages></query></api>"
                << "Title_Normalized"
                << (QList<QueryImages::Page>()
                        << constructsPageNormalized(4u, 19282u, "Title Normalized", "Title_Normalized", QList<QueryImages::Image>()
                                << QueryImages::Image(46u, "File:Image-1-1")
                                << QueryImages::Image(9997u, "File:Image-1-2")));

        QTest::newRow("Page missing normalized")
                << "<?xml version=\"1.0\"?><api><query><normalized><n from=\"Title_Missing_Normalized\" to=\"Title Missing Normalized\" /></normalized><pages><page ns=\"0\" title=\"Title Missing Normalized\" missing=\"\" /></pages></query></api>"
                << "Title_Missing_Normalized"
                << (QList<QueryImages::Page>()
                        << constructsPageMissingNormalized(0u, "Title Missing Normalized", "Title_Missing_Normalized"));                
    }

    void testConstructorTitleSeveralSignals() {
        QFETCH(QList<QString>, scenarios);
        QFETCH(QString, title);
        QFETCH(unsigned int, limit);
        QFETCH(QList<QList<QueryImages::Page> >, pagesExpectedList);

        // Constructs the fakeserver
        FakeServer fakeserver;
        for (int i = 0; i < scenarios.size(); ++i) {
            if (i == 0) {
                fakeserver.setScenario(scenarios[i]);
            } else {
                fakeserver.addScenario(scenarios[i]);
            }
        }
        fakeserver.startAndWait();

        // Prepare the job
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryImages * job = new QueryImages(mediawiki, title, limit);
        connect(job, SIGNAL(pages(QList<QueryImages::Page> const &)), this, SLOT(pagesHandle(QList<QueryImages::Page> const &)));
        job->exec();

        // Test job
        QCOMPARE(job->error(), int(KJob::NoError));

        // Test requests sent
        QList<FakeServer::Request> const requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), pagesExpectedList.size());
        for (int i = 0; i < requests.size(); ++i) {
            QCOMPARE(requests[i].agent, mediawiki.userAgent());
            QCOMPARE(requests[i].type, QString("GET"));
            if (i == 0) {
                QCOMPARE(requests[i].value, QString("?format=xml&action=query&titles=") + title + QString("&prop=images&imlimit=") + QString::number(limit));
            } else {
                QCOMPARE(requests[i].value, QString("?format=xml&action=query&titles=") + title + QString("&prop=images&imlimit=") + QString::number(limit) + QString("&imcontinue=") + QString::number(pagesExpectedList[i][0].pageId()) + QString("%7C") + pagesExpectedList[i][0].images()[0].title().remove(0, 5));
            }
        }

        // Test pages received
        QCOMPARE(pagesReceivedList, pagesExpectedList);

        // Test fakeserver
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testConstructorTitleSeveralSignals_data() {
        QTest::addColumn<QList<QString> >("scenarios");
        QTest::addColumn<QString>("title");
        QTest::addColumn<unsigned int>("limit");
        QTest::addColumn<QList<QList<QueryImages::Page> > >("pagesExpectedList");

        QTest::newRow("Page with three images by two signals")
                << (QList<QString>()
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"1234\" ns=\"5757\" title=\"Title-2\"><images><im ns=\"8\" title=\"File:Image-2-1\" /><im ns=\"8998\" title=\"File:Image-2-2\" /></images></page></pages></query><query-continue><images imcontinue=\"1234|Image-2-3\" /></query-continue></api>"
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"1234\" ns=\"5757\" title=\"Title-2\"><images><im ns=\"38423283\" title=\"File:Image-2-3\" /></images></page></pages></query></api>")
                << "Title-2"
                << 2u
                << (QList<QList<QueryImages::Page> >()
                        << (QList<QueryImages::Page>()
                                << constructsPage(1234u, 5757u, "Title-2", QList<QueryImages::Image>()
                                        << QueryImages::Image(8u, "File:Image-2-1")
                                        << QueryImages::Image(8998u, "File:Image-2-2")))
                        << (QList<QueryImages::Page>()
                                << constructsPage(1234u, 5757u, "Title-2", QList<QueryImages::Image>()
                                        << QueryImages::Image(38423283u, "File:Image-2-3"))));


        QTest::newRow("Page with six images by three signals")
                << (QList<QString>()
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"1234\" ns=\"5757\" title=\"Title-3\"><images><im ns=\"8\" title=\"File:Image-3-1\" /><im ns=\"8998\" title=\"File:Image-3-2\" /></images></page></pages></query><query-continue><images imcontinue=\"1234|Image-3-3\" /></query-continue></api>"
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"1234\" ns=\"5757\" title=\"Title-3\"><images><im ns=\"12\" title=\"File:Image-3-3\" /><im ns=\"4\" title=\"File:Image-3-4\" /></images></page></pages></query><query-continue><images imcontinue=\"1234|Image-3-5\" /></query-continue></api>"
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"1234\" ns=\"5757\" title=\"Title-3\"><images><im ns=\"988988\" title=\"File:Image-3-5\" /><im ns=\"452\" title=\"File:Image-3-6\" /></images></page></pages></query></api>")
                << "Title-3"
                << 2u
                << (QList<QList<QueryImages::Page> >()
                        << (QList<QueryImages::Page>()
                                << constructsPage(1234u, 5757u, "Title-3", QList<QueryImages::Image>()
                                        << QueryImages::Image(8u, "File:Image-3-1")
                                        << QueryImages::Image(8998u, "File:Image-3-2")))
                        << (QList<QueryImages::Page>()
                                << constructsPage(1234u, 5757u, "Title-3", QList<QueryImages::Image>()
                                        << QueryImages::Image(12u, "File:Image-3-3")
                                        << QueryImages::Image(4u, "File:Image-3-4")))
                        << (QList<QueryImages::Page>()
                                << constructsPage(1234u, 5757u, "Title-3", QList<QueryImages::Image>()
                                        << QueryImages::Image(988988u, "File:Image-3-5")
                                        << QueryImages::Image(452u, "File:Image-3-6"))));
    }

private:

    QList<QList<QueryImages::Page> > pagesReceivedList;

};

QTEST_MAIN(QueryImagesTest);

#include "queryimagestest.moc"
