#include "picture.h"
#include <Plasma/Label>
#include <QPainter>
#include <QWidget>
#include <QLabel>

Picture::Picture(QGraphicsWidget * parent, bool isLayout) :
        Plasma::Label(parent)
{

}

Picture::~Picture()
{

}

void Picture::dataUpdated(const QString &name, const Plasma::DataEngine::Data &data)
{

    if (data.isEmpty()) {
        return;
    }
    m_picture = data[name].value<QPixmap>();
    emit pictureUpdated();
}
;
void Picture::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,0,this->getPicture().scaled(this->size().width(),this->size().height(),Qt::KeepAspectRatio));

}

void Picture::firstPicture(){}
void Picture::nextPicture(){}
void Picture::previousPicture(){}


#include "picture.moc"
