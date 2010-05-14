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

#include <QMessageBox>

#include "geocodehandler.h"

GeocodeHandler::GeocodeHandler(KConfigDialog *prnt, bool byCoord)
{
    parent = prnt;
    searchByCoord = byCoord;
    metGeocodeResponseTag = false;  // first tag has to be GeocodeResponse otherwise it fails
    firstResult = true;
}

bool GeocodeHandler::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &)
{
    if (!firstResult)
        return true;

    if (!metGeocodeResponseTag && qName != "GeocodeResponse")
    {
        errorStr = I18N_NOOP("Invalid Geocode response.");
        return false;
    }

    if (qName == "GeocodeResponse")
    {
        metGeocodeResponseTag = true;
    }
    else if (qName == "address_component")
    {
        types.clear();
        long_name.clear();
        short_name.clear();
    }
    else if (qName == "location")
    {
        lat.clear();
        lng.clear();
    }

    currentText.clear();
    return true;
}

bool GeocodeHandler::endElement(const QString &, const QString &, const QString &qName)
{
    if (!firstResult)
        return true;

    if (qName == "status")
    {
        if (currentText != "OK")
        {
            if (currentText == "ZERO_RESULTS")
                errorStr = I18N_NOOP("Place not found!");
            else
                errorStr = I18N_NOOP("Invalid input: ") + currentText;

            return false;
        }
    }
    else if (qName == "long_name")
    {
        long_name = currentText;
    }
    else if (qName == "short_name")
    {
        short_name = currentText;
    }
    else if (qName == "type")
    {
        types << currentText;
    }
    else if (qName == "address_component")
    {
        // city
        if (types.contains("locality"))
        {
            location["city"] = long_name;
        }
        // country
        else if (types.contains("country"))
        {
            location["country"] = long_name;
            location["country code"] = short_name;
        }
        // street
        else if (types.contains("route"))
        {
            route = long_name;
        }
        // street number
        else if (types.contains("street_number"))
        {
            streetNb = long_name;
        }
    }
    else if (qName == "lat")
    {
        lat = currentText;
    }
    else if (qName == "lng")
    {
        lng = currentText;
    }
    else if (qName == "location")
    {
        // if search was done by coordinates, do not change them
        if (!searchByCoord)
        {
            location["latitude"] = lat;
            location["longitude"] = lng;
        }
    }
    else if (qName == "result")
    {
        if (!route.isEmpty())
        {
            if (!streetNb.isEmpty())
            {
                location["address"] = route + " " + streetNb;
            }
            else
            {
                location["address"] = route;
            }
        }
        else
        {
            location["address"] = "";
        }

        firstResult = false;
    }

    return true;
}

bool GeocodeHandler::characters(const QString &str)
{
    if (firstResult)
        currentText += str;

    return true;
}

bool GeocodeHandler::fatalError(const QXmlParseException &exception)
{
    QMessageBox::information(parent, QObject::tr("Check Error"), QObject::tr("Some troubles occured:\n%1")
                             .arg(exception.message()));
    return false;
}

QString GeocodeHandler::errorString() const
{
    return errorStr;
}

Plasma::DataEngine::Data GeocodeHandler::getData()
{
    return location;
}
