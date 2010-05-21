/*
 *   Copyright 2010 Peter Schiffer <peter.schiffer@eyeos.cz>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
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

#include <QWebPage>
#include <QWebFrame>
#include <QTcpSocket>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>

#include <KUrl>
#include <KDebug>
#include <Plasma/DataEngineManager>

#include "plasma-geoweb-openstreetmap-map.h"


OSMMap::OSMMap(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    // this will get us the standard applet background, for free!
    setBackgroundHints(DefaultBackground);
    resize(350, 400);

    m_data.clear();
    error = false;
}

OSMMap::~OSMMap()
{
    if (!hasFailedToLaunch())
        Plasma::DataEngineManager::self()->unloadEngine("geoweb");
}

void OSMMap::init()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);

    m_view = new QWebView();
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
    proxy->setWidget(m_view);

    layout->addItem(proxy);

    connect(this, SIGNAL(updateData()), this, SLOT(updateDataSlot()));
    connectEngine();
}

void OSMMap::connectEngine()
{
    Plasma::DataEngine *geowebEngine = Plasma::DataEngineManager::self()->loadEngine("geoweb");
    if (!geowebEngine)
        setFailedToLaunch(true, I18N_NOOP("Problem loading geoweb dataengine!"));

    geowebEngine->connectSource("location", this, 10000);
}

void OSMMap::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    if (source != "location")
        return;

    bool updated = false;

    if (m_data["lat"] != data["latitude"])
    {
        m_data["lat"] = data["latitude"];
        updated = true;
    }

    if (m_data["lon"] != data["longitude"])
    {
        m_data["lon"] = data["longitude"];
        updated = true;
    }

    if (updated || error)
        emit updateData();
}

void OSMMap::updateDataSlot()
{
    QString page;
    QTcpSocket socket;
    socket.connectToHost("www.openlayers.org", 80);
    if (!socket.waitForConnected(2000))
        error = true;
    else
        error = false;

    if (!error)
    {
        if (m_data["lat"].toDouble() > 90 || m_data["lat"].toDouble() < -90 ||
            m_data["lon"].toDouble() > 180 || m_data["lon"].toDouble() < -180)
        {
            page = "<h2 style=\"text-align:center;width:100%\">Invalid place.</h2>";
        }
        else
        {
            page = QString("<html><head>"
                "<style type=\"text/css\">"
                  "html,body,#basicMap{"
                      "height:100%;"
                      "margin:0;"
                      "width:100%;}"
                "</style></head>"
                "<body onload=\"init();\">"
                  "<div id=\"basicMap\"></div>"
                "</body>"
                "<script src=\"http://www.openlayers.org/api/OpenLayers.js\"></script>"
                "<script>"
                  "function init() {"
                    "map = new OpenLayers.Map(\"basicMap\");"
                    "var mapnik = new OpenLayers.Layer.OSM();"
                    "map.addLayer(mapnik);"
                    "map.setCenter(new OpenLayers.LonLat(%1,%2)"
                      ".transform("
                        "new OpenLayers.Projection(\"EPSG:4326\"),"
                        "new OpenLayers.Projection(\"EPSG:900913\")"
                      "), 17"
                    ");"
                  "}"
                "</script></html>").arg(m_data["lon"].toString(), m_data["lat"].toString());
        }
    }
    else
        page = "<h2 style=\"text-align:center;width:100%\">Service unavailable.</h2>";

    m_view->setHtml(page);
}

#include "plasma-geoweb-openstreetmap-map.moc"
