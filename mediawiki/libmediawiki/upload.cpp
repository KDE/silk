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
#include <Q3Url>

#include "upload.h"
#include "mediawiki.h"
#include "queryinfo.h"

namespace mediawiki
{
    struct UploadPrivate
    {
        UploadPrivate(MediaWiki  & mediawiki)
            : mediawiki(mediawiki){}

        MediaWiki  & mediawiki;
        QByteArray file;
        QString filename;
        QString comment;
        QString token;
    };
}

using namespace mediawiki;

Upload::Upload( MediaWiki  & media, QObject *parent)
    : Job(media,parent)
    , d(new UploadPrivate(media))
{
}

Upload::~Upload()
{
    delete d;
}

void Upload::setFilename( const QString& param )
{
    d->filename = param;
}

void Upload::setFile( const QByteArray & file )
{
    d->file = file;
}

void Upload::setComment( const QString& param )
{
    d->comment = param;
}

void Upload::start()
{
    QueryInfo *info = new QueryInfo(d->mediawiki,this);
    info->setPageName("File:" + d->filename);
    info->setToken("edit");
    connect(info,SIGNAL(page(Page )),this,SLOT(doWorkSendRequest(Page )));
    info->start();
}

void Upload::doWorkSendRequest(Page page)
{
    QString token = page.pageEditToken();
    // Add the token
    token.replace("+", "%2B");
    token.replace("\\", "%5C");
    d->token = token;
    qDebug()<<"upload token "<<token;

    // Get the extension
    QString extension = d->filename.mid(d->filename.length() - 2).toUpper().toLower();
    if (extension == "jpg")
        extension = "jpeg";
    else if (extension == "svg")
        extension += "+xml";

    // Set the url
    QUrl url = d->mediawiki.url();
    // Set the file content
    QByteArray content = d->file;
    content = "test";
    // Add params
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "upload");
    url.addQueryItem("filename", d->filename);
    url.addQueryItem("file", content);
    if(d->comment != "")
        url.addQueryItem("comment", d->comment);
    url.addEncodedQueryItem(QByteArray("token"), d->token.toUtf8());

    // Add the cookies
    QByteArray cookie = "";
    for(int i = 0 ; i<d->mediawiki.cookies().size();i++){
        cookie += d->mediawiki.cookies().at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ";";
    }

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());



    QByteArray boundary = "87142694621188";
    request.setRawHeader("Content-Type", "multipart/form-data; boundary=" + boundary);
    request.setRawHeader( "Cookie", cookie );

    QByteArray data = "--" + boundary + "\r\n";
    // filename
    data += "Content-Disposition: form-data; name=\"filename\"\r\n\r\n";
    data += d->filename;
    data += "\r\n";
    data += boundary;
    data += "\r\n";

    // edit token
    data += "Content-Disposition: form-data; name=\"token\"\r\n\r\n";
    data += d->token;
    data += "\r\n";
    data += boundary;
    data += "\r\n";

    // the actual file
    data += "Content-Disposition: form-data; name=\"file\"; filename=\"";
    data += d->filename;
    data += "\"\r\n";
    data += "Content-Type: text/plain";
    data += "\r\n\r\n";
    data += content;
    data += "\r\n";
    data += boundary;
    data += "\r\n";



    // Set the boundary
//    QByteArray boundary = "87142694621188";
//    QByteArray data = "--" + boundary + "\r\n";
//    data += "Content-Disposition: form-data; name=\"upload\"; filename=\""+d->filename+"\";\r\n";
//    data += "Content-Type: image/"+ extension +"\r\n\r\n" + content + "\r\n";
//    data += "--" + boundary + "--\r\n";
//    request.setRawHeader("Content-Type", "multipart/form-data; boundary=" + boundary);
//    request.setRawHeader("Content-Length", QString::number(data.size()).toAscii());
    // Send the request
    qDebug()<<"data "<<data;
    d->mediawiki.manager()->post( request, data );
    connect( d->mediawiki.manager(), SIGNAL( finished( QNetworkReply * ) ), this, SLOT( doWorkProcessReply( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );
}

void Upload::abort()
{
    this->setError(this->NetworkError);
    emitResult();
}

void Upload::doWorkProcessReply( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        qDebug()<<" NetworkError upload : ";
        this->setError(this->NetworkError);
        reply->close();
        reply->deleteLater();
        emitResult();
        return;
    }
    QXmlStreamReader reader( reply );
    while ( !reader.atEnd() && !reader.hasError() ) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if ( token == QXmlStreamReader::StartElement ) {
            QXmlStreamAttributes attrs = reader.attributes();
            qDebug()<<"name : "<< reader.name() << " attrs : "<<attrs.value( QString( "result" ) ).toString();
            if ( reader.name() == QString( "upload" ) ) {
                if ( attrs.value( QString( "result" ) ).toString() == "Success" ) {
                    this->setError(KJob::NoError);
                    qDebug()<<" Success upload";
                    reply->close();
                    reply->deleteLater();
                    emitResult();
                    return;
                }
                else if ( attrs.value( QString( "result" ) ).toString() == "Failure" ) {
                    reader.readNext();
                    attrs = reader.attributes();
                    qDebug()<<" Failure upload : "<<attrs.value( QString( "code" ) ).toString();
                    this->setError(this->getError(attrs.value( QString( "code" ) ).toString()));
                    reply->close();
                    reply->deleteLater();
                    emitResult();
                    return;
                }
            }
            else if ( reader.name() == QString( "error" ) ) {
                this->setError(this->getError(attrs.value( QString( "code" ) ).toString()));
                qDebug()<<" Error upload : "<<attrs.value( QString( "code" ) ).toString();
                reply->close();
                reply->deleteLater();
                emitResult();
                return;
            }
        }
        else if ( token == QXmlStreamReader::Invalid && reader.error() != QXmlStreamReader::PrematureEndOfDocumentError){
            qDebug()<<" XMLError upload : ";
            this->setError(this->XmlError);
            reply->close();
            reply->deleteLater();
            emitResult();
            return;
        }
    }
    reply->close();
    reply->deleteLater();
    emitResult();
}

int Upload::getError(const QString & error)
{
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
