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

#include <cmath>

#include <Plasma/DataEngineManager>

#include "geoloc.h"


GeoLoc::GeoLoc(QObject *parent)
    : QObject(parent)
{
    clearLocation();
    reload();
}

GeoLoc::~GeoLoc()
{
    if (Plasma::DataEngineManager::self()->engine("geolocation")->isValid())
        Plasma::DataEngineManager::self()->unloadEngine("geolocation");
}

void GeoLoc::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    if (data.isEmpty())
        return;

    if (source == "location")
    {
        geolocationData = data;
        updateData(Settings::EnumSource::GeolocationDataengine);
    }
}

void GeoLoc::updateData(Settings::EnumSource::type src)
{
    // determine if anything changed
    bool updated = false;

    if (src == Settings::EnumSource::GeolocationDataengine)
    {
        if (location["country code"] != geolocationData["country code"])
        {
            location["country code"] = geolocationData["country code"];
            updated = true;
        }

        if (location["country"] != geolocationData["country"])
        {
            location["country"] = geolocationData["country"];
            updated = true;
        }

        if (location["city"] != geolocationData["city"])
        {
            location["city"] = geolocationData["city"];
            updated = true;
        }

        if (location["longitude"] != geolocationData["longitude"])
        {
            location["longitude"] = geolocationData["longitude"];
            updated = true;
        }

        if (location["latitude"] != geolocationData["latitude"])
        {
            location["latitude"] = geolocationData["latitude"];
            updated = true;
        }

        if (!location["address"].toString().isEmpty())
        {
            location["address"].clear();
            updated = true;
        }
    }
    else if (src == Settings::EnumSource::Manual)
    {
        if (location["country code"] != Settings::countryCode())
        {
            location["country code"] = Settings::countryCode();
            updated = true;
        }

        if (location["country"] !=  Settings::country())
        {
            location["country"] = Settings::country();
            updated = true;
        }

        if (location["city"] != Settings::city())
        {
            location["city"] = Settings::city();
            updated = true;
        }

        if (location["address"] != Settings::address())
        {
            location["address"] = Settings::address();
            updated = true;
        }

        if (location["longitude"] != Settings::longitude())
        {
            location["longitude"] = Settings::longitude();
            updated = true;
        }

        if (location["latitude"] != Settings::latitude())
        {
            location["latitude"] = Settings::latitude();
            updated = true;
        }
    }

    location["status"] = "ok";

    // if location changed, emit signal
    if (updated)
    {
        countBox();
        emit locationUpdated();
    }
}

void GeoLoc::reload()
{
    delete Settings::self();

    if (Settings::source() == Settings::EnumSource::GeolocationDataengine)
    {
        if (!Plasma::DataEngineManager::self()->engine("geolocation")->isValid())
        {
            Plasma::DataEngineManager::self()->loadEngine("geolocation");
            Plasma::DataEngineManager::self()->engine("geolocation")->connectSource("location", this);
        }
        geolocationData = Plasma::DataEngineManager::self()->engine("geolocation")->query("location");
        updateData(Settings::EnumSource::GeolocationDataengine);
    }
    else if (Settings::source() == Settings::EnumSource::Manual)
    {
        if (Plasma::DataEngineManager::self()->engine("geolocation")->isValid())
            Plasma::DataEngineManager::self()->unloadEngine("geolocation");
        updateData(Settings::EnumSource::Manual);
    }
}

void GeoLoc::clearLocation()
{
    location["country code"].clear();
    location["country"].clear();
    location["city"].clear();
    location["address"].clear();
    location["longitude"].clear();
    location["latitude"].clear();
    location["status"] = "n/a";
}

void GeoLoc::countBox()
{
    double pi = 3.14159265358979323846;
    double rad = pi / 180.0;    // this is not one radian, it's used for conversion from degrees to radians

    //double milesPerNauticalMile = 57.875 / 50.292;
    double kilometresPerNauticalMile = 1.852;
    double unitsPerNauticalMile = kilometresPerNauticalMile;

    double lat_range = Settings::range() / (unitsPerNauticalMile * 60);
    double lon_range = Settings::range() / (cos(location["latitude"].toDouble() * rad) * unitsPerNauticalMile * 60);

    // left, bottom, right, top
    QString box = QString("%1,%2,%3,%4").arg(location["longitude"].toDouble() - lon_range, 0, 'f', 10)
                  .arg(location["latitude"].toDouble() - lat_range, 0, 'f', 10)
                  .arg(location["longitude"].toDouble() + lon_range, 0, 'f', 10)
                  .arg(location["latitude"].toDouble() + lat_range, 0, 'f', 10);

    location["box"] = box;
}

bool GeoLoc::isValid()
{
    bool valid = false;

    if (location["longitude"].toFloat() > 0.0 && location["latitude"].toFloat() > 0.0
            && location["country"] != "" && location["city"] != "")
        valid = true;

    return valid;
}

void GeoLoc::getLocation(Plasma::DataEngine::Data &data)
{
    reload();
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

QString GeoLoc::getAddress()
{
    return location["address"].toString();
}

QString GeoLoc::getLongitude()
{
    return location["longitude"].toString();
}

QString GeoLoc::getLatitude()
{
    return location["latitude"].toString();
}

double GeoLoc::getRange()
{
    return Settings::range();
}

QString GeoLoc::getBox()
{
    return location["box"].toString();
}
