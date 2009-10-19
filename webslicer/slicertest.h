#ifndef SLICERTEST_H
#define SLICERTEST_H

#include <qwidget.h>

class Slicer;
class QLabel;

class SlicerTest : public QWidget
{
    Q_OBJECT

public:
    SlicerTest();

public slots:
    void startTest();

private:
    Slicer *slicer;
    QLabel *view;
};

#endif // SLICERTEST_H

