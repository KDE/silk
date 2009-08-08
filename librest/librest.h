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

#ifndef LIBREST_H
#define LIBREST_H

#include "qobject.h"

#include <kdemacros.h>


class LibRestPrivate;

class KDE_EXPORT LibRest : public QObject
{
    Q_OBJECT

public:
    enum Modes {
        Get,    // Uses HTTP Get requests
        Post   // Uses HTTP Post requests
    };

    enum ReturnTypes {
        Xml,    // Will return XML
        Jason   // Will return Jason
    };

    /** Constructor **/
    LibRest();

    /** Constructor **/
    ~LibRest();

    /**
     * With this function you can set the base uri to the REST API
     */
    void setBaseUri( const QString& );

    /**
     * Sets the operating mode. You can change it per request you make.
     * Without setting it, Get is assumed.
     * TODO: not implemented yet;
     */
    void setMode( LibRest::Modes );

    /**
     * Gets the current operating mode.
     */
    LibRest::Modes mode() const;

    /**
     * Sets the return type. Optionally you can add the name and value
     * that needs to be added to the request to put it in that mode.
     * Without setting it, XML is assumed.
     * TODO: not implemented yet;
     */
    void setReturnType( LibRest::ReturnTypes, const QString&, const QString&);

    /**
     * Gets the current returnType;
     */
    LibRest::ReturnTypes returnType() const;

    /**
     * Sets a username and password for the requests.
     */
    void setCredentials( const QString&, const QString&);

    /**
     * Clear the credentials.
     */
    void clearCredentials();

    /**
     * Execute a request
     */
    void start( const QString& request );

    /**
     * Call this after you received the finished() or error() signal to retrieve
     * the data.
     */
    QByteArray data();

Q_SIGNALS:
    /**
     * Signal emitted after a call to start(), to indicate the data is ready
     * to be fetched.
     */
    void finished();

    /**
     * Signal emitted after a call to start(), to indicate the request failed.
     */
    void error( const QString& );

private:
    Q_DECLARE_PRIVATE( LibRest )
    LibRestPrivate* const d;

    Q_PRIVATE_SLOT( d, void slotFinished( KJob* ) )
};

#endif // LIBREST_H
