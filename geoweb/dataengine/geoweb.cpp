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

#include "geoweb.h"

Geoweb::Geoweb(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args)
{
    Q_UNUSED(args)

    setMinimumPollingInterval(30 * 60000);
}

void Geoweb::init()
{
    location = new GeoLoc(this);
    providers = new Provider(this, location);

    connect(this, SIGNAL(updatedLocation()), providers, SLOT(updateAllPlugins()));
    connect(this, SIGNAL(requestProvider(QString)), providers, SLOT(requestPluginSlot(QString)));
    connect(this, SIGNAL(updateProvider(QString)), providers, SLOT(updatePluginSlot(QString)));
    connect(location, SIGNAL(locationUpdated()), this, SLOT(gotNewLocation()));
    connect(providers, SIGNAL(dataUpdated(const QString &, const Plasma::DataEngine::Data &)), this,
            SLOT(setProviderData(const QString &, const Plasma::DataEngine::Data &)));
}

bool Geoweb::sourceRequestEvent(const QString &source)
{
    if (source == "location")
        return updateSourceEvent(source);

    if (!providers->getSources().contains(source))
        return false;

    setData(source, "status", providers->states["na"]);

    emit requestProvider(source);

    return true;
}

bool Geoweb::updateSourceEvent(const QString &source)
{
    if (source == "location")
    {
        location->getLocation(locationData);
        setData(source, locationData);
        return true;
    }

    if (!providers->getSources().contains(source))
        return false;

    emit updateProvider(source);

    return true;
}

QStringList Geoweb::sources() const
{
    return QStringList() << "location" << providers->getSources();
}

void Geoweb::gotNewLocation()
{
    location->getLocation(locationData);
    setData("location", locationData);
    emit updatedLocation();
}

void Geoweb::setProviderData(const QString &source, const Plasma::DataEngine::Data &data)
{
    setData(source, data);
}

K_EXPORT_PLASMA_DATAENGINE(geoweb, Geoweb)

#include "geoweb.moc"
