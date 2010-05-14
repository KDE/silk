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

#ifndef GEOCODEHANDLER_H
#define GEOCODEHANDLER_H

#include <QXmlDefaultHandler>

#include <KConfigDialog>
#include <Plasma/DataEngine>

/**
 * Handler for google's geocode response
 */

class GeocodeHandler : public QXmlDefaultHandler
{
    public:
        /**
         * Countructor.
         *
         * @param prnt      parent for QMessageBoxes
         * @param byCoord   boolean wheather request was for address(false) or for coordinates(true)
         */
        GeocodeHandler(KConfigDialog *prnt, bool byCoord = false);

        bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName,
                          const QXmlAttributes &attributes);
        bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
        bool characters(const QString &str);
        bool fatalError(const QXmlParseException &exception);
        QString errorString() const;

        /**
         * @return  parsed location
         */
        Plasma::DataEngine::Data getData();

    private:
        KConfigDialog *parent;
        Plasma::DataEngine::Data location;
        QString currentText;
        QString errorStr;
        bool metGeocodeResponseTag;
        bool searchByCoord;
        bool firstResult;

        // temporary values from XML
        QStringList types;
        QString long_name;
        QString short_name;
        QString route;
        QString streetNb;
        QString lat;
        QString lng;
};

#endif // GEOCODEHANDLER_H
