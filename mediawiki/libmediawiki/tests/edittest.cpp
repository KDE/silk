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

class EditTest : public QObject
{
    Q_OBJECT

public slots:

    void editHandle(KJob* job) {
        editCount++;
        editResults = ((Edit*)job)->getResults();
    }

private slots:

    void initTestCase()
    {
        editCount = 0;
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
        this->m_server = new FakeServer;
        this->request = "?format=xml&action=edit&lgname=alexTest&lgpassword=test";
    }

    void editTestConnectTrue()
    {
        editCount = 0;
        QString senario("<api><edit result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><edit result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /></api>";
        m_server->addScenario(senario, cookie);
        m_server->startAndWait();

        Edit edit(*m_mediaWiki, "alexTest", "test");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->editCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.cookies().isEmpty(), false);
        QVERIFY(edit.error() == Edit::NoError);
    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
        delete this->m_server;
    }

private:

    int editCount;
    Edit::Result editResults;
    QString request;
    MediaWiki* m_mediaWiki;
    FakeServer* m_server;
};


QTEST_MAIN(EditTest);
#include "edittest.moc"
#endif // TEST_EDIT_H

