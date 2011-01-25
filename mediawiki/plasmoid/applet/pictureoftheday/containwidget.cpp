#include "containwidget.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

ContainWidget::ContainWidget( QGraphicsItem * parent , Qt::WindowFlags wFlags ):
        QGraphicsWidget(parent,  wFlags)
{
    this->m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    this->setLayout(m_layout);

    this->m_layout->setContentsMargins(15,15,15,15);

}

void ContainWidget::setDate(QString date)
{
    this->m_date = new Plasma::Label(this);
    this->m_date->setText(date);
    this->m_date->setAlignment(Qt::AlignCenter);
    this->m_layout->addItem(m_date);
}

void ContainWidget::setImage(Picture *picture)
{
//    this->m_picture = new Picture(this, false);
//    this->m_image->setAlignment(Qt::AlignCenter);
//    this->m_image->setMinimumSize(110,135);
    this->m_layout->addItem(picture);
}

void ContainWidget::setText(QString text)
{
    this->m_text = new Plasma::Label(this);
    this->m_text->setText(text);
    this->m_layout->addItem(m_text);
}

void ContainWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}
