/*
 *   Copyright 2010 by Manuel Campomanes <campomanes.manuel@gmail.com>
 *   Copyright 2010 by Guillaume Hormiere <hormiere.guillaume@gmail.com>
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

#ifndef QUERYSITEINFOGENERAL_H
#define QUERYSITEINFOGENERAL_H
#include <QObject>
#include <QtCore/QList>
#include <QtCore/QString>


#include "job.h"
#include "generalinfo.h"

#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief QuerySiteInfoGeneral job.
 *
 * Uses for fetch a generals information about the wiki.
 */
class MEDIAWIKI_EXPORT QuerySiteInfoGeneral : public Job
{

    Q_OBJECT

public:
    enum
    {
        BadXml = Job::UserDefinedError+1,
        ConnectionAborted,
        IncludeAllDenied
    };
    /**
     * @brief Constructs a QuerySiteInfoGeneral job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QuerySiteInfoGeneral( MediaWiki & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys the QuerySiteInfoGeneral job.
     */
    virtual ~QuerySiteInfoGeneral();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();
    Generalinfo getResult();

private slots:
    void abort();
    void doWorkSendRequest();
    void doWorkProcessReply(QNetworkReply * reply);

private:

    struct QuerySiteInfoGeneralPrivate * const d;

public:


};
}
#endif // QUERYSITEINFOGENERAL_H
