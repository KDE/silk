/*
 * webappeditor.h
 *
 * Copyright (C) 2008 Sebastian Kuegler <sebas@kde.org>
 */
#ifndef WEBAPPEDITOR_H
#define WEBAPPEDITOR_H

#include <KPageDialog>

#include "ui_webapp.h"

class QListWidget;
class KDesktopFile;
class KPageDialog;
class KPushButton;

/**
 * This class serves as the main window for WebAppEditor.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Sebastian K?gler <sebas@kde.org>
 * @version 0.1
 */
class WebAppEditor : public KPageWidgetItem

{
    Q_OBJECT
public:
    WebAppEditor(QWidget *widget, const QString &name);

    /**
     * Default Destructor
     */
    virtual ~WebAppEditor();

private Q_SLOTS:
    void openActionFile();
    void save();
    void removeItem(QListWidgetItem *item);
    void addAllowedBase();

private:
    void setupMainWidget();
    void showActionFile();
    void dump();
    QStringList getItems(QListWidget *listWidget);
    void setItems(QListWidget *listWidget, const QStringList &list);

    Ui::WebAppEditor webAppUi;

    QString m_actionFile;
    QGridLayout *m_layout;
    QWidget *m_widget;

    KDesktopFile *m_desktopFile;
};

#endif // WEBAPPEDITOR_H
