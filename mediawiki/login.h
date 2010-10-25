/*
 *   Copyright 2010 by Alexandre Mendes <alex.mendes1988@gmail.com>
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

#ifndef LOGIN_H
#define LOGIN_H

#include <qobject.h>
#include <mediawiki.h>
#include <QList>
#include <kdemacros.h>
class QNetworkReply;

class KDE_EXPORT Login : public QObject
{
    Q_OBJECT
public:
    explicit Login( MediaWiki const & media, const QString &login, const QString &password, QObject * parent = 0 );

    virtual ~Login();

signals:
    /**
     * Emitted when a connection request has been completed.
     * @param success true if the request was completed successfully.
     */
    void finishedLogin( bool );
    /**
     * Emitted when a connection has been completed.
     * @param success true if the connection was completed successfully.
     */
    void finishedToken( bool );
public slots:
    /**
     * Aborts the currently running request.
     */
    void abort();
private slots:
    /**
     * Reads the xml
     * if the attribute value is equal to "NeedToken", try to log in the user
     * else if the attribute value is equal to "Success", the user is logged in
     */
    void finishedLogin( QNetworkReply *reply );
    /**
     * Reads the xml
     * if the attribute value is equal to "Success", the user is logged in
     */
    void finishedToken( QNetworkReply *reply );

private:

    struct LoginPrivate * const d;
};

#endif // LOGIN_H
