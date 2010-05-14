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

#include <kstandarddirs.h>
#include <kross/core/manager.h>

#include "provider.h"

Provider::Provider(QObject *parent, GeoLoc *loc)
    : QObject(parent)
{
    location = loc;
    loadSources();
    Kross::Manager::self().addObject(location, "Location");
    Kross::Manager::self().addObject(this, "Provider");
    pluginsCollection = new Kross::ActionCollection("plugins", Kross::Manager::self().actionCollection());

    states["ok"] = "ok";
    states["loading"] = "loading";
    states["na"] = "n/a";
}

Provider::~Provider()
{
    foreach(Kross::Action *plg, pluginsCollection->actions())
        plg->callFunction("destroy");
}

void Provider::loadSources()
{
    pluginsDir = QDir(KGlobal::dirs()->findResource("data", PLUGINS_PATH));
    QStringList dirs = pluginsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    // load only plugins we have interpreters for
    QStringList interpreters = Kross::Manager::self().interpreters();
    foreach (QString interp, interpreters)
    {
        if (interp == "python")
            interpExt[interp] = "py";
        else if (interp == "ruby")
            interpExt[interp] = "rb";
        else if (interp == "kjs" || interp == "javascript" || interp == "qtscript")
            interpExt[interp] = "js";
        else
            continue;
    }

    // assign extensions to plugins
    foreach (QString dir, dirs)
    {
        foreach (QString ext, interpExt)
        {
            if (pluginsDir.exists("./" + dir + "/plugin." + ext))
            {
                sources[dir] = ext;
                break;
            }
        }
    }
}

QStringList Provider::getSources()
{
    return (QStringList)sources.keys();
}

void Provider::setProperty(QString source, QString name, QString value)
{
    m_sourceData[source][name] = value;
}

void Provider::done(const QString source)
{
    if (!m_sourceData[source].contains("status") || m_sourceData[source]["status"] == states["loading"])
        m_sourceData[source]["status"] = states["ok"];

    emit dataUpdated(source, m_sourceData[source]);
}

void Provider::requestPluginSlot(const QString &source)
{
    // if wrong plugin is called
    if (source.isEmpty() || !sources.contains(source))
        return;

    // if plugin already exists, update it only
    plugin = pluginsCollection->action(source);
    if (plugin)
    {
        updatePluginSlot(source);
        return;
    }

    m_sourceData[source].clear();
    m_sourceData[source]["status"] = states["loading"];
    done(source);

    // load plugin
    plugin = new Kross::Action(this, source);
    QString filename = pluginsDir.absolutePath();
    filename += "/" + source + "/plugin." + sources[source];
    plugin->setFile(filename);
    plugin->trigger();
    pluginsCollection->addAction(plugin);

    plugin->callFunction("init");

    if (location->isValid())
        plugin->callFunction("request");
}

void Provider::updatePluginSlot(const QString &source)
{
    // if wrong plugin is called
    if (source.isEmpty() || !sources.contains(source))
        return;

    // if location data is not valid, do not update plugins
    if (!location->isValid())
        return;

    // if plugin doesn't exist, create it
    plugin = pluginsCollection->action(source);
    if (!plugin)
    {
        requestPluginSlot(source);
        return;
    }

    m_sourceData[source].clear();
    m_sourceData[source]["status"] = states["loading"];

    plugin->callFunction("request");
}

void Provider::updateAllPlugins()
{
    foreach(Kross::Action *plg, pluginsCollection->actions())
        plg->callFunction("request");
}
