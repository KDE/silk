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

#ifndef PARSER_H
#define PARSER_H

#include <QtCore/QString>
#include "job.h"
#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;
class ParserPrivate;

class MEDIAWIKI_EXPORT Parser : public Job {

    Q_OBJECT
    Q_DECLARE_PRIVATE(Parser)

public:

    explicit Parser(MediaWiki & mediawiki, QObject * parent = 0);

    virtual ~Parser();

    QString text() const;

    void setText(const QString & text);

    virtual void start();

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

signals:

    void result(const QString & text);


};

}

#endif // PARSER_H
