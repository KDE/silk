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

#ifndef PROVIDER_H
#define PROVIDER_H

#include <QDir>
#include <QObject>

#include <Plasma/DataEngine>
#include <kross/core/action.h>
#include <kross/core/actioncollection.h>

#include "geoloc.h"

#define PLUGINS_PATH "plasma_engine_geoweb/providerPlugins/"

/**
 * Class communicating with external plugins. For connecting to plugins is used
 * KDE Kross, so plugins can be written in python, ruby, js...
 */

class Provider : public QObject
{
    Q_OBJECT

    public:

        /**
         * QHash holding data from all plugins.
         */
        typedef QHash<QString, Plasma::DataEngine::Data> sourceData;

        /**
         * States of plugins (ok, loading, n/a)
         */
        QHash<QString, QString> states;

        /**
         * Constructor, creates Kross::ActionCollection.
         *
         * @param parent    parent
         * @param loc       Location object, exposed to plugins
         */
        Provider(QObject *parent, GeoLoc *loc);

        /**
         * Destructor.
         */
        ~Provider();

        /**
         * Returns list of available plugins.
         *
         * @return QStringList of available plugins
         */
        QStringList getSources();

    public Q_SLOTS:

        /**
         * Called when all plugins have to be updated.
         */
        void updateAllPlugins();

        /**
         * Called when requesting plugin.
         *
         * @param source    name of requested plugin
         */
        void requestPluginSlot(const QString &source);

        /**
         * Called when plugin should ne updated.
         *
         * @param source    name of requested plugin
         */
        void updatePluginSlot(const QString &source);

Q_SIGNALS:

        /**
         * Emitted when some plugin have new data.
         *
         * @param source    name of plugin
         * @param data      new data
         */
        //void dataUpdated(const QString &source, const Plasma::DataEngine::Data &data);
        void dataUpdated(const QString &source, const QHash<QString, QVariant> &data);

    private:
        // path to plugins
        QDir pluginsDir;
        // interpreters extensions, what kind of plugins can we ran
        // (py, rb, js,..)
        QHash<QString, QString> interpExt;
        // maps extensions to plugins, so we know what file are we calling in the dir
        // e.g. sources["helloPlugin"] = "py"
        QHash<QString, QString> sources;
        GeoLoc *location;
        sourceData m_sourceData;
        Kross::Action *plugin;
        Kross::ActionCollection *pluginsCollection;
        // remember if plugins is processing right now
        QHash<QString, bool> pluginsWorking;

        // loads sources accroding to directories in PLUGINS_PATH
        void loadSources();
        void requestPlugin(Kross::Action *plugin);

    private Q_SLOTS:
        // called by plugins to set data
        void setProperty(QString source, QString key, QString value);
        // when plugin finished sending data
        void done(const QString source);
};

#endif // PROVIDER_H
