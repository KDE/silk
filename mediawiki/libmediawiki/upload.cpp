/*
 *   Copyright 2011 by Alexandre Mendes <alex.mendes1988@gmail.com>
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
#include <QtCore/QFile>

#include "job_p.h"
#include "upload.h"
#include "mediawiki.h"
#include "queryinfo.h"

namespace mediawiki
{
    class UploadPrivate : public JobPrivate
    {

    public:

        UploadPrivate(MediaWiki & mediawiki)
            : JobPrivate(mediawiki)
        {}

        static int error(const QString & error) {
            QString temp = error;
            int ret = 0;
            QStringList list;
            list    << "internalerror"
                    << "uploaddisabled"
                    << "invalidsessionkey"
                    << "badaccessgroups"
                    << "missingparam"
                    << "mustbeloggedin"
                    << "fetchfileerror"
                    << "nomodule"
                    << "emptyfile"
                    << "filetypemissing"
                    << "filenametooshort"
                    << "overwrite"
                    << "stashfailed";
            ret = list.indexOf(temp.remove(QChar('-')));
            if(ret == -1){
                ret = 0;
            }
            return  ret + (int)Upload::InternalError ;
        }

        QIODevice* file;
        QString filename;
        QString comment;
        QString text;
        QString token;
    };
}

using namespace mediawiki;

Upload::Upload( MediaWiki  & mediawiki, QObject *parent)
    : Job(*new UploadPrivate(mediawiki), parent)
{}

Upload::~Upload() {}

void Upload::setFilename( const QString& param )
{
    Q_D(Upload);
    d->filename = param;
}

void Upload::setFile( QIODevice* file )
{
    Q_D(Upload);
    d->file = file;
}

void Upload::setComment( const QString& param )
{
    Q_D(Upload);
    d->comment = param;
}

void Upload::setText(const QString& text)
{
    Q_D(Upload);
    d->text = text;
}

void Upload::start()
{
    Q_D(Upload);
    QueryInfo *info = new QueryInfo(d->mediawiki,this);
    info->setPageName("File:" + d->filename);
    info->setToken("edit");
    connect(info,SIGNAL(page(Page )),this,SLOT(doWorkSendRequest(Page )));
    info->start();
}

void Upload::doWorkSendRequest(Page page)
{
    Q_D(Upload);
    QString token = page.pageEditToken();
    d->token = token;

    // Get the extension
    QStringList filename = d->filename.split(".");
    QString extension = filename.at(filename.size()-1);
    if (extension == "jpg")
        extension = "jpeg";
    else if (extension == "svg")
        extension += "+xml";

    // Set the url
    QUrl url = d->mediawiki.url();

    // Add params
    url.addQueryItem("action", "upload");
    url.addQueryItem("format", "xml");

    // Add the cookies
    QByteArray cookie = "";
    QList<QNetworkCookie> mediawikiCookies = d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url());
    for(int i = 0; i < mediawikiCookies.size(); ++i){
        cookie += mediawikiCookies.at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ";";
    }

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader( "User-Agent", d->mediawiki.userAgent().toUtf8());

    QByteArray boundary = "-----------------------------15827188141577679942014851228";
    request.setRawHeader( "Content-Type", "multipart/form-data; boundary="  + boundary);
    request.setRawHeader( "Cookie", cookie );

    // send data
    boundary = "--" + boundary + "\r\n";
    QByteArray out = boundary;
    // ignore warnings
    out += "Content-Disposition: form-data; name=\"ignorewarnings\"\r\n\r\n";
    out += "true\r\n";
    out += boundary;
    // filename
    out += "Content-Disposition: form-data; name=\"filename\"\r\n\r\n";
    out += d->filename.toUtf8();
    out += "\r\n";
    out += boundary;
    // comment
    if(d->comment != "")
    {
        out += "Content-Disposition: form-data; name=\"comment\"\r\n\r\n";
        out += d->comment.toUtf8();
        out += "\r\n";
        out += boundary;
    }
    // token
    out += "Content-Disposition: form-data; name=\"token\"\r\n\r\n";
    out += d->token.toUtf8();
    out += "\r\n";
    out += boundary;
    // the actual file
    out += "Content-Disposition: form-data; name=\"file\"; filename=\"";
    out += d->filename;
    out += "\"\r\n";
    out += "Content-Type: image/";
    out += extension;
    out += "\r\n\r\n";
    out += d->file->readAll();
    out += "\r\n";
    out += boundary;
    // description page
    out += "Content-Disposition: form-data; name=\"text\"\r\n";
    out += "Content-Type: text/plain\r\n\r\n";
    out += d->text;
    out += "\r\n";
    out += boundary.mid(0, boundary.length() - 2);
    out += "--\r\n";

    d->reply = d->manager->post( request, out );
    connectReply();
    connect( d->reply, SIGNAL( finished() ), this, SLOT( doWorkProcessReply() ) );
}

void Upload::doWorkProcessReply()
{
    Q_D(Upload);
    disconnect( d->reply, SIGNAL( finished() ), this, SLOT( doWorkProcessReply() ) );

    if ( d->reply->error() != QNetworkReply::NoError )
    {
        this->setError(this->NetworkError);
        d->reply->close();
        d->reply->deleteLater();
        emitResult();
        return;
    }

    QXmlStreamReader reader( d->reply );
    while ( !reader.atEnd() && !reader.hasError() ) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if ( token == QXmlStreamReader::StartElement ) {
            QXmlStreamAttributes attrs = reader.attributes();
            if ( reader.name() == QString( "upload" ) ) {
                if ( attrs.value( QString( "result" ) ).toString() == "Success" ) {
                    this->setError(KJob::NoError);
                }
            }
            else if ( reader.name() == QString( "error" ) ) {
                this->setErrorText(attrs.value( QString( "info" )).toString());
                this->setError(UploadPrivate::error(attrs.value("code" ).toString()));
            }
        }
        else if ( token == QXmlStreamReader::Invalid && reader.error() != QXmlStreamReader::PrematureEndOfDocumentError){
            this->setError(this->XmlError);
        }
    }
    d->reply->close();
    d->reply->deleteLater();
    emitResult();
}
