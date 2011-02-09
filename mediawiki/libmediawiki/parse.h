/*
 *   Copyright 2010 by Manuel Campomanes <campomanes.manuel@gmail.com>
 *   Copyright 2010 by Vincent Garcia <xavier.vincent.garcia@gmail.com>
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

#ifndef PARSE_H
#define PARSE_H

#include <QtCore/QString>
#include "job.h"
#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;
class ParsePrivate;

class MEDIAWIKI_EXPORT Parse : public Job {

    Q_OBJECT
    Q_DECLARE_PRIVATE(Parse)

public:

    /**
     * @brief Indicates all possible error conditions found during the processing of the job.
     */
    enum {

        /**
         * @brief An internal error occurred.
         */
        InternalError= Job::UserDefinedError+1,

        /**
         * @brief The page parameter cannot be used together with the text and title parameters
         */
        TooManyParams,

        /**
         * @brief The page you specified doesn't exist
         */
        MissingPage

    };

    explicit Parse(MediaWiki & mediawiki, QObject * parent = 0);

    virtual ~Parse();

    void setText(const QString & text);

    virtual void start();

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply();

signals:

    void result(const QString & text);


};

}

#endif // PARSE_H
