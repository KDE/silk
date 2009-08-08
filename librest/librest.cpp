/*
    Copyright (c) 2009 Omat Holding B.V. <info@omat.nl>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "librest.h"

#include <kdebug.h>
#include <kio/job.h>
#include <kurl.h>

class LibRestPrivate
{
    public:
        LibRestPrivate( LibRest* lib) : q( lib ) {};
        KUrl baseUri;
        LibRest::Modes mode;
        LibRest::ReturnTypes returnType;
        LibRest* const q;
        QString username;
        QString password;
        QByteArray data;

    public slots:
        void slotFinished( KJob* job );
};

void LibRestPrivate::slotFinished( KJob* job )
{
    if ( job->error() ) {
        kDebug() << "Job error, " << job->errorString();
        emit q->error( job->errorString() );
        return;
    }

    KIO::StoredTransferJob* transJob = static_cast<KIO::StoredTransferJob*>( job );
    data = transJob->data();
    kDebug() << "Job finished";
    emit q->finished();
}

LibRest::LibRest() : QObject(), d( new LibRestPrivate( this ) )
{
    d->mode = LibRest::Get;
    d->returnType = LibRest::Xml;
}

LibRest::~LibRest()
{
    delete d;
}

void LibRest::setBaseUri( const QString& uri )
{
    d->baseUri = uri;
}

void LibRest::setMode( Modes mode )
{
    d->mode = mode;
}

LibRest::Modes LibRest::mode() const
{
    return d->mode;
}

void LibRest::setReturnType( ReturnTypes returnType,
                             const QString& name, const QString& value )
{
    d->returnType = returnType;
    if ( !name.isEmpty() && !value.isEmpty())
        d->baseUri.addQueryItem( name, value );
}

LibRest::ReturnTypes LibRest::returnType() const
{
    return d->returnType;
}

void LibRest::setCredentials( const QString &username, const QString &password )
{
    d->username = username;
    d->password = password;
}

void LibRest::clearCredentials()
{
    d->username.clear();
    d->password.clear();
}

void LibRest::start( const QString& request )
{
    kDebug();
    d->data.clear();
    KUrl finalRequest( d->baseUri );
    finalRequest.addPath( request );
    if ( !d->username.isEmpty() && !d->password.isEmpty() ) {
        finalRequest.setUser( d->username );
        finalRequest.setPass( d->password );
    }

    KIO::StoredTransferJob *job = KIO::storedGet( finalRequest, KIO::Reload,
                                                  KIO::HideProgressInfo ) ;
    connect( job, SIGNAL( result( KJob* ) ), this, SLOT( slotFinished( KJob* ) ) );
}

QByteArray LibRest::data()
{
    return d->data;
}

#include "librest.moc"
