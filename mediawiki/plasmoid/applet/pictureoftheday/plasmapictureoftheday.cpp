#include "plasmapictureoftheday.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QTimer>
#include <QDebug>

#include <KConfigDialog>

#include <plasma/svg.h>
#include <plasma/theme.h>
#include "picture.h"
#include "setting.h"


PlasmaPictureOfTheDay::PlasmaPictureOfTheDay(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),m_picture(new Picture(this))
{
    setHasConfigurationInterface(true);
    setBackgroundHints(DefaultBackground);
    resize(200, 200);
    connect(m_picture,SIGNAL(pictureUpdated()),this,SLOT(updatePicture()));
    m_provider = QString("wcpotd:");
}

void PlasmaPictureOfTheDay::updatePicture()
{
    update(rect());
}

PlasmaPictureOfTheDay::~PlasmaPictureOfTheDay()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    } else {
        // Save settings
    }
}

void PlasmaPictureOfTheDay::init()
{
    QTimer().singleShot(0,this,SLOT(reloadPicture()));
}

void PlasmaPictureOfTheDay::reloadPicture()
{
    Plasma::DataEngine *engine = dataEngine("potd");
    //disconnect yesterday's source
    QString identifier = m_provider + m_picture->getCurrentDate().toString(Qt::ISODate);
    engine->disconnectSource(identifier, m_picture);
    //connect today's source
    identifier = m_provider + QDate::currentDate().toString(Qt::ISODate);
    m_picture->setCurrentDate(QDate::currentDate());
    engine->connectSource(identifier, m_picture);
}

void PlasmaPictureOfTheDay::paintInterface(QPainter *p,
        const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    Q_UNUSED(option)
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);
    QPixmap pix(m_picture->getPicture());
    // Now we draw the applet, starting with our svg
    // We place the icon and text
    p->drawPixmap((int)contentsRect.left(), (int)contentsRect.top(),pix.scaled(QSize((int)contentsRect.width(), (int)contentsRect.height()),Qt::KeepAspectRatio));
    p->save();
    p->setPen(Qt::white);
    p->drawText(contentsRect,
                Qt::AlignBottom | Qt::AlignHCenter,
                "Hello!");
    p->restore();
}

void PlasmaPictureOfTheDay::createConfigurationInterface(KConfigDialog *parent)
{
    qDebug()<<"createConfigurationInterface";
    m_settingDialog = new Setting(parent);
    parent->addPage(m_settingDialog->settingWidget, i18n("Picture of the day sources"), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

void PlasmaPictureOfTheDay::configAccepted()
{
    Plasma::DataEngine *engine = dataEngine("potd");
    QString currentData = m_settingDialog->settingUI.comboBox->itemText(m_settingDialog->settingUI.comboBox->currentIndex());
    if( currentData == QString("Wikipedia") && m_provider != QString("wppotd:"))
    {
        qDebug()<<"martine";
        QString identifier = m_provider + m_picture->getCurrentDate().toString(Qt::ISODate);
        engine->disconnectSource(identifier, m_picture);
        m_provider = QString("wppotd:");
        identifier = m_provider + QDate::currentDate().toString(Qt::ISODate);
        m_picture->setCurrentDate(QDate::currentDate());
        engine->connectSource(identifier, m_picture);

    }
    if( currentData == QString("Mediawiki") && m_provider != QString("wcpotd:"))
    {
        QString identifier = m_provider + m_picture->getCurrentDate().toString(Qt::ISODate);
        engine->disconnectSource(identifier, m_picture);
        m_provider = QString("wcpotd:");
        identifier = m_provider + QDate::currentDate().toString(Qt::ISODate);
        m_picture->setCurrentDate(QDate::currentDate());
        engine->connectSource(identifier, m_picture);

    }
    update(rect());
}

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(pictureoftheday, PlasmaPictureOfTheDay)

#include "plasmapictureoftheday.moc"
