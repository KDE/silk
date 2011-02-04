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

#include "plasmaarticleoftheday.h"
#include "setting.h"
#include <Plasma/Theme>
#include <QSvgRenderer>

PlasmaArticleOfTheDay::PlasmaArticleOfTheDay(QObject * parent, const QVariantList & args)
    : Plasma::Applet(parent, args)
    , m_dateWidget(new Plasma::Label(this))
    , m_contentWidget(new Plasma::Label(this))
    , m_layoutH(new QGraphicsLinearLayout(Qt::Horizontal, this))
    , m_layout(new QGraphicsLinearLayout(Qt::Vertical))
    , m_date(QDate::currentDate())
{
    setHasConfigurationInterface(true);
    setBackgroundHints(DefaultBackground);
    setAspectRatioMode(Plasma::KeepAspectRatio);
    setupButton();
}
void PlasmaArticleOfTheDay::setupButton()
{
    m_navigationWidget = new Plasma::PushButton[2];
    m_navigationWidget[0].setMaximumWidth(20);
    m_navigationWidget[1].setMaximumWidth(20);
    m_navigationWidget[0].setMaximumHeight(20);
    m_navigationWidget[1].setMaximumHeight(20);
    Plasma::Theme theme;
    m_navigationWidget[0].setImage(theme.imagePath("widgets/arrows"),"left-arrow");
    m_navigationWidget[1].setImage(theme.imagePath("widgets/arrows"),"right-arrow");
    connect(&m_navigationWidget[0],SIGNAL(clicked()), this, SLOT(yesterday()));
    connect(&m_navigationWidget[1],SIGNAL(clicked()), this, SLOT(tomorrow()));
}

void PlasmaArticleOfTheDay::yesterday()
{
    Plasma::DataEngine * const engine = dataEngine("articleoftheday");
    engine->disconnectSource(provider(), this);
    m_date = this->m_date.addDays(-1);
    engine->connectSource(provider(), this);
}

void PlasmaArticleOfTheDay::tomorrow()
{
    Plasma::DataEngine * const engine = dataEngine("articleoftheday");
    engine->disconnectSource(provider(), this);
    m_date = this->m_date.addDays(1);
    engine->connectSource(provider(), this);
}

PlasmaArticleOfTheDay::~PlasmaArticleOfTheDay()
{
    delete[] m_navigationWidget;
}

void PlasmaArticleOfTheDay::init()
{
    if (m_dateWidget == 0 || m_contentWidget == 0 || m_layout == 0 || m_layoutH == 0) {
        setFailedToLaunch(true, i18n("Null pointer"));
    }
    m_dateWidget->setText(QDate::currentDate().toString("dddd dd MMMM"));
    m_dateWidget->setAlignment(Qt::AlignCenter);
    m_layout->addItem(m_dateWidget);
    m_layout->addItem(m_contentWidget);
    m_layoutH->addItem(&m_navigationWidget[0]);
    m_layoutH->addItem(m_layout);
    m_layoutH->addItem(&m_navigationWidget[1]);
    setLayout(m_layoutH);
    Plasma::DataEngine * const engine = dataEngine("articleoftheday");
    engine->connectSource("mediawiki", this);
}


void PlasmaArticleOfTheDay::createConfigurationInterface(KConfigDialog * parent)
{
    m_setting = new Setting(m_providers, parent);
    parent->addPage(m_setting, i18n("Article of the day"), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    connect(parent, SIGNAL(applyClicked()), this, SLOT(loadSettings()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(loadSettings()));
}


void PlasmaArticleOfTheDay::loadSettings()
{

}

void PlasmaArticleOfTheDay::dataUpdated(const QString & name, const Plasma::DataEngine::Data & data)
{
    if (name == "mediawiki") {
        m_providers = data;
        if (m_provider.isNull() && m_providers.begin() != m_providers.end()) {
            connectProvider(m_providers.begin().value().toString());
            adjustSize();
        }
    }
    else {
        m_dateWidget->setText(m_date.toString("dddd dd MMMM"));
        m_contentWidget->setText(data["content"].value<QString>());
        adjustSize();
    }
}

QString PlasmaArticleOfTheDay::provider() const
{
    return m_provider + ':' + m_date.toString("yyyy-MM-dd");
}

void PlasmaArticleOfTheDay::connectProvider(const QString & newProvider)
{
    Plasma::DataEngine * const engine = dataEngine("articleoftheday");
    engine->disconnectSource(provider(), this);
    m_provider = newProvider;
    engine->connectSource(provider(), this);
}

K_EXPORT_PLASMA_APPLET(articleoftheday, PlasmaArticleOfTheDay)

#include "plasmaarticleoftheday.moc"
