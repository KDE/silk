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

#include "mediawiki.h"
#include <QList>
#include <kdemacros.h>
class QNetworkReply;

class LoginL {

public:

    QString lgname;
    QString lgpassword;
    QString lgtoken;
    QString lgsessionid;

};

class KDE_EXPORT Login : public QObject
{
    Q_OBJECT
public:
    class Result {
    public:
        Result() {
        };

        Result( const Result &r ) {
            this->title = r.title;
            this->url = r.url;
        };

        Result &operator= (const Result &other)  {
            this->title = other.title;
            this->url = other.url;

            return *this;
        };

        /** The page title of the match. */
        QString title;
        /** The URL of the page containing the match. */
        QUrl url;
    };
    explicit Login(MediaWiki const & media, const QString &login, const QString &password, QObject * parent = 0);

    virtual ~Login();

private slots:
    void finishedLogin( QNetworkReply *reply );
    void finishedToken( QNetworkReply *reply );

signals:
    void finishedLogin(bool);
    void finishedToken(bool);


private:

    struct LoginPrivate * const d;
};

#endif // LOGIN_H
