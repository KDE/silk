/*
 * Copyright 2009 Richard Moore <rich@kde.org>
 * Copyright 2009 Omat Holding B.V. <info@omat.nl>
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

#ifndef VIEW_H
#define VIEW_H

#include <qwebview.h>
#include <qmap.h>
#include <qaction.h>


#include <KActionCollection>
#include <KPluginInfo>

#include "webappaction.h"

class QProgressBar;
class QSignalMapper;
class Page;
class ScriptApi;
class WebApp;

/** Stuff that should be per-webapp */
struct WebAppOptions
{
    QString name;
    QString plugin;
    QUrl startUrl;
    QList<QUrl> allowedBases;
    QIcon windowIcon;
    QString windowTitle;
    QList<QAction *> actions;
};

class View : public QWebView
{
    Q_OBJECT

public:
    View( QWidget *parent=0 );
    ~View();

    WebAppOptions *options() const;
    QList<QAction *> actions() const;
    bool loadWebAppActions(WebApp *parent);
    QString name() const;
    QString plugin() const;

public slots:
    void evaluateScript( const QString &script );

protected slots:
    void iconLoaded();

private slots:
    void slotPrint( QWebFrame* );
    void updateActions();

private:
    void resetToolbarActions();
    void triggerUrlActions();
    bool isActionShown(WebAppAction *action);

private:
    /** should the provided action currently be shown? */
    bool actionShown(WebAppAction *action);
    KActionCollection *m_actionCollection;
    QSignalMapper *m_mapper;
    WebAppOptions *m_options;
    Page *m_page;
    ScriptApi *m_scriptapi;
    QProgressBar* m_progressBar;
    QTimer* m_progressTimer;
};

#endif // VIEW_H
