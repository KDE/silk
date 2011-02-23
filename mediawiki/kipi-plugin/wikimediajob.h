#ifndef WIKIMEDIAJOB_H
#define WIKIMEDIAJOB_H
#include <KJob>
#include <QString>
#include <KUrl>
namespace KIPI
{
    class ImageInfo;
    class Interface;
}
namespace mediawiki
{
    class MediaWiki;
}
namespace KIPIWikiMediaPlugin
{
class WikiMediaJob : public KJob
{
    Q_OBJECT
public:
    WikiMediaJob(KIPI::Interface* interface, QString login ,mediawiki::MediaWiki* mediawiki, QObject* parent=0);
    QString buildWikiText( KIPI::ImageInfo * info);
    void start();
public slots:
    void begin();
    void uploadHandle(KJob* j);
private:

    KUrl::List m_urls;
    KIPI::Interface* m_interface;
    mediawiki::MediaWiki* m_mediawiki;
    QString m_login;
};
}
#endif // WIKIMEDIAJOB_H
