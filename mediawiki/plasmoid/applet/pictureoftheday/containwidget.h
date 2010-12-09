#ifndef CONTAINWIDGET_H
#define CONTAINWIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <Plasma/Label>
#include <picture.h>

class ContainWidget : public QGraphicsWidget
{
public:
    ContainWidget( QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0 );
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    void setDate(QString date);
    void setImage(Picture*);
    void setText(QString text);
private:

    QGraphicsLinearLayout* m_layout;
    Plasma::Label* m_date;
    Plasma::Label* m_text;
};

#endif // CONTAINWIDGET_H
