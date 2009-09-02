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

#include <QtNetwork/QNetworkRequest>
#include <QtGui/QDesktopServices>
#include <QtWebKit/QWebFrame>

#include <KDebug>

#include "view.h"
#include "page.h"

Page::Page( View *view )
    : QWebPage( view )
{
    m_view = view;
}

bool Page::acceptNavigationRequest( QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type )
{
    if ( type == QWebPage::NavigationTypeLinkClicked ) {
        bool inScope = false;
        QUrl base;
        foreach( base, m_view->options()->allowedBases ) {
            if ( base.isParentOf( request.url() ) ) {
                inScope = true;
            }
        }

        if ( !inScope ) {
            QDesktopServices::openUrl( request.url() );
            return false;
        } else {
        }
    }

    return QWebPage::acceptNavigationRequest( frame, request, type );
}

