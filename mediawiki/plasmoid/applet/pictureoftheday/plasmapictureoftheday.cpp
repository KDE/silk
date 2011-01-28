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

#include <QtGui/QLabel>
#include <QtGui/QGraphicsLinearLayout>
#include <KDE/KConfigDialog>

#include "setting.h"

#include "plasmapictureoftheday.h"

PlasmaPictureOfTheDay::PlasmaPictureOfTheDay(QObject * parent, const QVariantList & args)
    : Plasma::Applet(parent, args)
    , m_dateWidget(new Plasma::Label(this))
    , m_pictureWidget(new QLabel())
    , m_contentWidget(new Plasma::Label(this))
    , m_date(QDate::currentDate())
{
    setHasConfigurationInterface(true);
    setBackgroundHints(DefaultBackground);
    setAspectRatioMode(Plasma::KeepAspectRatio);
}

PlasmaPictureOfTheDay::~PlasmaPictureOfTheDay()
{
    delete m_pictureWidget;
}

void PlasmaPictureOfTheDay::init()
{
    Plasma::DataEngine * const engine = dataEngine("pictureoftheday");
    engine->connectSource("mediawiki", this);

    QGraphicsLinearLayout * const layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    QGraphicsProxyWidget * const pictureProxy = new QGraphicsProxyWidget(this);
    if (m_dateWidget == 0 || m_pictureWidget == 0 || m_contentWidget == 0 || layout == 0 || pictureProxy == 0) {
        setFailedToLaunch(true, i18n("Null pointer"));
    }

    m_dateWidget->setText(QDate::currentDate().toString("dddd dd MMMM"));
    m_dateWidget->setAlignment(Qt::AlignCenter);
    layout->addItem(m_dateWidget);

    pictureProxy->setWidget(m_pictureWidget);
    layout->addItem(pictureProxy);

    layout->addItem(m_contentWidget);

    setLayout(layout);
}


void PlasmaPictureOfTheDay::paintInterface(QPainter *, const QStyleOptionGraphicsItem *, const QRect & contentsRect)
{
    m_pictureWidget->setPixmap(m_picture.scaled(contentsRect.size(), Qt::KeepAspectRatio));
}

void PlasmaPictureOfTheDay::createConfigurationInterface(KConfigDialog * parent)
{
    m_setting = new Setting(m_providers, parent);
    parent->addPage(m_setting, i18n("Picture of the day sources"), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    connect(parent, SIGNAL(applyClicked()), this, SLOT(loadSettings()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(loadSettings()));
}


void PlasmaPictureOfTheDay::loadSettings()
{
    connectProvider(m_setting->comboBox->itemData(m_setting->comboBox->currentIndex()).toString());
}

void PlasmaPictureOfTheDay::dataUpdated(const QString & name, const Plasma::DataEngine::Data & data)
{
    if (name == "mediawiki") {
        m_providers = data;
        if (m_provider.isNull() && m_providers.begin() != m_providers.end()) {
            connectProvider(m_providers.begin().value().toString());
        }
    }
    else {
        m_picture = data["image"].value<QPixmap>();
        m_pictureWidget->setPixmap(m_picture);
        m_pictureWidget->setMinimumSize(m_picture.size());
        m_contentWidget->setText(data["content"].value<QString>());
        adjustSize();
    }
}

QString PlasmaPictureOfTheDay::provider() const
{
    return m_provider + ':' + m_date.toString("yyyy-MM-dd");
}

void PlasmaPictureOfTheDay::connectProvider(const QString & newProvider)
{
    Plasma::DataEngine * const engine = dataEngine("pictureoftheday");
    engine->disconnectSource(provider(), this);
    m_provider = newProvider;
    engine->connectSource(provider(), this);
}

K_EXPORT_PLASMA_APPLET(pictureoftheday, PlasmaPictureOfTheDay)

#include "plasmapictureoftheday.moc"
