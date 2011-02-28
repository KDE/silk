/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2011-02-11
 * Description : a kipi plugin to export images to WikiMedia web service
 *
 * Copyright (C) 2011 by Alexandre Mendes <alex dot mendes1988 at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef WIKIMEDIAJOB_H
#define WIKIMEDIAJOB_H
#include <KJob>
#include <QString>
#include <KUrl>

// Local includes

#include "imageslist.h"

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
    WikiMediaJob(KIPI::Interface* interface, QString login ,mediawiki::MediaWiki* mediawiki,KIPIPlugins::ImagesList* imageList, QObject* parent=0);
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
    KIPIPlugins::ImagesList* m_imageList;
};
}
#endif // WIKIMEDIAJOB_H
