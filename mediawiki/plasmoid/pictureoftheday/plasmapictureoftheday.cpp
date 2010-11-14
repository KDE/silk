#include "plasmapictureoftheday.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QTimer>

#include <plasma/svg.h>
#include <plasma/theme.h>
#include "picture.h"


PlasmaPictureOfTheDay::PlasmaPictureOfTheDay(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),m_picture(new Picture(this))
{
    setBackgroundHints(DefaultBackground);
    resize(200, 200);
    connect(m_picture,SIGNAL(pictureUpdated),this,SLOT(updatePicture()));
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
    QString identifier = "wcpotd:" + QDate::currentDate().toString(Qt::ISODate);
    engine->disconnectSource(identifier, m_picture);
    //connect today's source
    identifier = "wcpotd:" + QDate::currentDate().toString(Qt::ISODate);
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

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(pictureoftheday, PlasmaPictureOfTheDay)

#include "plasmapictureoftheday.moc"
