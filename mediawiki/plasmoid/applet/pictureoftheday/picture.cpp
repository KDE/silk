#include "picture.h"
#include <Plasma/Label>
#include <QPainter>
#include <QWidget>
#include <QLabel>

Picture::Picture(QGraphicsWidget * parent, bool isLayout) :
        Plasma::Label(parent)
{
    Q_UNUSED(isLayout)
}

Picture::~Picture()
{

}

void Picture::dataUpdated(const QString &name, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(name)

    if (data.isEmpty()) {
        return;
    }
    m_picture = data["image"].value<QPixmap>();
    m_content = data["content"].value<QString>();
    emit pictureUpdated();
}
;
void Picture::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawPixmap(0,0,this->getPicture().scaled(this->size().width(),this->size().height(),Qt::KeepAspectRatio));
}

void Picture::firstPicture(){}
void Picture::nextPicture(){}
void Picture::previousPicture(){}


#include "picture.moc"
