#ifndef SLICEWIDGET_H
#define SLICEWIDGET_H

#include <qwidget.h>

class QUrl;

class SliceWidget : public QWidget
{
    Q_OBJECT

public:
    SliceWidget( QWidget *parent=0 );
    ~SliceWidget();

    void setUrl( const QUrl &url );
    void setElement( const QString &selector );

protected slots:
    void createSlice(bool);

private:
    struct SliceWidgetPrivate *d;
};

#endif // SLICEWIDGET_H

