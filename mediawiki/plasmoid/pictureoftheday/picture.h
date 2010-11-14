#ifndef PICTURE_H
#define PICTURE_H

#include <QObject>
#include <Plasma/DataEngine>

class Picture : public QObject
{
Q_OBJECT
public:
    explicit Picture(QObject *parent = 0);
    QPixmap getPicture(){return this->m_picture;}

signals:
    void pictureUpdated();
public slots:
    void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);
    void firstPicture();
    void nextPicture();
    void previousPicture();
private:
    QPixmap m_picture;

};

#endif // PICTURE_H
