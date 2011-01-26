#ifndef PICTURE_H
#define PICTURE_H

#include <QObject>
#include <QDate>
#include <Plasma/DataEngine>
#include <Plasma/Label>
#include <QGraphicsLayoutItem>

class Picture : public Plasma::Label
{
Q_OBJECT
public:
    Picture(QGraphicsWidget * parent = 0, bool isLayout = false);
    virtual ~Picture();
    QPixmap getPicture(){return this->m_picture;}
    QDate getCurrentDate(){return this->m_currentDate;}
    QString getContent(){return this->m_content;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option=0, QWidget *widget=0);
    void setCurrentDate(QDate d){this->m_currentDate = d;}


signals:
    void pictureUpdated();

public slots:
    void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);
    void firstPicture();
    void nextPicture();
    void previousPicture();

private:
    QPixmap m_picture;
    QDate m_currentDate;
    QString m_content;


};

#endif // PICTURE_H
