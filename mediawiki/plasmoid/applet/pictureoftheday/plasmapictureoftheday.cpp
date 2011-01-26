#include "plasmapictureoftheday.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QTimer>
#include <QDebug>
#include <QGraphicsProxyWidget>

#include <KConfigDialog>

#include <plasma/svg.h>
#include <plasma/theme.h>
#include <plasma/widgets/label.h>
#include "picture.h"
#include "setting.h"

#include <QGraphicsScene>


PlasmaPictureOfTheDay::PlasmaPictureOfTheDay(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),m_picture(new Picture(this, false)),m_containWidget(new ContainWidget(this))
{
    setHasConfigurationInterface(true);
    setBackgroundHints(DefaultBackground);

    resize(250,300);
    this->m_containWidget->resize(this->size());

    connect(m_picture,SIGNAL(pictureUpdated()),this,SLOT(updatePicture()));
    m_provider = QString("commons.wikimedia.org");

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
    Plasma::DataEngine *engine = dataEngine("pictureoftheday");
    //disconnect yesterday's source
    QString identifier = m_provider + ':' + m_picture->getCurrentDate().toString(Qt::ISODate);
    engine->disconnectSource(identifier, m_picture);
    //connect today's source
    identifier = m_provider + ':' + QDate::currentDate().toString(Qt::ISODate);
    m_picture->setCurrentDate(QDate::currentDate());
    engine->connectSource(identifier, m_picture);

    m_containWidget->setDate(m_picture->getCurrentDate().toString("dddd dd MMMM"));
    m_containWidget->setImage(m_picture);
    m_containWidget->setText(m_picture->getContent());
}

void PlasmaPictureOfTheDay::paintInterface(QPainter *p,
        const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    Q_UNUSED(contentsRect)
    //get image size scalled
    QSize sizeR = m_picture->getPicture().scaled(this->size().width(),this->size().height(),Qt::KeepAspectRatio).size();
    //set minimum size of the image
    //30 = double margin of 15
    m_picture->setMinimumSize(sizeR.width()-30, sizeR.height()-30);

    m_containWidget->resize(this->size());
    m_containWidget->paint(p,option,0);
}

void PlasmaPictureOfTheDay::createConfigurationInterface(KConfigDialog *parent)
{
    m_settingDialog = new Setting(parent);
    Plasma::DataEngine *engine = dataEngine("pictureoftheday");
    engine->connectSource(QString("mediawiki"), m_settingDialog);
    parent->addPage(m_settingDialog->settingWidget, i18n("Picture of the day sources"), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

void PlasmaPictureOfTheDay::configAccepted()
{
    Plasma::DataEngine *engine = dataEngine("pictureoftheday");
    QString identifier = m_provider + ':' + m_picture->getCurrentDate().toString(Qt::ISODate);
    engine->disconnectSource(identifier, m_picture);
    m_provider = m_settingDialog->settingUI.comboBox->itemData(m_settingDialog->settingUI.comboBox->currentIndex()).toString();
    identifier = m_provider + ':' + QDate::currentDate().toString(Qt::ISODate);
    m_picture->setCurrentDate(QDate::currentDate());
    engine->connectSource(identifier, m_picture);
    update(rect());
}

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(pictureoftheday, PlasmaPictureOfTheDay)

#include "plasmapictureoftheday.moc"
