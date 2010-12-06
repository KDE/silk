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

#ifndef TEST_EDIT_H
#define TEST_EDIT_H

#include <QObject>
#include <QtTest/QtTest>
#include <KJob>

#include "mediawiki.h"
#include "edit.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::Edit;

Q_DECLARE_METATYPE(FakeServer::Request)
Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(Edit::Result)

class EditTest : public QObject
{
    Q_OBJECT

signals:

    void captchaSignal(QString  const & captchaword);

public slots:

    void editHandle(QVariant const & captcha) {
        editCount++;
        this->captchaquestionorurl = captcha;
        emit captchaSignal(this->captchaword);
    }

private slots:

    void init() {
        editCount = 0;
    }

    void testResult() {
        QFETCH(QString, scenario1);
        QFETCH(QString, scenario2);
        QFETCH(FakeServer::Request, requestTrue);
        QFETCH(int, error);
        QFETCH(QVariant, word);
        QFETCH(QString, answer);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario1);
        fakeserver.addScenario(scenario2);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        Edit * job = new Edit( mediawiki, "Talk:Main_Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!", "new", "Hello World" );

        connect(job, SIGNAL( resultCaptcha(QVariant const &) ), this, SLOT( editHandle(QVariant const &) ));
        connect(this, SIGNAL( captchaSignal(QString const &) ), job, SLOT( finishedCaptcha(QString const &) ));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 2);

        FakeServer::Request request = requests[0];
        QCOMPARE(request.agent, mediawiki.userAgent());
        QCOMPARE(request.type, QString("POST"));

        QCOMPARE(job->error(), error);
        QCOMPARE(editCount, 1);
        QCOMPARE(requestTrue.value, request.value);
        QCOMPARE(captchaquestionorurl, word);
        QCOMPARE(captchaword, answer);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testResult_data() {
        QTest::addColumn<QString>("scenario1");
        QTest::addColumn<QString>("scenario2");
        QTest::addColumn<FakeServer::Request>("requestTrue");
        QTest::addColumn<int>("error");
        QTest::addColumn< QVariant >("word");
        QTest::addColumn< QString >("answer");

        Edit::Result captcha;
        captcha.captchaid = QString("509895952").toUInt();
        captcha.captchaquestionorurl = QVariant("36 + 4 = ");
        captcha.captchaword = QString("40");
        this->captchaword = captcha.captchaword;

        QTest::newRow("Test: 1")
                << "<api><edit result=\"Failure\"><captcha type=\"math\" mime=\"text/tex\" id=\"509895952\" question=\"36 + 4 = \" /></edit></api>"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << FakeServer::Request("POST","","?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main_Page&token=cecded1f35005d22904a35cc7b736e18%2B%5C")
                << int(KJob::NoError)
                << captcha.captchaquestionorurl
                << captcha.captchaword;
    }

private:

    int editCount;
    QVariant captchaquestionorurl;
    QString captchaword;
};


QTEST_MAIN(EditTest);
#include "edittest.moc"
#endif // TEST_EDIT_H

