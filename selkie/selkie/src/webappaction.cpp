/*
 * Copyright 2009-2010 Sebastian Kügler <sebas@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#include <QFile>

#include <KDebug>
#include <KIcon>
#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <KStandardDirs>

#include "webapp.h"
#include "webappaction.h"

WebAppAction::WebAppAction(WebApp *parent )
    : KAction(parent)
{
    m_webappPlugin = parent->name();
    m_options = new WebAppActionOptions;
}

WebAppAction::~WebAppAction()
{
    delete m_options;
}

WebAppActionOptions *WebAppAction::options() const
{
    return m_options;
}

QString WebAppAction::name() const
{
    return m_options->name;
}

void WebAppAction::setPackageRoot(const QString root)
{
    m_options->packageRoot = root;
}

KPluginInfo::List WebAppAction::listWebAppActions(const QString &name)
{
    QString constraint;
    if (!name.isEmpty()) {
        constraint.append(QString("[X-Silk-WebApp] == '%1'").arg(name));
    }
    KService::List offers = KServiceTypeTrader::self()->query("Silk/WebApp/Action", constraint);
    return KPluginInfo::fromServices(offers);
}

bool WebAppAction::load(const KPluginInfo &info)
{
    QString comment = info.comment();

    if (comment.isEmpty()) {
        comment = i18n("No description available");
    }

    //kDebug() << "Silk/WebApp/Action:" << comment << info.pluginName() << info.property("X-Silk-ShowOnUrl") <<  info.property("X-Silk-TriggerOnUrl") << info.icon();
    //kDebug() << "Found plugin:" << info.pluginName() << info.name() << info.icon();
    m_options->name = info.pluginName();
    m_options->showOnUrl = info.property("X-Silk-ShowOnUrl").toStringList();
    m_options->triggerOnUrl = info.property("X-Silk-TriggerOnUrl").toStringList();
    m_options->showOnWildcard = info.property("X-Silk-ShowOnWildcard").toStringList();
    m_options->triggerOnWildcard = info.property("X-Silk-TriggerOnWildcard").toStringList();
    m_options->icon = KIcon(info.icon());
    m_options->text = info.name();


    if (!m_options->showOnUrl.isEmpty()) {
        //kDebug() << "=====> ShowOnUrl" << m_options->showOnUrl;
    } else if (!m_options->showOnWildcard.isEmpty()){
        //kDebug() << "=====> ShowOnWildcard" << m_options->showOnWildcard;
    }
    // Loading the JavaScript stuff
    QString script = info.property("X-Silk-Script").toString();
    QString scriptFile = info.property("X-Silk-ScriptFile").toString();

    if (!script.isEmpty()) {
        m_options->script = script;
    } else if (!scriptFile.isEmpty()) {
        m_options->script = loadScript(scriptFile);
    }
    // Output errors in the .desktop file in the console
    if (!scriptFile.isEmpty() && !script.isEmpty()) {
        kWarning() << "Both, X-Silk-Script and X-Silk-ScriptFile have been defined in the .desktop file";
        kWarning() << "X-Silk-Script takes precedence.";
    }
    if (scriptFile.isEmpty() && script.isEmpty()) {
        kWarning() << "Neither X-Silk-Script, nor X-Silk-ScriptFile has been defined for this action";
        kWarning() << "No action will be triggered.";
    }

    finishLoading();
    return true;
}

bool WebAppAction::load(const KConfigGroup &cfg)
{
    if (!cfg.isValid()) {
        kWarning() << "Could not load config group, invalid";
        return false;
    }
    m_options->name = cfg.readEntry("X-KDE-PluginInfo-Name", QString());
    m_options->showOnUrl = cfg.readEntry("X-Silk-ShowOnUrl", QStringList());
    m_options->triggerOnUrl = cfg.readEntry("X-Silk-TriggerOnUrl", QStringList());
    m_options->showOnWildcard = cfg.readEntry("X-Silk-ShowOnWildcard", QStringList());
    m_options->triggerOnWildcard = cfg.readEntry("X-Silk-TriggerOnWildcard", QStringList());
    m_options->icon = KIcon(cfg.readEntry("Icon", QString("edit-delete-page")));
    m_options->text = cfg.readEntry("Name", QString());

    if (!m_options->showOnUrl.isEmpty()) {
        //kDebug() << "=====> ShowOnUrl" << m_options->showOnUrl;
    } else if (!m_options->showOnWildcard.isEmpty()){
        //kDebug() << "=====> ShowOnWildcard" << m_options->showOnWildcard;
    }
    // Loading the JavaScript stuff
    QString script = cfg.readEntry("X-Silk-Script", QString());
    QString scriptFile = cfg.readEntry("X-Silk-ScriptFile", QString());

    if (!script.isEmpty()) {
        m_options->script = script;
    } else if (!scriptFile.isEmpty()) {
        m_options->script = loadScript(scriptFile);
    }
    // Output errors in the .desktop file in the console
    if (!scriptFile.isEmpty() && !script.isEmpty()) {
        kWarning() << "Both, X-Silk-Script and X-Silk-ScriptFile have been defined in the .desktop file";
        kWarning() << "X-Silk-Script takes precedence.";
    }
    if (scriptFile.isEmpty() && script.isEmpty()) {
        kWarning() << "Neither X-Silk-Script, nor X-Silk-ScriptFile has been defined for this action";
        kWarning() << "No action will be triggered.";
    }
    finishLoading();
    return true;
}

void WebAppAction::finishLoading()
{
    // FIXME: append X-Silk-Script to the contents of X-Silk-ScriptFile
    // should make it possible to first include a javascript lib, and
    // then executing parts of it as a one-liner.


    setText(m_options->text);
    setIcon(m_options->icon);
}

QString WebAppAction::loadScript(const QString &jsfile)
{
    if (jsfile.isEmpty()) {
        return QString();
    }
    kDebug() << "Attempting to load jsfile:" << jsfile;
    // FIXME: for packages, we need to consider the packageroot instead of the KGlobal dir
    // otherwise, scripts can't be loaded
    QString script;
    QString scriptfile;
    if (!m_options->packageRoot.isEmpty()) {
        // We're using a package
        scriptfile = m_options->packageRoot + "scripts/" + jsfile;
        kDebug() << "Looking up inside .selkie package" << scriptfile;
    } else {
        // Installed selkie app, look into KStandardDirs
        scriptfile = "silk/webapps/" + m_webappPlugin + "/scripts/" + jsfile;
        kDebug() << "Looking up installed WebApp" << scriptfile;
    }
    //kDebug() << "------ Search:" << scriptfile;
    //kDebug() << "Package Root:" << m_options->packageRoot;
    if (QUrl().isRelative()) {
        scriptfile = KGlobal::dirs()->findResource("data", scriptfile);
    }
    kDebug() << "++++ Found:" << jsfile << scriptfile;
    if (scriptfile.isEmpty()) {
        kDebug() << "Could not find script file";
        return QString();
    }
    QFile f(scriptfile);
    if (!f.open(QIODevice::ReadOnly)) {
        kWarning() << "Could not read scriptfile" << scriptfile;
        return QString();
    }

    QTextStream t(&f);
    script = t.readAll();
    f.close();
    //kDebug() << "Read Script" << endl << script;

    return script;
}
