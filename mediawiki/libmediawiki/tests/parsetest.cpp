/*
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

#ifndef TEST_PARSE_H
#define TEST_PARSE_H

#include <QObject>
#include <QtTest/QtTest>
#include <KJob>

#include "mediawiki.h"
#include "parse.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::Parse;

Q_DECLARE_METATYPE(FakeServer::Request)
Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(Parse*)

class ParseTest : public QObject
{
    Q_OBJECT

public slots:

    void parseHandle(KJob* job) {
        parseCount++;
    }

private slots:

    void initTestCase()
    {
        parseCount = 0;
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
        this->m_infoScenario = QStringFromFile("./parsetest.rc");
    }    

    void parseSetters()
    {
        QFETCH(QString, request);
        QFETCH(QString, scenario);
        QFETCH(Parse*, job);

        parseCount = 0;
        FakeServer fakeserver;
        fakeserver.setScenario(m_infoScenario);
        fakeserver.addScenario(scenario);
        fakeserver.startAndWait();
        connect(job, SIGNAL(result(KJob* )),this, SLOT(parseHandle(KJob*)));
        job->exec();
        FakeServer::Request serverrequest = fakeserver.getRequest()[1];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, request);
        QCOMPARE(job->error(), (int)Parse::NoError);
        QCOMPARE(this->parseCount, 1);
    }

    void parseSetters_data()
    {
        QTest::addColumn<QString>("request");
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<Parse*>("job");

        Parse * p1 = new Parse( *m_mediaWiki, NULL);
        p1->setText("<p><a href=\"/wiki/Foo\" title=\"Foo\" class=\"mw-redirect\">foo</a> <a href=\"/w/index.php?title=API:Query&amp;action=edit&amp;redlink=1\" class=\"new\" title=\"API:Query (page does not exist)\">bar</a> <a href=\"http://www.example.com/\" class=\"external text\" rel=\"nofollow\">baz</a></p><!--NewPP limit report Preprocessor node count: 1/1000000 Post-expand include size: 0/2048000 bytes Template argument size: 0/2048000 bytes Expensive parser function count: 0/500-->");
        p1->setTitle("titreParse");        
        p1->setPropText("<p><a href=\"/wiki/Foo\" title=\"Foo\" class=\"mw-redirect\">foo</a> <a href=\"/w/index.php?title=API:Query&amp;action=edit&amp;redlink=1\" ");
        p1->setPropLanglinks("http://www.mediawiki.org/wiki/API:Parsing_wikitext");
        p1->setPropcategories("language categories");
        p1->setPropLinks("<pl ns=\"0\" exists=\"\" xml:space=\"preserve\">Case sensitivity</pl><pl ns=\"4\" exists=\"\" xml:space=\"preserve\">Wikipedia:Article wizard</pl>");
        p1->setPropTemplates("templates");
        p1->setPropImages("<img>Wiktionary-logo-en.png</img><img>Wikibooks-logo.svg</img>");
        p1->setPropExternallinks("http://www.mediawiki.org/wiki/API:Parsing_wikitext");
        p1->setPropSections("sections");
        p1->setPropRevid("34156367");
        p1->setPst("signatures");
        p1->setUselang("en");

        QTest::newRow("Text")
                << "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e1;

        Parse * p2 = new Parse( *m_mediaWiki, NULL);
        p2->setPage("http://community.kde.org/IUP_ISI/MediaWiki-Silk/");
        p2->setPropText("<p><a href=\"/wiki/Foo\" title=\"Foo\" class=\"mw-redirect\">foo</a> <a href=\"/w/index.php?title=API:Query&amp;action=edit&amp;redlink=1\" ");
        p2->setPropLanglinks("http://www.mediawiki.org/wiki/API:Parsing_wikitext");
        p2->setPropcategories("language categories");
        p2->setPropLinks("<pl ns=\"0\" exists=\"\" xml:space=\"preserve\">Case sensitivity</pl><pl ns=\"4\" exists=\"\" xml:space=\"preserve\">Wikipedia:Article wizard</pl>");
        p2->setPropTemplates("templates");
        p2->setPropImages("<img>Wiktionary-logo-en.png</img><img>Wikibooks-logo.svg</img>");
        p2->setPropExternallinks("http://www.mediawiki.org/wiki/API:Parsing_wikitext");
        p2->setPropSections("sections");
        p2->setPropRevid("34156367");
        p2->setPst("signatures");
        p2->setUselang("en");

        Parse * p3 = new Parse( *m_mediaWiki, NULL);
        p3->setText("<p><a href=\"/wiki/Foo\" title=\"Foo\" class=\"mw-redirect\">foo</a> <a href=\"/w/index.php?title=API:Query&amp;action=edit&amp;redlink=1\" class=\"new\" title=\"API:Query (page does not exist)\">bar</a> <a href=\"http://www.example.com/\" class=\"external text\" rel=\"nofollow\">baz</a></p><!--NewPP limit report Preprocessor node count: 1/1000000 Post-expand include size: 0/2048000 bytes Template argument size: 0/2048000 bytes Expensive parser function count: 0/500-->");

        Parse * p4 = new Parse( *m_mediaWiki, NULL);
        p4->setTitle("titreParse");

        Parse * p5 = new Parse( *m_mediaWiki, NULL);
        p5->setPropText("<p><a href=\"/wiki/Foo\" title=\"Foo\" class=\"mw-redirect\">foo</a> <a href=\"/w/index.php?title=API:Query&amp;action=edit&amp;redlink=1\" ");

        Parse * p6 = new Parse( *m_mediaWiki, NULL);
        p6->setPropLanglinks("http://www.mediawiki.org/wiki/API:Parsing_wikitext");

        Parse * p7 = new Parse( *m_mediaWiki, NULL);
        p7->setPropcategories("language categories");

        Parse * p8 = new Parse( *m_mediaWiki, NULL);
        p8->setPropLinks("<pl ns=\"0\" exists=\"\" xml:space=\"preserve\">Case sensitivity</pl><pl ns=\"4\" exists=\"\" xml:space=\"preserve\">Wikipedia:Article wizard</pl>");

        Parse * p9 = new Parse( *m_mediaWiki, NULL);
        p9->setPropTemplates("templates");

        Parse * p10 = new Parse( *m_mediaWiki, NULL);
        p10->setPropImages("<img>Wiktionary-logo-en.png</img><img>Wikibooks-logo.svg</img>");

        Parse * p11 = new Parse( *m_mediaWiki, NULL);
        p11->setPropExternallinks("http://www.mediawiki.org/wiki/API:Parsing_wikitext");

        Parse * p12 = new Parse( *m_mediaWiki, NULL);
        p12->setPropSections("sections");

        Parse * p13 = new Parse( *m_mediaWiki, NULL);
        p13->setPropRevid("34156367");

        Parse * p14 = new Parse( *m_mediaWiki, NULL);
        p14->setPst("signatures");

        Parse * p15 = new Parse( *m_mediaWiki, NULL);
        p15->setUselang("en");
    }

    void error()
    {
        QFETCH(QString, scenario);
        QFETCH(int, error);


        parseCount = 0;
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer fakeserver;
        if(scenario != QString("error serveur"))
        {
            fakeserver.setScenario(m_infoScenario);
            fakeserver.addScenario(scenario);
            fakeserver.startAndWait();
        }

        Parse * job = new Parse(mediawiki, NULL);
        job->setText("<p><a href=\"/wiki/Foo\" title=\"Foo\" class=\"mw-redirect\">foo</a> <a href=\"/w/index.php?title=API:Query&amp;action=edit&amp;redlink=1\" class=\"new\" title=\"API:Query (page does not exist)\">bar</a> <a href=\"http://www.example.com/\" class=\"external text\" rel=\"nofollow\">baz</a></p><!--NewPP limit report Preprocessor node count: 1/1000000 Post-expand include size: 0/2048000 bytes Template argument size: 0/2048000 bytes Expensive parser function count: 0/500-->");
        job->setTitle("titreParse");
        job->setPropText("<p><a href=\"/wiki/Foo\" title=\"Foo\" class=\"mw-redirect\">foo</a> <a href=\"/w/index.php?title=API:Query&amp;action=edit&amp;redlink=1\" ");
        job->setPropLanglinks("http://www.mediawiki.org/wiki/API:Parsing_wikitext");
        job->setPropcategories("language categories");
        job->setPropLinks("<pl ns=\"0\" exists=\"\" xml:space=\"preserve\">Case sensitivity</pl><pl ns=\"4\" exists=\"\" xml:space=\"preserve\">Wikipedia:Article wizard</pl>");
        job->setPropTemplates("templates");
        job->setPropImages("<img>Wiktionary-logo-en.png</img><img>Wikibooks-logo.svg</img>");
        job->setPropExternallinks("http://www.mediawiki.org/wiki/API:Parsing_wikitext");
        job->setPropSections("sections");
        job->setPropRevid("34156367");
        job->setPst("signatures");
        job->setUselang("en");
        connect(job,  SIGNAL(result(KJob* )),this, SLOT(parseHandle(KJob*)));

        job->exec();

        if(scenario != QString("error serveur"))
        {
            QList<FakeServer::Request> requests = fakeserver.getRequest();
            QCOMPARE(requests.size(), 2);
        }
        QCOMPARE(job->error(), error);
        QCOMPARE(parseCount, 1);

        if(scenario != QString("error serveur"))
        {
            QVERIFY(fakeserver.isAllScenarioDone());
        }
    }

    void error_data()
    {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");

        QTest::newRow("Too Many Parameters")
                << "<api><error code=\"params\" info=\"\" /> </api>"
                << int(Parse::TooManyParams);
        QTest::newRow("Missing Page")
                << "<api><error code=\"missingtitle\" info=\"\" /> </api>"
                << int(Parse::MissingPage);
    }    

    QString QStringFromFile( const QString &fileName )
    {
      QFile file( fileName );
      file.open( QFile::ReadOnly );
      QTextStream in(&file);
      QString scenario;
      // When loading from files we never have the authentication phase
      // force jumping directly to authenticated state.

      while ( !in.atEnd() ) {
        scenario.append( in.readLine() );
      }
      file.close();
      return scenario;
    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
    }

private:

    int parseCount;
    QString request;
    QString m_infoScenario;
    MediaWiki* m_mediaWiki;
};


QTEST_MAIN(ParseTest);
#include "parsetest.moc"
#endif // TEST_PARSE_H

