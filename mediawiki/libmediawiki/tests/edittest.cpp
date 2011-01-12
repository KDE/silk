/*
 *   Copyright 2010 by Alexandre Mendes <alex.mendes1988@gmail.com>
 *   Copyright 2011 by Manuel Campomanes <campomanes.manuel@gmail.com>
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

class EditTest : public QObject
{
    Q_OBJECT

signals:

    void captchaSignal(QString  const & CaptchaAnswer);

public slots:

    void editHandle(KJob* ) {
        editCount++;
    }

    void editHandle(QVariant const & captcha) {
        editCount++;
        this->CaptchaQuestion = captcha;
        emit captchaSignal(this->CaptchaAnswer);
    }

private slots:
    void initTestCase()
    {
        editCount = 0;
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
    }

    void editTestTextTrue()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }

    void editTestApendPrependTrue()
    {
        editCount = 0;
        FakeServer fakeserver;
        this->request = "?format=xml&action=edit&appendtext=Hello%20everyone!&basetimestamp=2008-03-20T17:26:39Z&md5=4ade2c502d52ad05e7449c2470ba29d9&prependtext=Hello%20everyone!&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>");
        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        //connect(&edit, SIGNAL( resultCaptcha(QVariant const &) ), this, SLOT( editHandle(QVariant const &) ));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }

    void editTestPrependTrue()
    {
        editCount = 0;
        FakeServer fakeserver;
        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&prependtext=Hello%20everyone!&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>");
        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "", "Hello everyone!" );
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }

    void editTestApendTrue()
    {
        editCount = 0;
        FakeServer fakeserver;
        this->request = "?format=xml&action=edit&appendtext=Hello%20everyone!&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>");
        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!", "");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }

    void editTestUndoTrue()
    {
        editCount = 0;
        FakeServer fakeserver;
        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&title=Talk:Main%20Page&undo=13585&undoafter=13579&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>");
        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", 13585, 13579);
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }

    void editTestNotext()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=d41d8cd98f00b204e9800998ecf8427e&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"notext\" info=\"\" /> </api>");
        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::TextMissing);
        QCOMPARE(this->editCount, 1);
    }

    void editTestInvalidsection()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"invalidsection\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::InvalidSection);
        QCOMPARE(this->editCount, 1);
    }

    void editTestProtectedtitle()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"protectedtitle\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::TitleProtected);
        QCOMPARE(this->editCount, 1);
    }

    void editTestCantcreate()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"cantcreate\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::CreatePagePermissionMissing);
        QCOMPARE(this->editCount, 1);
    }

    void editTestCantcreateanon()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"cantcreate-anon\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::AnonymousCreatePagePermissionMissing);
        QCOMPARE(this->editCount, 1);
    }

    void editTestArticleexists()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"articleexists\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::ArticleDuplication);
        QCOMPARE(this->editCount, 1);
    }

    void editTestNoimageredirectanon()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"noimageredirect-anon\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::AnonymousCreateImagePermissionMissing);
        QCOMPARE(this->editCount, 1);
    }

    void editTestNoimageredirect()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"noimageredirect\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::CreateImagePermissionMissing);
        QCOMPARE(this->editCount, 1);
    }

    void editTestspamdetected()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"spamdetected\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::SpamDetected);
        QCOMPARE(this->editCount, 1);
    }

    void editTestfiltered()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"filtered\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::Filtered);
        QCOMPARE(this->editCount, 1);
    }

    void editTestcontenttoobig()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"contenttoobig\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::ArticleSizeExceed);
        QCOMPARE(this->editCount, 1);
    }

    void editTestnoeditanon()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"noedit-anon\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::AnonymousEditPagePermissionMissing);
        QCOMPARE(this->editCount, 1);
    }

    void editTestnoedit()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"noedit\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::EditPagePermissionMissing);
        QCOMPARE(this->editCount, 1);
    }

    void editTestpagedeleted()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"pagedeleted\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::PageDeleted);
        QCOMPARE(this->editCount, 1);
    }

    void editTestemptypage()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"emptypage\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::EmptyPage);
        QCOMPARE(this->editCount, 1);
    }

    void editTestemptynewsection()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"emptynewsection\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::EmptySection);
        QCOMPARE(this->editCount, 1);
    }

    void editTesteditconflict()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"editconflict\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::EditConflict);
        QCOMPARE(this->editCount, 1);
    }

    void editTestrevwrongpage()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"revwrongpage\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::RevWrongPage);
        QCOMPARE(this->editCount, 1);
    }

    void editTestundofailure()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><error code=\"undofailure\" info=\"\" /> </api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::UndoFailed);
        QCOMPARE(this->editCount, 1);
    }

    void editTestSetsWatch()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&createonly&md5=4d184ec6e8fe61abccb8ff62c4583cd0&minor&nocreate&recreate&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&watchlist=watch&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");
        edit.setRecreate();
        edit.setCreateonly();
        edit.setNocreate();
        edit.setMinor(true);
        edit.setWatchlist(Edit::watch);

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }

    void editTestSetsUnwatch()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&createonly&md5=4d184ec6e8fe61abccb8ff62c4583cd0&minor&nocreate&recreate&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&watchlist=unwatch&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");
        edit.setRecreate();
        edit.setCreateonly();
        edit.setNocreate();
        edit.setMinor(true);
        edit.setWatchlist(Edit::unwatch);

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }

    void editTestSetsPreferences()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&createonly&md5=4d184ec6e8fe61abccb8ff62c4583cd0&minor&nocreate&recreate&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&watchlist=preferences&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");
        edit.setRecreate();
        edit.setCreateonly();
        edit.setNocreate();
        edit.setMinor(true);
        edit.setWatchlist(Edit::preferences);

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }

    void editTestSetsNochange()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&createonly&md5=4d184ec6e8fe61abccb8ff62c4583cd0&minor&nocreate&recreate&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&watchlist=nochange&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>");

        fakeserver.setScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");
        edit.setRecreate();
        edit.setCreateonly();
        edit.setNocreate();
        edit.setMinor(true);
        edit.setWatchlist(Edit::nochange);

        connect(&edit, SIGNAL(result(KJob* )),this, SLOT(editHandle(KJob*)));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }


    void editTestTextCaptchaTrue()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B%255C";
        QString senario("<api><edit result=\"Failure\"><captcha type=\"math\" mime=\"text/tex\" id=\"509895952\" question=\"36 + 4 = \" /></edit></api>");

        fakeserver.setScenario(senario);
        senario = "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>";
        fakeserver.addScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, "Talk:Main Page", "cecded1f35005d22904a35cc7b736e18+\\", "2008-03-20T17:26:39Z", "2008-03-27T21:15:39Z", "Hello everyone!");
        edit.setSection("new");
        edit.setSummary("Hello World");

        connect(&edit, SIGNAL( resultCaptcha(QVariant const &) ), this, SLOT( editHandle(QVariant const &) ));
        connect(this, SIGNAL( captchaSignal(QString const &) ), &edit, SLOT( finishedCaptcha(QString const &) ));
        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 2);
    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
    }

private:

    int editCount;
    QVariant CaptchaQuestion;
    QString CaptchaAnswer;
    QString request;
    MediaWiki* m_mediaWiki;
};


QTEST_MAIN(EditTest);
#include "edittest.moc"
#endif // TEST_EDIT_H

