#include "wikimediajob.h"
using namespace KIPIWikiMediaPlugin;
WikiMediaJob::WikiMediaJob(const QString &albumName, const KUrl::List &url, QObject *parent)
    : KJob(parent), m_urls(url), m_albumName(albumName)
{
}
