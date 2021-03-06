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

#ifndef WEBAPPACTION_H
#define WEBAPPACTION_H

#include <QDir>

#include <KAction>
#include <KConfigGroup>
#include <KDesktopFile>
#include <KPluginInfo>

class WebApp;

/** Stuff that should be per-webapp */
struct WebAppActionOptions
{
    QString name;
    QString text;
    QString tooltip;
    KIcon icon;
    QStringList triggerOnUrl;
    QStringList showOnUrl;
    QStringList triggerOnWildcard;
    QStringList showOnWildcard;
    QString script;
    QString packageRoot;
};

class WebAppAction: public KAction
{
    Q_OBJECT

public:
    WebAppAction(WebApp *parent=0 );
    ~WebAppAction();

    WebAppActionOptions* options() const;
    static QStringList listWebAppActions(const QUrl &name = QUrl());
    QString name() const;
    void setPackageRoot(const QString root);

public slots:
    //bool load();
    bool load(const QString &path);
    bool load(const KConfigGroup &cfg);

private:
    QString loadScript(const QString &jsfile);
    void finishLoading();
    WebAppActionOptions *m_options;
    QString m_webappPlugin;

};

#endif // WEBAPPACTION_H
