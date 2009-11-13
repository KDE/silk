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

/**
 * @class SliceWidget 
 *
 * @short A QWidget displaying a part of a webpage
 *
 * SliceWidget provides a graphicsview embedding the SliceGraphicsWidget
 * graphicswidget that displays a part of a webpage, based on a QRectF
 * inside the webpage or a CSS selector (preferred).
 */
class SliceWidget : public QGraphicsView
{
    Q_OBJECT

public:
    SliceWidget( QWidget *parent=0 );
    ~SliceWidget();

    /**
     * Set the URL of the webpage the is being sliced. If you want
     * to use the more flexible CSS selector to render your slice,
     * the URL should point to the frame you're looking for, not to
     * the mainframe of the page. If the page doesn't use frames,
     * just the URL will work of course.
     *
     * @param url The page or frame URL to render
     **/
    void setUrl( const QUrl &url );

    /**
     * Specify the element of the webpage to be rendered. selector can
     * be any CSS that is matched by WebKit's findElement() API. If you
     * want to select a div with the id "mybox", specify "#mybox" here.
     * The first element found will be used.
     *
     * @param selector the name of the group to access
     **/
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

