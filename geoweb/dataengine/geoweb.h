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

#ifndef GEOWEB_H
#define GEOWEB_H

#include <KDebug>
#include <Plasma/DataEngine>

#include "geoloc.h"
#include "provider.h"

/**
 * Main geoweb dataengine class. Sends request to Provider object.
 * Getting location from Location object.
 */

class Geoweb : public Plasma::DataEngine
{
    Q_OBJECT

    public:

        /**
         * Constructor.
         *
         * @param parent    parent
         * @param args      ignored arguments
         */
        Geoweb(QObject *parent, const QVariantList &args);

        /**
         * Init method called after the dataengine is created.
         * Creates Location and Provider objects, connects signals and slots.
         */
        void init();

        /**
         * Returns list of available sources (plugins).
         *
         * @return QStringList of available sources
         */
        QStringList sources() const;

    public Q_SLOTS:

        /**
         * Slot called when new location from Location object is available
         */
        void gotNewLocation();

        /**
         * Slot called by Provider object to set data for dataengine
         *
         * @param source    name of source
         * @param data      data to be set
         */
        void setProviderData(const QString &source, const QHash<QString, QVariant> &data);

Q_SIGNALS:

        /**
         * Signal emmited for Provider object when new location is available.
         */
        void updatedLocation();

        /**
         * Emitted when sourceRequestEvent is called.
         *
         * @param plugin    name of requested source
         */
        void requestProvider(const QString &plugin);

        /**
         * Emitted when updateSourceEvent is called.
         *
         * @param plugin    name of the requested source
         */
        void updateProvider(const QString &plugin);

    protected:
        bool sourceRequestEvent(const QString &source);
        bool updateSourceEvent(const QString &source);

    private:
        Plasma::DataEngine::Data locationData;
        Provider *providers;
        GeoLoc *location;
};

#endif // GEOWEB_H
