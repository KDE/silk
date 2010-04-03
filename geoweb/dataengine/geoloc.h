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

#ifndef LOCATION_H
#define LOCATION_H

#include <Plasma/DataEngine>

/**
 * This class provides information about geolocation to geoweb dataengine
 * mainly using geolocation dataengine.
 */
class GeoLoc : public QObject
{
    Q_OBJECT

    public:
        /**
         * Contructor, connects to geolocation dataengine
         *
         * @param parent    parent
         */
        GeoLoc(QObject *parent);
        /**
         * Disconnects geolocation engine
         */
        ~GeoLoc();

        /**
         * Bool method to determine if location data are valid.
         * Data are valid if latitude, longitude, country and city
         * attributes are set.
         *
         * @return @c   true if data are valid
         * @return @c   false otherwise
         */
        bool isValid();


    public Q_SLOTS:
        /**
         * Slot called when geolocation dataengine has new data
         *
         * @param source    data source of engine with new data
         * @param data      new data
         */
        void dataUpdated(const QString &source, const Plasma::DataEngine::Data &data);

        // getters
        /**
         * Puts location as Plasma::DataEngine::Data to the parameter
         *
         * @param data  sets to actual location data
         */
        void getLocation(Plasma::DataEngine::Data &data);
        /**
         * Returns country code, e.g. CZ
         */
        QString getCountryCode();
        /**
         * Return country
         */
        QString getCountry();
        /**
         * Returns city
         */
        QString getCity();
        /**
         * Returns longitude
         */
        QString getLongitude();
        /**
         * Returns latitude
         */
        QString getLatitude();


Q_SIGNALS:
        /**
         * Signal emitted when location has been updated.
         */
        void locationUpdated();

    private:
        // geolocation engine
        Plasma::DataEngine *geolocationEngine;
        // location data
        Plasma::DataEngine::Data location;
};

#endif // LOCATION_H
