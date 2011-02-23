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
namespace KIPIWikiMediaPlugin
{
class WikiMediaJob : public KJob
{
public:
    WikiMediaJob(KIPI::Interface* interface, QString login, QObject* parent=0);
    void start();
    QString buildWikiText( KIPI::ImageInfo * info);
private:

    KUrl::List m_urls;
    KIPI::Interface* m_interface;
    QString m_login;
};
}
#endif // WIKIMEDIAJOB_H
