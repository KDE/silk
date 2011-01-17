/*
 *   Copyright 2009 by Paolo de Vathaire <paolo.devathaire@gmail.com>
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

#ifndef MEDIAWIKIJOB_H
#define MEDIAWIKIJOB_H

#include <KDE/KJob>
#include "mediawiki_export.h"

namespace mediawiki
{

class MediaWiki;
class JobPrivate;

/**
 * @brief The base class for all MediaWiki jobs.
 */
class MEDIAWIKI_EXPORT Job : public KJob {

    Q_OBJECT
    Q_DECLARE_PRIVATE(Job)

public:

    /**
     * @brief Indicates all possible error conditions found during the processing of the job.
     */
    enum {

        NetworkError = KJob::UserDefinedError + 1,

        XmlError,

        UserRequestDefinedError = KJob::UserDefinedError + 100,

    };

    /**
     * @brief Constructs a Job.
     * @param mediawiki the mediawiki receiving the request
     * @param parent the QObject parent
     */
    Job(const MediaWiki & mediawiki, QObject * parent = 0);

    /**
     * @brief Destructs the Job.
     */
    virtual ~Job();

private:

    JobPrivate * const d_ptr;

};

}

#endif // MEDIAWIKIJOB_H
