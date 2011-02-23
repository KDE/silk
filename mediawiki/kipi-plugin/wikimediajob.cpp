#include "wikimediajob.h"
#include <libkipi/interface.h>
#include <libkipi/imageinfo.h>
#include <QMessageBox>

using namespace KIPIWikiMediaPlugin;

WikiMediaJob::WikiMediaJob(KIPI::Interface *interface, QString login, QObject *parent)
    : KJob(parent), m_interface(interface), m_login(login)
{
    m_urls = interface->currentSelection().images();
}
void WikiMediaJob::start()
{
    for (KUrl::List::ConstIterator it = m_urls.constBegin(); it != m_urls.constEnd(); ++it)
    {
        KIPI::ImageInfo info = m_interface->info(*it);
        QMessageBox popup;
        popup.setText(buildWikiText(&info));
        popup.exec();
    }
}
QString WikiMediaJob::buildWikiText(KIPI::ImageInfo *info)
{
    QString text;
    text.append(" == {{int:filedesc}} ==");
    text.append( "{{Information");
    text.append( "|Description=").append( info->description());
    text.append( "|Source=").append( "{{own}}");
    text.append( "|Author=").append( "[[").append( m_login.append( "]]"));
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
        text.append( "}}");
    }
    text.append( " == {{int:license}} ==");
    if(attributes.contains("licence"))
        text.append( attributes["licence"].toString());
    return text;
}
