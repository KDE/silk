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
#include "image.h"

using mediawiki::MediaWiki;
using mediawiki::QueryImages;
using mediawiki::Image;

Q_DECLARE_METATYPE(QList<QString>);
Q_DECLARE_METATYPE(QList<Image::Image>);
Q_DECLARE_METATYPE(QList<QueryImages::Page>);
Q_DECLARE_METATYPE(QList<QList<QueryImages::Page> >);

QueryImages::Page constructsPage(unsigned int pageId,
                                 unsigned int namespaceId,
                                 const QString & title,
                                 const QList<Image::Image> & images)
{
    const QueryImages::Page page(pageId, namespaceId, title, title, images, false);
    Q_ASSERT(!page.isMissing());
    Q_ASSERT(!page.isNormalized());
    return page;
}

QueryImages::Page constructsPageNormalized(unsigned int pageId,
                                           unsigned int namespaceId,
                                           const QString& title,
                                           const QString& titleNoNormalized,
                                           const QList<Image::Image> & images)
{
    const QueryImages::Page page(pageId, namespaceId, title, titleNoNormalized, images, false);
    Q_ASSERT(!page.isMissing());
    Q_ASSERT(page.isNormalized());
    return page;
}

QueryImages::Page constructsPageMissing(unsigned int namespaceId,
                                        const QString & title)
{
    unsigned int pageId;
    const QueryImages::Page page(pageId, namespaceId, title, title, QList<Image::Image>(), true);
    Q_ASSERT(page.isMissing());
    Q_ASSERT(!page.isNormalized());
    return page;
}

QueryImages::Page constructsPageMissingNormalized(unsigned int namespaceId,
                                                  const QString& title,
                                                  const QString& titleNoNormalized)
{
    unsigned int pageId;
    const QueryImages::Page page(pageId, namespaceId, title, titleNoNormalized, QList<Image::Image>(), true);
    Q_ASSERT(page.isMissing());
    Q_ASSERT(page.isNormalized());
    return page;
}

bool operator==(const QueryImages::Page & lhs, const QueryImages::Page & rhs) {
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

    void pagesHandle(const QList<QueryImages::Page> & pages) {
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
        QueryImages * job = new QueryImages(mediawiki);
        job->setTitle(title);

        connect(job, SIGNAL(pages(const QList<QueryImages::Page> &)), this, SLOT(pagesHandle(const QList<QueryImages::Page> &)));

        job->exec();

        QCOMPARE(job->error(), int(KJob::NoError));

        const QList<FakeServer::Request> requests = fakeserver.getRequest();
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
        Image image, image2;

        QTest::newRow("Empty title")
                << "<?xml version=\"1.0\"?><api />"
                << ""
                << QList<QueryImages::Page>();

        QTest::newRow("Page with no image")
                << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"736\" ns=\"1\" title=\"Title-1\"></page></pages></query></api>"
                << "Title-1"
                << (QList<QueryImages::Page>()
                        << constructsPage(736u, 1u, "Title-1", QList<Image::Image>()));
        
        image.setNamespaceId(46u);
        image.setTitle("File:Image-1-1");
        QTest::newRow("Page with one image")
                << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"736\" ns=\"1\" title=\"Title-1\"><images><im ns=\"46\" title=\"File:Image-1-1\" /></images></page></pages></query></api>"
                << "Title-1"
                << (QList<QueryImages::Page>()
                        << constructsPage(736u, 1u, "Title-1", QList<Image::Image>()
                                << image));

        image2.setNamespaceId(9997u);
        image2.setTitle("File:Image-1-2");
        QTest::newRow("Page with two images")
                << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"736\" ns=\"1\" title=\"Title-1\"><images><im ns=\"46\" title=\"File:Image-1-1\" /><im ns=\"9997\" title=\"File:Image-1-2\" /></images></page></pages></query></api>"
                << "Title-1"
                << (QList<QueryImages::Page>()
                        << constructsPage(736u, 1u, "Title-1", QList<Image::Image>()
                                << image
                                << image2));

        QTest::newRow("Page missing")
                << "<?xml version=\"1.0\"?><api><query><pages><page ns=\"0\" title=\"Title-Missing\" missing=\"\" /></pages></query></api>"
                << "Title-Missing"
                << (QList<QueryImages::Page>()
                        << constructsPageMissing(0u, "Title-Missing"));

        QTest::newRow("Normalize page with two images")
                << "<?xml version=\"1.0\"?><api><query><normalized><n from=\"Title_Normalized\" to=\"Title Normalized\" /></normalized><pages><page pageid=\"4\" ns=\"19282\" title=\"Title Normalized\"><images><im ns=\"46\" title=\"File:Image-1-1\" /><im ns=\"9997\" title=\"File:Image-1-2\" /></images></page></pages></query></api>"
                << "Title_Normalized"
                << (QList<QueryImages::Page>()
                        << constructsPageNormalized(4u, 19282u, "Title Normalized", "Title_Normalized", QList<Image::Image>()
                                << image
                                << image2));

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
        QueryImages * job = new QueryImages(mediawiki);
        job->setTitle(title);
        job->setLimit(limit);
        connect(job, SIGNAL(pages(const QList<QueryImages::Page> &)), this, SLOT(pagesHandle(const QList<QueryImages::Page> &)));
        job->exec();

        // Test job
        QCOMPARE(job->error(), int(KJob::NoError));

        // Test requests sent
        const QList<FakeServer::Request> requests = fakeserver.getRequest();
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
        Image image, image2, image3;

        image.setNamespaceId(8u);
        image.setTitle("File:Image-2-1");

        image2.setNamespaceId(8998u);
        image2.setTitle("File:Image-2-2");

        image3.setNamespaceId(38423283u);
        image3.setTitle("File:Image-2-3");

        QTest::newRow("Page with three images by two signals")
                << (QList<QString>()
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"1234\" ns=\"5757\" title=\"Title-2\"><images><im ns=\"8\" title=\"File:Image-2-1\" /><im ns=\"8998\" title=\"File:Image-2-2\" /></images></page></pages></query><query-continue><images imcontinue=\"1234|Image-2-3\" /></query-continue></api>"
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"1234\" ns=\"5757\" title=\"Title-2\"><images><im ns=\"38423283\" title=\"File:Image-2-3\" /></images></page></pages></query></api>")
                << "Title-2"
                << 2u
                << (QList<QList<QueryImages::Page> >()
                        << (QList<QueryImages::Page>()
                                << constructsPage(1234u, 5757u, "Title-2", QList<Image::Image>()
                                        << image
                                        << image2))
                        << (QList<QueryImages::Page>()
                                << constructsPage(1234u, 5757u, "Title-2", QList<Image::Image>()
                                        << image3)));

    }

private:

    QList<QList<QueryImages::Page> > pagesReceivedList;

};

QTEST_MAIN(QueryImagesTest);

#include "queryimagestest.moc"
