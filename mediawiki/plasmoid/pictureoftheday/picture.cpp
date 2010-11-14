#include "picture.h"

Picture::Picture(QObject *parent) :
    QObject(parent)
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
void Picture::firstPicture(){}
void Picture::nextPicture(){}
void Picture::previousPicture(){}
#include "picture.moc"
