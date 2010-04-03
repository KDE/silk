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

#include <Plasma/DataEngineManager>

#include "geoloc.h"

GeoLoc::GeoLoc(QObject *parent)
    : QObject(parent)
{
    location["country code"] = "";
    location["country"] = "";
    location["city"] = "";
    location["longitude"] = "";
    location["latitude"] = "";

    geolocationEngine = Plasma::DataEngineManager::self()->loadEngine("geolocation");
    if (geolocationEngine)
        geolocationEngine->connectSource("location", this);
}

GeoLoc::~GeoLoc()
{
    if (Plasma::DataEngineManager::self()->engine("geolocation")->isValid())
        Plasma::DataEngineManager::self()->unloadEngine("geolocation");
}

void GeoLoc::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    // determine if anything changed
    bool updated = false;

    if (data.isEmpty())
        return;

    if (source == "location")
    {
        if (location["country code"] != data["country code"])
        {
            location["country code"] = data["country code"];
            updated = true;
        }

        if (location["country"] != data["country"])
        {
            location["country"] = data["country"];
            updated = true;
        }

        if (location["city"] != data["city"])
        {
            location["city"] = data["city"];
            updated = true;
        }

        if (location["longitude"] != data["longitude"])
        {
            location["longitude"] = data["longitude"];
            updated = true;
        }

        if (location["latitude"] != data["latitude"])
        {
            location["latitude"] = data["latitude"];
            updated = true;
        }

        // if location changed, emit signal
        if (updated)
            emit locationUpdated();
    }
}

bool GeoLoc::isValid()
{
    bool valid = false;

    if (location["longitude"] != "" && location["latitude"] != ""
            && location["country"] != "" && location["city"] != "")
        valid = true;

    return valid;
}

void GeoLoc::getLocation(Plasma::DataEngine::Data &data)
{
    data = location;
}

QString GeoLoc::getCountryCode()
{
    return location["country code"].toString();
}

QString GeoLoc::getCountry()
{
    return location["country"].toString();
}

QString GeoLoc::getCity()
{
    return location["city"].toString();
}

QString GeoLoc::getLongitude()
{
    return location["longitude"].toString();
}

QString GeoLoc::getLatitude()
{
    return location["latitude"].toString();
}
