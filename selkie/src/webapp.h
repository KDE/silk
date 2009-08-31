/*
 * Copyright 2009 Sebastian Kügler <sebas@kde.org>
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

#ifndef WEBAPP_H
#define WEBAPP_H


#include <kxmlguiwindow.h>
#include <KPluginInfo>

#include "ui_prefs_base.h"

class View;
class QPrinter;
class KToggleAction;
class KUrl;

/**
 * This class serves as the main window for selkie.  It handles the
 * toolbars. Plugins are loaded as actions into the toolbar, and triggered "on"
 * the main View.
 *
 * @short Selkie browser main window class
 * @author Sebastian Kügler <sebas@kde.org>
 * @version 0.1
 */
class WebApp : public KXmlGuiWindow
{
    Q_OBJECT
public:
    WebApp();
    virtual ~WebApp();

    static KPluginInfo::List listWebApps(const QString &name = QString());

    bool loadWebApp(const QString &name);
    void startApplication();
    QString name();

private slots:
    void fileNew();
    void optionsPreferences();

private:
    void setupActions();

private:
    Ui::prefs_base ui_prefs_base ;
    View *m_view;

    QPrinter   *m_printer;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // _SITESPECIFICBROWSER_H_
