/*
 * webappeditorview.h
 *
 * Copyright (C) 2007 Sebastian Kügler <sebas@kde.org>
 */
#ifndef WEBAPPEDITORVIEW_H
#define WEBAPPEDITORVIEW_H

#include <QtGui/QWidget>

#include "ui_webappeditorview_base.h"

class QPainter;
class KUrl;

/**
 * This is the main view class for webappeditor.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Sebastian Kügler <sebas@kde.org>
 * @version 0.1
 */

class webappeditorView : public QWidget, public Ui::webappeditorview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    webappeditorView(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~webappeditorView();

private:
    Ui::webappeditorview_base ui_webappeditorview_base;

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

private slots:
    void switchColors();
    void settingsChanged();
};

#endif // webappeditorVIEW_H
