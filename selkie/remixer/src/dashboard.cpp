/******************************************************************************
*   Copyright 2009-2012 by Sebastian Kügler <sebas@kde.org>                   *
*                                                                             *
*   This library is free software; you can redistribute it and/or             *
*   modify it under the terms of the GNU Library General Public               *
*   License as published by the Free Software Foundation; either              *
*   version 2 of the License, or (at your option) any later version.          *
*                                                                             *
*   This library is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          *
*   Library General Public License for more details.                          *
*                                                                             *
*   You should have received a copy of the GNU Library General Public License *
*   along with this library; see the file COPYING.LIB.  If not, write to      *
*   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
*   Boston, MA 02110-1301, USA.                                               *
*******************************************************************************/

//#include <QtWebKit/QWebView>

#include <QWebPage>
#include <KDebug>

#include "dashboard.h"
#include "package.h"

DashBoard::DashBoard( QWidget *parent )
    : QWebView( parent )
{
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(this, SIGNAL(linkClicked(const QUrl&)), this, SLOT(handleLinkClicked(const QUrl&)));
    update();
}

void DashBoard::update()
{
    QString html  = "<h1>Selkie Remixer</h1>\n";
    QStringList selkies;
    //selkies << "silk" << "gmail" << "reviewboard";

    html.append("Selkies: \n<ul>\n");
    foreach (const QString &s, Package::listPackages()) {
        QString _path = Package::findPackage(s);
        html.append(QString("\t<li><a href=\"%1\">%2</a></li>\n").arg(_path, s));
    }
    html.append("</ul>\n");
    setHtml(html);
}

void DashBoard::handleLinkClicked(const QUrl &url)
{
    kDebug() << "url clicked" << url.path();
    QString p = url.path();
    emit openWebApp(p);
}