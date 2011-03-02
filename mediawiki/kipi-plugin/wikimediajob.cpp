/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2011-02-11
 * Description : a kipi plugin to export images to wikimedia commons
 *
 * Copyright (C) 2011 by Alexandre Mendes <alex dot mendes1988 at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "wikimediajob.h"
#include <libkipi/interface.h>
#include <libkipi/imageinfo.h>
#include <libmediawiki/upload.h>
#include <libmediawiki/mediawiki.h>
#include "imageslist.h"
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <KMessageBox>
#include <KLocale>

using namespace KIPIWikiMediaPlugin;

WikiMediaJob::WikiMediaJob(KIPI::Interface *interface, QString login, mediawiki::MediaWiki* mediawiki,KIPIPlugins::ImagesList* imageList, QObject *parent)
    : KJob(parent), m_interface(interface), m_mediawiki(mediawiki), m_login(login), m_imageList(imageList), m_currentFile(QString()), m_error(QString())
{
}
void WikiMediaJob::start()
{
        m_urls = m_imageList->imageUrls();
        QTimer::singleShot(0,this,SLOT(uploadHandle(KJob*)));
}
void WikiMediaJob::begin()
{
    start();
}
void WikiMediaJob::uploadHandle(KJob* j)
{
    if(j != 0)
    {
        qDebug() << "Upload" << (int)j->error();
        disconnect(j,SIGNAL(result(KJob* )),this, SLOT(uploadHandle(KJob*)));
        if((int)j->error() != 0)
        {
            m_error.append(i18n("Error on file : "));
            m_error.append(m_currentFile);
            m_error.append(" : "+ (int)j->error());
        }
    }

    KIPI::ImageInfo info = m_interface->info(m_urls.first());
    mediawiki::Upload * e1 = new mediawiki::Upload( *m_mediawiki, this);

    qDebug() << "image path : " << info.path().url().remove("file://");
    QFile* file = new QFile(info.path().url().remove("file://"),this);
    file->open(QIODevice::ReadOnly);
    //emit fileUploadProgress(done = 0, total file.size());
    e1->setFile(file);
    m_currentFile=file->fileName();
    qDebug() << "image name : " << file->fileName().split("/").last();
    e1->setFilename(file->fileName());
    e1->setText(buildWikiText(&info));
    connect(e1, SIGNAL(result(KJob* )),this, SLOT(uploadHandle(KJob*)));

    if(m_urls.size() > 0)
    {
        e1->start();
        m_urls.removeFirst();
    }else {
        KMessageBox::error(0,m_error);
    }
}
QString WikiMediaJob::buildWikiText(KIPI::ImageInfo *info)
{
    QString text;
    text.append(" == {{int:filedesc}} ==");
    text.append( "\n{{Information");
    text.append( "\n|Description=").append( info->description());
    text.append( "\n|Source=").append( "{{own}}");
    text.append( "\n|Author=").append( "[[").append(m_login).append( "]]");
    text.append( "\n|Date=").append( info->time().toString(Qt::ISODate));
    text.append( "\n|Permission=");
    text.append( "\n|other_versions=");

    text.append( "\n}}");
    QMap<QString,QVariant> attributes = info->attributes();
    double altitude = 0, longitude = 0, latitude = 0;
    if(attributes.contains("latitude") ||
       attributes.contains("longitude") ||
       attributes.contains("altitude"))
    {
        if(attributes.contains("latitude"))
            latitude = attributes["latitude"].toDouble();
        if(attributes.contains("longitude"))
            longitude = attributes["longitude"].toDouble();
        if(attributes.contains("altitude"))
            altitude = attributes["altitude"].toDouble();
    }
    if(longitude && latitude)
    {
        text.append( "\n{{Location dec");
        text.append( "\n|").append( QString::number(longitude));
        text.append( "\n|").append( QString::number(latitude));
        text.append( "\n}}");
    }
    text.append( " == {{int:license}} ==");
    if(attributes.contains("licence"))
        text.append( attributes["licence"].toString());

    QList<QString> buff2 =  attributes.keys();
    foreach(QString s , buff2)
        qDebug() << s+" : " << attributes[s].typeName()+QString("\n");

    QStringList buff = attributes["tags"].toStringList();
    for(int i = 0; i < buff.size(); i++)
        qDebug() << buff[i];

    buff = attributes["tagspath"].toStringList();
    for(int i = 0; i < buff.size(); i++)
        qDebug() << buff[i];
    qDebug() << text;
    return text;
}

