/*
 *   Copyright 2009 by Sebastian Kügler <sebas@kde.org>
 *   Copyright 2009 by Richard Moore <rich@kde.org>

 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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

protected:
    QSize sizeHint () const;

protected slots:
    void resizeEvent ( QResizeEvent * event );
    void sizeChanged ( QSizeF newsize );

private:
    struct SliceWidgetPrivate *d;
};

#endif // SLICEWIDGET_H

