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


#include "setting.h"
#include "plasmapictureoftheday.h"
#include <Plasma/Theme>
#include <QSvgRenderer>

PlasmaPictureOfTheDay::PlasmaPictureOfTheDay(QObject * parent, const QVariantList & args)
    : Plasma::Applet(parent, args)
    , m_dateWidget(new Plasma::Label(this))
    , m_pictureWidget(new QLabel())
    , m_contentWidget(new Plasma::Label(this))
    , m_layoutH(new QGraphicsLinearLayout(Qt::Horizontal, this))
    , m_layout(new QGraphicsLinearLayout(Qt::Vertical))
    , m_pictureProxy(new QGraphicsProxyWidget(this))
    , m_date(QDate::currentDate())
{
    setHasConfigurationInterface(true);
    setBackgroundHints(DefaultBackground);
    setAspectRatioMode(Plasma::KeepAspectRatio);
    setupButton();
}
void PlasmaPictureOfTheDay::setupButton()
{
    m_navigationWidget = new Plasma::PushButton[2];
    m_navigationWidget[0].setMaximumWidth(20);
    m_navigationWidget[1].setMaximumWidth(20);
    m_navigationWidget[0].setMaximumHeight(20);
    m_navigationWidget[1].setMaximumHeight(20);
    m_navigationWidget[1].setEnabled(false);
    Plasma::Theme theme;
    m_navigationWidget[0].setImage(theme.imagePath("widgets/arrows"),"left-arrow");
    m_navigationWidget[1].setImage(theme.imagePath("widgets/arrows"),"right-arrow");
    connect(&m_navigationWidget[0],SIGNAL(clicked()), this, SLOT(yesterday()));
    connect(&m_navigationWidget[1],SIGNAL(clicked()), this, SLOT(tomorrow()));
}

void PlasmaPictureOfTheDay::yesterday()
{
    Plasma::DataEngine * const engine = dataEngine("pictureoftheday");
    engine->disconnectSource(provider(), this);
    m_date = this->m_date.addDays(-1);
    engine->connectSource(provider(), this);
    m_navigationWidget[1].setEnabled(true);
}

void PlasmaPictureOfTheDay::tomorrow()
{
    if(m_date < QDate::currentDate())
    {
        Plasma::DataEngine * const engine = dataEngine("pictureoftheday");
        engine->disconnectSource(provider(), this);
        m_date = this->m_date.addDays(1);
        engine->connectSource(provider(), this);
    }
    if(m_date == QDate::currentDate())
        m_navigationWidget[1].setEnabled(false);
}

PlasmaPictureOfTheDay::~PlasmaPictureOfTheDay()
{
    delete m_pictureWidget;
    delete[] m_navigationWidget;
}

void PlasmaPictureOfTheDay::init()
{
    if (m_dateWidget == 0 || m_pictureWidget == 0 || m_contentWidget == 0 || m_layout == 0 || m_layoutH == 0 || m_pictureProxy == 0) {
        setFailedToLaunch(true, i18n("Null pointer"));
    }
    m_dateWidget->setText(QDate::currentDate().toString("dddd dd MMMM"));
    m_dateWidget->setAlignment(Qt::AlignCenter);
    m_layout->addItem(m_dateWidget);
    m_pictureProxy->setWidget(m_pictureWidget);
    m_layout->addItem(m_pictureProxy);
    m_layout->addItem(m_contentWidget);
    m_layoutH->addItem(&m_navigationWidget[0]);
    m_layoutH->addItem(m_layout);
    m_layoutH->addItem(&m_navigationWidget[1]);
    setLayout(m_layoutH);
    Plasma::DataEngine * const engine = dataEngine("pictureoftheday");
    engine->connectSource("mediawiki", this);
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
            adjustSize();
        }
    }
    else {
        m_picture = data["image"].value<QPixmap>();
        m_dateWidget->setText(m_date.toString("dddd dd MMMM"));
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
