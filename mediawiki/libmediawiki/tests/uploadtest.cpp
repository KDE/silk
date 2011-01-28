/*
 *   Copyright 2011 by Alexandre Mendes <alex.mendes1988@gmail.com>
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

#ifndef TEST_UPLOAD_H
#define TEST_UPLOAD_H

#include <QObject>
#include <QtTest/QtTest>
#include <KJob>

#include "mediawiki.h"
#include "upload.h"
#include "login.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::Upload;
using mediawiki::Login;

Q_DECLARE_METATYPE(FakeServer::Request)
Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(Upload*)
Q_DECLARE_METATYPE(Login*)

class UploadTest : public QObject
{
    Q_OBJECT

public slots:

    void uploadHandle(KJob* ) {
        uploadCount++;
    }

private slots:

//    void initTestCase()
//    {
//        uploadCount = 0;
//        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
//        this->m_infoScenario = "<api><query><pages><page pageid=\"1\" ns=\"0\" title=\"Foo\" touched=\"2000-01-01T00:00:00Z\" lastrevid=\"1\" counter=\"0\" length=\"0\"edittoken=\"+\\\" /></pages></query></api>";
//    }

    void test()
    {
       MediaWiki mediawiki(QUrl("http://en.wikipedia.org/w/api.php"));

       Login *login = new Login(mediawiki, "iup", "isi");
       login->exec();
       qDebug()<<"Login "<<login->error();
       Upload * e1 = new Upload( mediawiki, NULL);
//       QFile file("Test.jpeg");
//       file.open(QIODevice::ReadOnly);
//       e1->setFile(file.readAll());
//       file.close();
       e1->setFile(QByteArray("test"));
       e1->setFilename("Test.jpeg");
       e1->exec();
    }

//    void uploadSetters()
//    {
//        QFETCH(QString, request);
//        QFETCH(QString, senario);
//        QFETCH(Upload*, job);
//
//        uploadCount = 0;
//        FakeServer fakeserver;
//        fakeserver.setScenario(m_infoScenario);
//        fakeserver.addScenario(senario);
//        fakeserver.startAndWait();
//        connect(job, SIGNAL(result(KJob* )),this, SLOT(uploadHandle(KJob*)));
//        job->exec();
//        FakeServer::Request serverrequest = fakeserver.getRequest()[1];
//        QCOMPARE(serverrequest.type, QString("POST"));
//        QCOMPARE(serverrequest.value, request);
//        QCOMPARE(job->error(), (int)Upload::NoError);
//        QCOMPARE(this->uploadCount, 1);
//    }
//    void uploadSetters_data()
//    {
//        QTest::addColumn<QString>("request");
//        QTest::addColumn<QString>("senario");
//        QTest::addColumn<Upload*>("job");
//
//        Upload * e1 = new Upload( *m_mediaWiki, NULL);
//        e1->setFile(QFile());
//        e1->setFilename("Test.txt");
//        e1->setComment("Test");
//        QTest::newRow("Text")
//                << "?format=xml&action=upload&file=&filename=Test.txt&token=cecded1f35005d22904a35cc7b736e18+\\"
//                << "<api><upload result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
//                << e1;
//    }
//
//    void error()
//    {
//        QFETCH(QString, scenario);
//        QFETCH(int, error);
//
//        uploadCount = 0;
//        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
//        FakeServer fakeserver;
//        if(scenario != QString("error server"))
//        {
//            fakeserver.setScenario(m_infoScenario);
//            fakeserver.addScenario(scenario);
//            fakeserver.startAndWait();
//        }
//
//        Upload * job = new Upload(mediawiki, NULL);
//        e1->setFile(QFile());
//        e1->setFilename("Test.txt");
//        e1->setComment("Test");
//        connect(job,  SIGNAL(result(KJob* )),this, SLOT(uploadHandle(KJob*)));
//
//        job->exec();
//
//        if(scenario != QString("error server"))
//        {
//            QList<FakeServer::Request> requests = fakeserver.getRequest();
//            QCOMPARE(requests.size(), 2);
//        }
//        QCOMPARE(job->error(), error);
//        QCOMPARE(uploadCount, 1);
//
//        if(scenario != QString("error server"))
//        {
//            QVERIFY(fakeserver.isAllScenarioDone());
//        }
//    }
//    void error_data()
//    {
//        QTest::addColumn<QString>("scenario");
//        QTest::addColumn<int>("error");
//
//        QTest::newRow("UploadDisabled")
//                << "<api><upload result=\"Failure\"><error code=\"upload-disabled\" info=\"\" /></upload> </api>"
//                << int(Upload::UploadDisabled);
//
//        QTest::newRow("InvalidSessionKey")
//                << "<api><upload result=\"Failure\"><error code=\"invalid-session-key\" info=\"\" /></upload> </api>"
//                << int(Upload::InvalidSessionKey);
//
//        QTest::newRow("BadAccess")
//                << "<api><upload result=\"Failure\"><error code=\"bad-access-groups\" info=\"\" /></upload> </api>"
//                << int(Upload::BadAccess);
//
//        QTest::newRow("ParamMissing")
//                << "<api><upload result=\"Failure\"><error code=\"missing-param\" info=\"\" /></upload> </api>"
//                << int(Upload::ParamMissing);
//
//        QTest::newRow("MustBeLoggedIn")
//                << "<api><upload result=\"Failure\"><error code=\"mustbeloggedin\" info=\"\" /></upload> </api>"
//                << int(Upload::MustBeLoggedIn);
//
//        QTest::newRow("FetchFileError")
//                << "<api><upload result=\"Failure\"><error code=\"fetchfile-error\" info=\"\" /></upload> </api>"
//                << int(Upload::FetchFileError);
//
//        QTest::newRow("NoModule")
//                << "<api><upload result=\"Failure\"><error code=\"no-module\" info=\"\" /></upload> </api>"
//                << int(Upload::NoModule);
//
//        QTest::newRow("EmptyFile")
//                << "<api><upload result=\"Failure\"><error code=\"emptyfile\" info=\"\" /></upload> </api>"
//                << int(Upload::EmptyFile);
//
//        QTest::newRow("ExtensionMissing")
//                << "<api><upload result=\"Failure\"><error code=\"filetype-missing\" info=\"\" /></upload> </api>"
//                << int(Upload::ExtensionMissing);
//
//        QTest::newRow("TooShortFilename")
//                << "<api><upload result=\"Failure\"><error code=\"filenametooshort\" info=\"\" /></upload> </api>"
//                << int(Upload::TooShortFilename);
//
//        QTest::newRow("OverWriting")
//                << "<api><upload result=\"Failure\"><error code=\"overwrite\" info=\"\" /></upload> </api>"
//                << int(Upload::OverWriting);
//
//        QTest::newRow("StashFailed")
//                << "<api><upload result=\"Failure\"><error code=\"stashfailed\" info=\"\" /></upload> </api>"
//                << int(Upload::StashFailed);
//
//        QTest::newRow("InternalError")
//                << "<api><upload result=\"Failure\"><error code=\"internal-error\" info=\"\" /></upload> </api>"
//                << int(Upload::InternalError);
//    }
//
//    void cleanupTestCase()
//    {
//        delete this->m_mediaWiki;
//    }

private:

    int uploadCount;
    QString request;
    QString m_infoScenario;
    MediaWiki* m_mediaWiki;
};


QTEST_MAIN(UploadTest);
#include "uploadtest.moc"
#endif // TEST_UPLOAD_H


