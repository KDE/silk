#ifndef WIKIMEDIAJOB_H
#define WIKIMEDIAJOB_H
#include <KJob>
#include <QString>
#include <KUrl>

namespace KIPIWikiMediaPlugin
{
class WikiMediaJob : public KJob
{
public:
    WikiMediaJob(const QString& albumName, const KUrl::List& url, QObject* parent=0);
    void start(){};
private:

    KUrl::List m_urls;
    QString    m_albumName;
};
}
#endif // WIKIMEDIAJOB_H
