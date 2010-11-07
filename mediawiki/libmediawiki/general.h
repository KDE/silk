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

#ifndef GENERAL_H
#define GENERAL_H
#include <QObject>
#include <QtCore/QList>
#include <QtCore/QString>

#include <KDE/KJob>

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief General job.
 *
 * Uses for fetch a generals information about the wiki.
 */
class General : public KJob
{

    Q_OBJECT

public:
    enum
    {
        falsexml = KJob::UserDefinedError+1,
        connectionAbort,
        includeAllDenied
    };
    /**
     * @brief Constructs a General job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit General(MediaWiki const & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys the general job.
     */
    virtual ~General();
    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();
    /**
     * @brief A general result.
     */
    struct Result {
        QString mainpage;
        QString base;
        QString sitename;
        QString generator;
        QString phpversion;
        QString phpsapi;
        QString dbtype;
        QString dbversion;
        QString rev;
        QString cas;
        QString rights;
        QString lang;
        QString fallback8bitencoding;
        QString writeapi;
        QString timezone;
        QString timeoffset;
        QString articlepath;
        QString scriptpath;
        QString script;
        QString variantarticlepath;
        QString server;
        QString wikiid;
        QString time;

    };
    /**
     * @brief Result Getter.
     * @return General::Result
     */
    General::Result getResult();

private slots:
    void abort();
    void doWorkSendRequest();
    void doWorkProcessReply(QNetworkReply * reply);
private:

    struct GeneralPrivate * const d;

};
}
#endif // GENERAL_H
