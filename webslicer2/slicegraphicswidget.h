#ifndef SLICEGRAPHICSWIDGET_H
#define SLICEGRAPHICSWIDGET_H

#include <qgraphicswidget.h>

class QUrl;

class SliceGraphicsWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    SliceGraphicsWidget( QGraphicsWidget *parent=0 );
    ~SliceGraphicsWidget();

    void setUrl( const QUrl &url );
    void setElement( const QString &selector );

protected slots:
    void createSlice( bool );
    void resizeEvent ( QResizeEvent * event );

private:
    struct SliceGraphicsWidgetPrivate *d;
};

#endif // SLICEGRAPHICSWIDGET_H

