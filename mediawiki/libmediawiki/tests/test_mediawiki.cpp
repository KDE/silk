#ifndef TEST_MEDIAWIKI_H
#define TEST_MEDIAWIKI_H

#include <QObject>
#include <QtTest/QtTest>
#include "../mediawiki.h"

class test_mediawiki: public QObject
{
    Q_OBJECT
private slots:

    void initTestCase()
{
    this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
}
    void mediawikiUrlTest()
{
    Q_ASSERT(this->m_mediaWiki->url() == QUrl("http://127.0.0.1:12566"));
}
    void cleanupTestCase()
{
    delete this->m_mediaWiki;
}
private:
    MediaWiki* m_mediaWiki;
};


QTEST_MAIN(test_mediawiki)
#include "test_mediawiki.moc"

#endif // TEST_MEDIAWIKI_H

