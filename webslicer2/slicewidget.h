#ifndef SLICEWIDGET_H
#define SLICEWIDGET_H

#include <qgraphicsview.h>

class QUrl;

class SliceWidget : public QGraphicsView
{
    Q_OBJECT

public:
    SliceWidget( QWidget *parent=0 );
    ~SliceWidget();

    void setUrl( const QUrl &url );
    void setElement( const QString &selector );

protected slots:
    void resizeEvent ( QResizeEvent * event );

private:
    struct SliceWidgetPrivate *d;
};

#endif // SLICEWIDGET_H

