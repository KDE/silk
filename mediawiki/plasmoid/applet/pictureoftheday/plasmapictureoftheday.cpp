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
    resize(200, 300);
    connect(m_picture,SIGNAL(pictureUpdated()),this,SLOT(updatePicture()));
    m_provider = QString("wppotd:");
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
    if(pix.width() > pix.height() && this->size().height() > this->size().width())
    {
        this->resize(this->size().height(),this->size().width());
    }
    pix = pix.scaled(QSize((int)contentsRect.width()-34, (int)contentsRect.height()-74),Qt::KeepAspectRatio);

    p->drawText(contentsRect,
                Qt::AlignTop | Qt::AlignHCenter,
                m_picture->getCurrentDate().toString("dddd dd MMMM"));
    p->drawPixmap((int)contentsRect.left()+(contentsRect.size().width()-pix.size().width())/2, (int)contentsRect.top()+17,pix);
    p->save();
    QString text("Légi, Patres colendissimi, in Arabum monumentis, interrogatum Abdalam 1 Sarracenum, quid in hac quasi mundana scaena");

    p->drawText(contentsRect,
                Qt::AlignBottom | Qt::AlignJustify | Qt::TextWordWrap,
                text.toUtf8());
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
        QString identifier = m_provider + m_picture->getCurrentDate().toString(Qt::ISODate);
        engine->disconnectSource(identifier, m_picture);
        m_provider = QString("wppotd:");
        identifier = m_provider + QDate::currentDate().toString(Qt::ISODate);
        m_picture->setCurrentDate(QDate::currentDate());
        engine->connectSource(identifier, m_picture);

    }
    if( currentData == QString("Wikimedia") && m_provider != QString("wcpotd:"))
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
