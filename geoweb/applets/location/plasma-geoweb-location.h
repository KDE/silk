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

#ifndef LOCATION_HEADER
#define LOCATION_HEADER

#include <QtNetwork>

#include <KConfigDialog>
#include <Plasma/Applet>
#include <Plasma/Label>
#include <Plasma/DataEngine>

#include "ui_locationConf.h"

#include "settings.h"

/**
 * Main applet for Geoweb dataengine. It displays current location
 * and provides configuration interface to the dataengine.
 */

class Location : public Plasma::Applet
{
    Q_OBJECT
    public:
        /**
         * Basic contructor.
         *
         * @param parent    parent
         * @param args      arguments
         */
        Location(QObject *parent, const QVariantList &args);
        /**
         * Destructor
         */
        ~Location();

        /**
         * Method called rigth after object is created. It's taking care of
         * complete initialization of the applet.
         */
        void init();
        /**
         * Method creating configuration interface
         */
        void createConfigurationInterface(KConfigDialog *parent);

    public Q_SLOTS:
        /**
         * Slot called when geolocation dataengine has new data
         *
         * @param source    data source of engine with new data
         * @param data      new data
         */
        void dataUpdated(const QString &source, const Plasma::DataEngine::Data &data);
        /**
         * Save config
         */
        void configChanged();

    private:
        Plasma::Label *labels;                  // labels on the left side of the applet
        Plasma::Label *dataLabels;              // labels on the right side
        Ui::LocationConf locationConfUi;        // configuration widget ui
        Plasma::DataEngine *geowebEngine;       // geoweb dataengine
        Plasma::DataEngine::Data location;      // location data
        KConfigDialog *configParent;            // cofniguration widget parent
        QUrl url;                               // url for google geocode with params
        QNetworkReply *reply;                   // google's reply
        QNetworkAccessManager qnam;             // network access manager

        void connectEngine();           // connect to dataengine
        void disableAddress();          // disable address line edit fields on configuration widget
        void disableCoordinates();      // disable coordinates line edit fields on configuration widget
        void setConfigLocation(Plasma::DataEngine::Data loc);       // sets location line edit fields to given location
        void invalidInput();            // clear other fields when invalid options were inserted

    private slots:
        void checkLocation();       // check user inserted location against google geocode
        void httpFinished();        // called when google response is downloaded
        void redirect(QUrl url);    // in case of redirection need
};

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(geoweb-location, Location)

#endif
