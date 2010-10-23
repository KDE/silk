#ifndef TEST_LOGIN_H
#define TEST_LOGIN_H

#include <QObject>
#include <QtTest/QtTest>
#include "../../mediawiki.h"
#include "../../login.h"
#include "../fake_server/server.h"

class test_login : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void loginConnect();
    void cleanupTestCase();
private:
    Login* m_login;
    MediaWiki* m_mediaWiki;
    Server* m_server;
};

#endif // TEST_LOGIN_H
