/*
 * Copyright 2010 Sebastian Kügler <sebas@kde.org>
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