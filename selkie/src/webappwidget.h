#ifndef WEBAPPWIDGET_H
#define WEBAPPWIDGET_H

#include <qgraphicsview.h>

class KMainWindow;
class QUrl;
class View;

class WebAppWidget : public QGraphicsView
{
    Q_OBJECT

public:
    WebAppWidget( KMainWindow *parent=0 );
    ~WebAppWidget();

    View* view();

protected slots:
    void resizeEvent ( QResizeEvent * event );
    void sizeChanged ( QRectF geometry );

private:
    struct WebAppWidgetPrivate *d;
};

#endif // WebAppWidget_H

