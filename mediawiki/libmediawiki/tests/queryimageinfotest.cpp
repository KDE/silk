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
#include "queryimageinfo.h"

using mediawiki::MediaWiki;
using mediawiki::QueryImageinfo;

class QueryImageinfoTest : public QObject {

    Q_OBJECT

public slots:

    void imagesHandle(const QList<QueryImageinfo::Image> & images) {
        imagesReceived.push_back(images);
    }

private slots:

    void init() {
        imagesReceived = QList<QList<QueryImageinfo::Image> >();
    }

    void testConstructorTitle() {
        // Constructs the fakeserver
        FakeServer fakeserver;
        fakeserver.setScenario("<?xml version=\"1.0\"?><api><query><normalized><n from=\"Image:Image.bmp\" to=\"File:Image.bmp\" /></normalized><pages><page ns=\"6\" title=\"File:Image.bmp\" missing=\"\" imagerepository=\"shared\"><imageinfo><ii timestamp=\"2008-06-06T22:27:45Z\" user=\"User1\" size=\"448798\" width=\"924\" height=\"1203\" url=\"http://url/File:Image.bmp\" thumburl=\"http://thumburl/File:Image.bmp\" thumbwidth=\"78\" thumbheight=\"102\" descriptionurl=\"http://descriptionurl/File:Image.bmp\" comment=\"Comment1\" sha1=\"00be23585fde01190a0f8c60fc4267ea00f3745d\" mime=\"image/bmp\"><metadata><metadata name=\"Name1\" value=\"Value1\" /><metadata name=\"Name2\" value=\"Value2\" /></metadata></ii></imageinfo></page></pages></query><query-continue><imageinfo iistart=\"2007-06-06T22:27:45Z\" /></query-continue></api>");
        fakeserver.addScenario("<?xml version=\"1.0\"?><api><query><normalized><n from=\"Image:Image.bmp\" to=\"File:Image.bmp\" /></normalized><pages><page ns=\"6\" title=\"File:Image.bmp\" missing=\"\" imagerepository=\"shared\"><imageinfo><ii timestamp=\"2007-06-06T22:27:45Z\" user=\"User2\" size=\"448798\" width=\"924\" height=\"1203\" url=\"http://url/File:Image.bmp\" descriptionurl=\"http://descriptionurl/File:Image.bmp\" comment=\"Comment2\" sha1=\"00be23585fde01190a0f8c60fc4267ea00f3745d\" mime=\"image/bmp\"><metadata><metadata name=\"Name1\" value=\"Value1\" /><metadata name=\"Name2\" value=\"Value2\" /></metadata></ii></imageinfo></page></pages></query></api>");
        fakeserver.startAndWait();

        // Prepare the job
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryImageinfo * job = new QueryImageinfo(mediawiki, "Image:Image.bmp");
        job->paramProperties(QueryImageinfo::ALL_PROPERTIES);
        job->paramLimit(1u, false);
        job->paramStart(QDateTime(QDate(2008, 06, 06), QTime(22, 27, 45, 0)));
        job->paramEnd(QDateTime(QDate(2007, 06, 06), QTime(22, 27, 45, 0)));
        job->paramScale(78u, 102u);
        connect(job, SIGNAL(images(const QList<QueryImageinfo::Image> &)), this, SLOT(imagesHandle(const QList<QueryImageinfo::Image> &)));
        job->exec();

        // Test job
        QCOMPARE(job->error(), int(KJob::NoError));

        // Test fakeserver
        QVERIFY(fakeserver.isAllScenarioDone());

        // Test requests sent
        const QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 2);
        for (unsigned int i = 0; i < 2; ++i) {
            QCOMPARE(requests[i].agent, mediawiki.userAgent());
            QCOMPARE(requests[i].type, QString("GET"));
        }
        QCOMPARE(requests[0].value, QString("?format=xml&action=query&titles=Image:Image.bmp&prop=imageinfo&iiprop=timestamp%7Cuser%7Ccomment%7Curl%7Csize%7Csha1%7Cmime%7Cmetadata&iilimit=1&iistart=2008-06-06T22:27:45Z&iiend=2007-06-06T22:27:45Z&iiurlwidth=78&iiurlheight=102"));
        QCOMPARE(requests[1].value, QString("?format=xml&action=query&titles=Image:Image.bmp&prop=imageinfo&iiprop=timestamp%7Cuser%7Ccomment%7Curl%7Csize%7Csha1%7Cmime%7Cmetadata&iilimit=1&iistart=2007-06-06T22:27:45Z&iiend=2007-06-06T22:27:45Z&iiurlwidth=78&iiurlheight=102"));

        // Test pages received
        QList<QList<QueryImageinfo::Image> > imagesExpected;
        QHash<QString, QVariant> metadata;
        metadata["Name1"] = "Value1";
        metadata["Name2"] = "Value2";
        imagesExpected.push_back(QList<QueryImageinfo::Image>()
            << (QueryImageinfo::Image(6, "File:Image.bmp", "Image:Image.bmp", "shared", QVector<QueryImageinfo::Imageinfo>()
                << (QueryImageinfo::Imageinfo(QDateTime(QDate(2008, 06, 06), QTime(22, 27, 45, 0)),
                                             QString("User1"),
                                             QString("Comment1"),
                                             QUrl("http://url/File:Image.bmp"),
                                             QUrl("http://descriptionurl/File:Image.bmp"),
                                             QUrl("http://thumburl/File:Image.bmp"),
                                             78u,
                                             102u,
                                             448798u,
                                             924u,
                                             1203u,
                                             QString("00be23585fde01190a0f8c60fc4267ea00f3745d"),
                                             QString("image/bmp"),
                                             metadata,
                                             QueryImageinfo::ALL_PROPERTIES))))
        );
        imagesExpected.push_back(QList<QueryImageinfo::Image>()
            << (QueryImageinfo::Image(6, "File:Image.bmp", "Image:Image.bmp", "shared", QVector<QueryImageinfo::Imageinfo>()
                << (QueryImageinfo::Imageinfo(QDateTime(QDate(2007, 06, 06), QTime(22, 27, 45, 0)),
                                             QString("User2"),
                                             QString("Comment2"),
                                             QUrl("http://url/File:Image.bmp"),
                                             QUrl("http://descriptionurl/File:Image.bmp"),
                                             QUrl(),
                                             0u,
                                             0u,
                                             448798u,
                                             924u,
                                             1203u,
                                             QString("00be23585fde01190a0f8c60fc4267ea00f3745d"),
                                             QString("image/bmp"),
                                             metadata,
                                             QueryImageinfo::ALL_PROPERTIES))))
        );
        QCOMPARE(imagesReceived, imagesExpected);
    }

private:

    QList<QList<QueryImageinfo::Image> > imagesReceived;

};

QTEST_MAIN(QueryImageinfoTest);

#include "queryimageinfotest.moc"
