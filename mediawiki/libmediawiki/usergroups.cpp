/*
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
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

#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "mediawiki.h"

#include "usergroups.h"

struct UserGroupsPrivate
{
    QNetworkAccessManager * manager;
    QNetworkReply * reply;
    QList<UserGroups::Result> usergroups;
};

UserGroups::UserGroups(MediaWiki const & mediawiki, QObject * parent)
    : QObject(parent)
    , d(new UserGroupsPrivate)
{
    // Set the url
    QUrl url = mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("meta", "siteinfo");
    url.addQueryItem("siprop", "usergroups");
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "mediawiki-silk");
    // Send the request
    d->manager = new QNetworkAccessManager(this);
    d->reply = d->manager->get(request);
    connect(d->reply, SIGNAL(finished()), this, SLOT(processReply()));
}

UserGroups::~UserGroups()
{
    delete d;
}

void UserGroups::processReply()
{
    if (d->reply->error() == QNetworkReply::NoError)
    {
        QXmlStreamReader reader(d->reply);
        UserGroups::Result usergroup;
        while(!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();
            if(token == QXmlStreamReader::StartElement)
            {
                if(reader.name() == "group")
                {
                    usergroup.name = reader.attributes().value("name").toString();
                }
                else if (reader.name() == "rights")
                {
                    usergroup.rights.clear();
                }
                else if (reader.name() == "permission")
                {
                    reader.readNext();
                    usergroup.rights.push_back(reader.text().toString());
                }
            }
            else if (token == QXmlStreamReader::EndElement)
            {
                if (reader.name() == "group")
                {
                    d->usergroups.push_back(usergroup);
                }
            }
        }
        emit finished(d->usergroups);
    }
}

