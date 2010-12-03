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

#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QStringList>
#include <QDebug>

#include "edit.h"
#include "mediawiki.h"

namespace mediawiki
{
    struct EditPrivate
    {
        
        //Update page
        EditPrivate(QNetworkAccessManager * const manager, QString const & title, QString const & token, QString const & text, MediaWiki const & mediawiki)
            : manager(manager)
            , mediawiki(mediawiki)
        {
            result.title = title;
            result.token = token;
            result.text = text;
        }

        QNetworkAccessManager *manager;
        QUrl baseUrl;
        MediaWiki const & mediawiki;
        Edit::Result result;
    };

}

using namespace mediawiki;

Edit::Edit( MediaWiki const & media, QString const & title, QString const & token, QString const & params, QObject *parent )
    : KJob(parent)
    , d(new EditPrivate(new QNetworkAccessManager(this), title, token, media))
{
    setCapabilities(KJob::NoCapabilities);
}

Edit::~Edit()
{
    delete d;
}

void Edit::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void Edit::doWorkSendRequest()
{
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "edit");
    // Add params
    d->baseUrl = url;
    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager->post( request, url.toString().toUtf8() );
    connect( d->manager, SIGNAL( finished( QNetworkReply * ) ), this, SLOT( finishedEdit( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );

}

void Edit::abort()
{
    this->setError(this->ConnectionAbort);
    emitResult();
}

void Edit::finishedEdit( QNetworkReply *reply )
{

}


void Edit::finishedCaptcha( QNetworkReply *reply )
{
}
