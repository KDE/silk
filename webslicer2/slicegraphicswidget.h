#ifndef SLICEGRAPHICSWIDGET_H
#define SLICEGRAPHICSWIDGET_H

#include <qgraphicswidget.h>

class QGraphicsSceneResizeEvent;
class QUrl;

class SliceGraphicsWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    SliceGraphicsWidget( QGraphicsWidget *parent=0 );
    ~SliceGraphicsWidget();

    void setUrl( const QUrl &url );
    void setElement( const QString &selector );

    QUrl url();
    QString element();

signals:
    void newSize(QRectF);
    void loadFinished();

protected slots:
    void createSlice();
    void resizeEvent ( QGraphicsSceneResizeEvent * event );

private:
    struct SliceGraphicsWidgetPrivate *d;
};

#endif // SLICEGRAPHICSWIDGET_H

