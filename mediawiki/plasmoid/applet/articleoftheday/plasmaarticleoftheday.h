/*
 *   Copyright 2010 by de Vathaire Paolo <paolo.devathaire@gmail.com>
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


#ifndef PLASMAARTICLEOFTHEDAY_H
#define PLASMAARTICLEOFTHEDAY_H

#include <QtCore/QDate>
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QLabel>
#include <KDE/KConfigDialog>
#include <Plasma/Applet>
#include <Plasma/Label>
#include <Plasma/TextBrowser>
#include <Plasma/PushButton>

class QLabel;
class Setting;

class PlasmaArticleOfTheDay : public Plasma::Applet
{
    Q_OBJECT

public:
    PlasmaArticleOfTheDay(QObject * parent, const QVariantList & args);

    virtual ~PlasmaArticleOfTheDay();

    virtual void init();

    virtual void createConfigurationInterface(KConfigDialog * parent);

private slots:

    void loadSettings();

    void dataUpdated(const QString & name, const Plasma::DataEngine::Data & data);

    void yesterday();
    void tomorrow();

private:

    QString provider() const;
    void setupButton();
    void connectProvider(const QString & newProvider);

    Plasma::Label * const m_dateWidget;
    Plasma::TextBrowser * const m_contentWidget;
    Plasma::PushButton * m_navigationWidget;
    QGraphicsLinearLayout * m_layoutH;
    QGraphicsLinearLayout * m_layout;

    Setting * m_setting;

    QDate m_date;

    Plasma::DataEngine::Data m_providers;
    QString m_provider;
};

#endif // PLASMAARTICLEOFTHEDAY_H
