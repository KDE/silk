#include "wikimediajob.h"
#include <libkipi/interface.h>
#include <libkipi/imageinfo.h>
#include <libmediawiki/upload.h>
#include <libmediawiki/mediawiki.h>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

using namespace KIPIWikiMediaPlugin;

WikiMediaJob::WikiMediaJob(KIPI::Interface *interface, QString login, mediawiki::MediaWiki* mediawiki, QObject *parent)
    : KJob(parent), m_interface(interface), m_mediawiki(mediawiki), m_login(login)
{
}
void WikiMediaJob::start()
{
    m_urls = m_interface->currentSelection().images();

    for (KUrl::List::ConstIterator it = m_urls.constBegin(); it != m_urls.constEnd(); ++it)
    {

        KIPI::ImageInfo info = m_interface->info(*it);
        mediawiki::Upload * e1 = new mediawiki::Upload( *m_mediawiki, this);
        qDebug() << "image path : " << info.path().url().remove("file://");
        QFile file(info.path().url().remove("file://"));
        file.open(QIODevice::ReadOnly);
        e1->setFile(&file);
        qDebug() << "image name : " << file.fileName().split("/").last();
        e1->setFilename(file.fileName());
        e1->setText(buildWikiText(&info));
        connect(e1, SIGNAL(result(KJob* )),this, SLOT(uploadHandle(KJob*)));
        e1->exec();
    }
}
void WikiMediaJob::begin()
{
    start();
}
void WikiMediaJob::uploadHandle(KJob* j)
{
    qDebug() << "Upload" << (int)j->error();
}
QString WikiMediaJob::buildWikiText(KIPI::ImageInfo *info)
{
    QString text;
    text.append(" == {{int:filedesc}} ==");
    text.append( "{{Information");
    text.append( "|Description=").append( info->description());
    text.append( "|Source=").append( "{{own}}");
    text.append( "|Author=").append( "[[").append(m_login).append( "]]");
    text.append( "|Date=").append( info->time().toString(Qt::ISODate));
    text.append( "|Permission=");
    text.append( "|other_versions=");

    text.append( "}}");
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
        text.append( "{{Location dec");
        text.append( "|").append( QString::number(longitude));
        text.append( "|").append( QString::number(latitude));
        text.append( "|}}");
    }
    text.append( " == {{int:license}} ==");
    if(attributes.contains("licence"))
        text.append( attributes["licence"].toString());
    qDebug() << text;
    return text;
}

//        QString text("== {{int:filedesc}} == {{Information |Description=");
//        text.append(this->ui->descriptionEdit->text());
//        text.append(" |Source=").append(this->ui->sourceEdit->text());
//        text.append(" |Date=").append(this->ui->dateEdit->text());
//        text.append(" |Author=").append(this->ui->authorEdit->text());
//        text.append(" |Permission=").append(this->ui->permissionEdit->text());
//        text.append(" |other_versions=").append(this->ui->versionsEdit->text());
//        text.append("}} == {{int:license}} ==");
