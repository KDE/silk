#ifndef PICTURE_H
#define PICTURE_H

#include <QObject>
#include <QDate>
#include <Plasma/DataEngine>

class Picture : public QObject
{
Q_OBJECT
public:
    explicit Picture(QObject *parent = 0);
    QPixmap getPicture(){return this->m_picture;}
    QDate getCurrentDate(){return this->m_currentDate;}
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

};

#endif // PICTURE_H
