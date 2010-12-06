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

    void imageinfosHandle(QList<QueryImageinfo::Imageinfo> const & imageinfos) {
        imageinfosReceived.push_back(imageinfos);
    }

private slots:

    void init() {
        imageinfosReceived = QList<QList<QueryImageinfo::Imageinfo> >();
    }

    void testConstructorTitle() {
        // Constructs the fakeserver
        FakeServer fakeserver;
        fakeserver.setScenario("<?xml version=\"1.0\"?><api><query><pages><page ns=\"6\" title=\"File:Image.bmp\" missing=\"\" imagerepository=\"shared\"><imageinfo><ii timestamp=\"2008-06-06T22:27:45Z\" user=\"User\" size=\"448798\" width=\"924\" height=\"1203\" url=\"http://url/File:Image.bmp\" descriptionurl=\"http://descriptionurl/File:Image.bmp\" comment=\"Comment\" /></imageinfo></page></pages></query></api>");
        fakeserver.startAndWait();

        // Prepare the job
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryImageinfo * job = new QueryImageinfo(mediawiki, "File:Image.bmp");
        job->paramProperties(QueryImageinfo::ALL_PROPERTIES);
        connect(job, SIGNAL(imageinfos(QList<QueryImageinfo::Imageinfo> const &)), this, SLOT(imageinfosHandle(QList<QueryImageinfo::Imageinfo> const &)));
        job->exec();

        // Test job
        QCOMPARE(job->error(), int(KJob::NoError));

        // Test fakeserver
        QVERIFY(fakeserver.isAllScenarioDone());

        // Test requests sent
        QList<FakeServer::Request> const requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);
        QCOMPARE(requests[0].agent, mediawiki.userAgent());
        QCOMPARE(requests[0].type, QString("GET"));
        QCOMPARE(requests[0].value, QString("?format=xml&action=query&titles=File:Image.bmp&prop=imageinfo&iiprop=timestamp%7Cuser%7Ccomment%7Curl%7Csize"));

        // Test pages received
        QList<QList<QueryImageinfo::Imageinfo> > imageinfosExpected;
        imageinfosExpected.push_back(QList<QueryImageinfo::Imageinfo>()
            << QueryImageinfo::Imageinfo(QDateTime(QDate(2008, 06, 06), QTime(22, 27, 45, 0)),
                                         QString("User"),
                                         QString("Comment"),
                                         QUrl("http://url/File:Image.bmp"),
                                         QUrl("http://descriptionurl/File:Image.bmp"),
                                         448798u,
                                         924u,
                                         1203u,
                                         QueryImageinfo::ALL_PROPERTIES)
        );
        QCOMPARE(imageinfosReceived, imageinfosExpected);
    }

private:

    QList<QList<QueryImageinfo::Imageinfo> > imageinfosReceived;

};

QTEST_MAIN(QueryImageinfoTest);

#include "queryimageinfotest.moc"
