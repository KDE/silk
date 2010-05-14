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

#ifndef HELLO_HEADER
#define HELLO_HEADER

#include <Plasma/Applet>
#include <Plasma/Label>

/**
 * Example applet for Geoweb dataengine.
 */

class Hello : public Plasma::Applet
{
    Q_OBJECT
    public:
        /**
         * Basic contructor.
         *
         * @param parent    parent
         * @param args      arguments
         */
        Hello(QObject *parent, const QVariantList &args);
        /**
         * Destructor
         */
        ~Hello();

        /**
         * Method called rigth after object is created. It's taking care of
         * complete initialization of the applet.
         */
        void init();

    public Q_SLOTS:
        /**
         * Slot called when geolocation dataengine has new data
         *
         * @param source    data source of engine with new data
         * @param data      new data
         */
        void dataUpdated(const QString &source, const Plasma::DataEngine::Data &data);

    private:
        Plasma::Label *key;
        Plasma::Label *text;

        void connectEngine();
};

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(geoweb-hello, Hello)

#endif
