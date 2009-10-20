#ifndef SLICER_H
#define SLICER_H

#include <qobject.h>
#include <qpixmap.h>

class QUrl;

class Slicer : public QObject
{
    Q_OBJECT

public:
    Slicer( QObject *parent );
    ~Slicer();

    void setUrl( const QUrl &url );
    QPixmap renderElement( const QString &selector );

signals:
    void finished( bool );

private:
    class SlicerPrivate *d;
};

#endif // SLICER_H

