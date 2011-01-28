/*
 *   Copyright 2010 by Hormiere Guillaume <hormiere.guillaume@gmail.com>
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
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

#ifndef PLASMAPICTUREOFTHEDAY_H
#define PLASMAPICTUREOFTHEDAY_H

#include <QtCore/QDate>
#include <Plasma/Applet>
#include <Plasma/Label>

class QLabel;
class Setting;

class PlasmaPictureOfTheDay : public Plasma::Applet
{

    Q_OBJECT

public:

    PlasmaPictureOfTheDay(QObject * parent, const QVariantList & args);

    virtual ~PlasmaPictureOfTheDay();

    virtual void init();

    virtual void paintInterface(QPainter * p, const QStyleOptionGraphicsItem * option, const QRect & contentsRect);

    virtual void createConfigurationInterface(KConfigDialog * parent);

private slots:

    void loadSettings();

    void dataUpdated(const QString & name, const Plasma::DataEngine::Data & data);

private:

    QString provider() const;

    void connectProvider(const QString & newProvider);

    Plasma::Label * const m_dateWidget;
    QLabel * const m_pictureWidget;
    Plasma::Label * const m_contentWidget;

    Setting * m_setting;

    const QDate m_date;
    QPixmap m_picture;

    Plasma::DataEngine::Data m_providers;
    QString m_provider;

};

#endif
