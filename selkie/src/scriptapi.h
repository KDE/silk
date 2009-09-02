/*
 * Copyright 2009 Richard Moore <rich@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SCRIPTAPI_H
#define SCRIPTAPI_H

#include <QtCore/QObject>
#include <QVariant>

#include <kconfiggroup.h>

class QWebFrame;
class View;

class ScriptApi : public QObject
{
    Q_OBJECT

public:
    ScriptApi( QObject *parent=0 );

    void setWebView( View *view );

    void setTrusted( bool yes=false );

    // Set the greasemonkey identifiers used for things like GM_setValue
    // these can't be slots as they should not be modifiable by scripts.

    void setGreaseMonkeyName( const QString &name );
    void setGreaseMonkeyNameSpace( const QString &gmnamespace );

public slots:
    //
    // Methods used to implement the greasemonkey api
    //

    /** GM_log - log messages to the JavaScript Console */
    void GM_log( const QString &message );

    /** GM_getValue - get script-specific configuration value */
    QVariant GM_getValue( const QString &key, const QVariant &defaultVal = QVariant() );

    /** GM_setValue - set script-specific configuration value */
    void GM_setValue( const QString &key, const QVariant &value );

    /** GM_registerMenuCommand - add a menu item to the User Script Commands submenu */
    // Not sure how to implement this

    /** GM_xmlhttpRequest - make an arbitrary HTTP request */
    // Not sure if we want to implement this

    //
    // Methods for a fluid-like api
    //

    void showNotification( const QString &text );

    // Only works if trusted
    bool include( const QString &filename );

protected:
    /** Installs forwarding functions for GM_ global methods. */
    void installGlobals();

    KConfigGroup config() const;

private slots:
    void attachObject();

private:
    View *m_view;
    bool m_trusted;
    QString m_gmName;
    QString m_gmNameSpace;
};

#endif // SCRIPTAPI_H

