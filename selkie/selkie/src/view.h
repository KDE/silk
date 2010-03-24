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
#include <QGraphicsWebView>
#include <qwebview.h>
#include <qmap.h>
#include <qaction.h>


#include <KActionCollection>
#include <KPluginInfo>

#include "webappaction.h"

class KMainWindow;
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
    QStringList styleSheets;
    QIcon windowIcon;
    QString windowTitle;
    QList<QAction *> actions;
};

class View : public QGraphicsWebView
{
    Q_OBJECT

public:
    View( KMainWindow *win, QGraphicsItem *parent = 0 );
    ~View();

    WebAppOptions *options() const;
    QList<QAction *> actions() const;
    bool loadWebAppActions(WebApp *parent);
    QString name() const;
    QString plugin() const;
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
public slots:
    void evaluateScript( const QString &script );

protected slots:
    void iconLoaded();

private slots:
    void slotPrint(QWebFrame*);
    void updateActions();
    void triggerUrlActions();
    void loadStyleSheets();
    void updateProgress(int progress);
    void showProgressBar();
    void hideProgressBar();
    void toggleProgressBar();
    void startTimer();

private:
    void resetToolbarActions();
    bool match (QStringList wildcards, QStringList urls);

private:
    /** should the provided action currently be shown? */
    bool actionShown(WebAppAction *action);
    KActionCollection *m_actionCollection;
    QSignalMapper *m_mapper;
    WebAppOptions *m_options;
    Page *m_page;
    ScriptApi *m_scriptapi;
    QTimer* m_progressTimer;
    KMainWindow* m_win;
    qreal m_progress;
    bool m_showProgressBar;
};

#endif // VIEW_H
