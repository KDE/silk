#ifndef TEST_USERGROUPS_H
#define TEST_USERGROUPS_H

#include <QObject>
#include <QtTest/QtTest>
#include "mediawiki/mediawiki.h"
#include "mediawiki/usergroups.h"
#include "mediawiki/tests/fake_server/server.h"

Q_DECLARE_METATYPE(QList<UserGroups::Result>)

class test_usergroups : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
        this->m_server = new Server(0);
    }

    void UserGroupsConnect()
    {
         qRegisterMetaType< QList<UserGroups::Result> >("QList<UserGroups::Result>" );
         QString senario("<?xml version=\"1.0\" encoding=\"utf-8\"?><api><query><usergroups><group name=\"*\"><rights><permission>createaccount</permission><permission>read</permission><permission>edit</permission><permission>createtalk</permission><permission>writeapi</permission><permission>centralauth-merge</permission><permission>abusefilter-view</permission><permission>abusefilter-log</permission><permission>abusefilter-log-detail</permission></rights></group><group name=\"user\"><rights><permission>reupload-own</permission><permission>move-rootuserpages</permission><permission>read</permission><permission>edit</permission><permission>createpage</permission><permission>createtalk</permission><permission>writeapi</permission><permission>minoredit</permission><permission>purge</permission><permission>sendemail</permission></rights></group><group name=\"autoconfirmed\"><rights><permission>patrol</permission><permission>reupload</permission><permission>upload</permission><permission>move</permission><permission>collectionsaveasuserpage</permission><permission>collectionsaveascommunitypage</permission><permission>autoconfirmed</permission><permission>movestable</permission><permission>autoreview</permission><permission>skipcaptcha</permission></rights></group><group name=\"bot\"><rights><permission>noratelimit</permission><permission>bot</permission><permission>autoconfirmed</permission><permission>nominornewtalk</permission><permission>autopatrol</permission><permission>suppressredirect</permission><permission>apihighlimits</permission><permission>writeapi</permission><permission>autoreview</permission><permission>skipcaptcha</permission></rights></group><group name=\"sysop\"><rights><permission>suppressredirect</permission><permission>noratelimit</permission><permission>deleterevision</permission><permission>block</permission><permission>createaccount</permission><permission>delete</permission><permission>deletedhistory</permission><permission>deletedtext</permission><permission>undelete</permission><permission>editinterface</permission><permission>editusercss</permission><permission>edituserjs</permission><permission>import</permission><permission>move</permission><permission>move-subpages</permission><permission>move-rootuserpages</permission><permission>patrol</permission><permission>autopatrol</permission><permission>protect</permission><permission>proxyunbannable</permission><permission>rollback</permission><permission>trackback</permission><permission>upload</permission><permission>reupload</permission><permission>reupload-shared</permission><permission>unwatchedpages</permission><permission>autoconfirmed</permission><permission>upload_by_url</permission><permission>ipblock-exempt</permission><permission>blockemail</permission><permission>markbotedits</permission><permission>apihighlimits</permission><permission>browsearchive</permission><permission>versiondetail</permission><permission>movefile</permission><permission>autoreview</permission><permission>stablesettings</permission><permission>movestable</permission><permission>review</permission><permission>tboverride</permission><permission>globalblock-whitelist</permission><permission>nuke</permission><permission>skipcaptcha</permission><permission>override-antispoof</permission><permission>centralnotice-admin</permission><permission>centralnotice-translate</permission><permission>abusefilter-log-detail</permission><permission>abusefilter-revert</permission><permission>abusefilter-view-private</permission></rights></group><group name=\"bureaucrat\"><rights><permission>noratelimit</permission><permission>renameuser</permission><permission>override-antispoof</permission></rights></group><group name=\"checkuser\"><rights><permission>checkuser</permission><permission>checkuser-log</permission></rights></group><group name=\"reviewer\"><rights><permission>patrol</permission><permission>validate</permission><permission>review</permission><permission>autoreview</permission><permission>autoconfirmed</permission><permission>patrolmarks</permission></rights></group><group name=\"steward\"><rights><permission>noratelimit</permission><permission>globalblock</permission><permission>globalunblock</permission><permission>userrights</permission><permission>centralauth-unmerge</permission><permission>centralauth-lock</permission><permission>centralauth-oversight</permission></rights></group><group name=\"accountcreator\"><rights><permission>override-antispoof</permission><permission>tboverride</permission><permission>noratelimit</permission></rights></group><group name=\"import\"><rights><permission>importupload</permission><permission>import</permission></rights></group><group name=\"transwiki\"><rights><permission>import</permission></rights></group><group name=\"ipblock-exempt\"><rights><permission>ipblock-exempt</permission><permission>torunblocked</permission></rights></group><group name=\"oversight\"><rights><permission>deleterevision</permission><permission>hideuser</permission><permission>suppressrevision</permission><permission>suppressionlog</permission><permission>abusefilter-hide-log</permission><permission>abusefilter-hidden-log</permission><permission>hiderevision</permission><permission>oversight</permission></rights></group><group name=\"founder\"><rights><permission>userrights</permission></rights></group><group name=\"rollbacker\"><rights><permission>rollback</permission></rights></group><group name=\"confirmed\"><rights><permission>patrol</permission><permission>reupload</permission><permission>upload</permission><permission>move</permission><permission>collectionsaveasuserpage</permission><permission>collectionsaveascommunitypage</permission><permission>autoconfirmed</permission><permission>movestable</permission><permission>autoreview</permission><permission>skipcaptcha</permission></rights></group><group name=\"autoreviewer\"><rights><permission>autopatrol</permission></rights></group><group name=\"researcher\"><rights><permission>browsearchive</permission><permission>deletedhistory</permission><permission>apihighlimits</permission></rights></group><group name=\"abusefilter\"><rights><permission>abusefilter-modify</permission></rights></group></usergroups></query></api>");
         m_server->addScenario(senario);
         m_server->startAndWait();
         this->m_usergroups = new UserGroups(*m_mediaWiki);
         QSignalSpy spy(m_usergroups, SIGNAL(finished(QList<UserGroups::Result>)));
         QEventLoop loop;
         connect(m_usergroups, SIGNAL(finished(QList<UserGroups::Result> const &)),&loop,SLOT(quit()));
         loop.exec();
         QCOMPARE(spy.count(), 1);
         QVariantList arguments = spy.takeFirst();
         QList<UserGroups::Result> results = arguments.at(0).value< QList<UserGroups::Result> >();
         QCOMPARE( results.size(), 20);
    }


    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
        delete this->m_server;
    }

private:
    UserGroups* m_usergroups;
    MediaWiki* m_mediaWiki;
    Server* m_server;
};

#endif // TEST_USERGROUPS_H

QTEST_MAIN(test_usergroups)
#include "test_usergroups.moc"


