/*
 * webappactioneditor.h
 *
 * Copyright (C) 2008 Sebastian Kuegler <sebas@kde.org>
 */
#ifndef WEBAPPACTIONEDITOR_H
#define WEBAPPACTIONEDITOR_H

#include <KPageDialog>

#include "ui_webappaction.h"

class QListWidget;
class KDesktopFile;
class KPageDialog;
class KPushButton;
class KToggleAction;
class KUrl;

/**
 * This class serves as the main window for WebAppActionEditor.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Sebastian Kügler <sebas@kde.org>
 * @version 0.1
 */
class WebAppActionEditor : public KPageWidgetItem
{
    Q_OBJECT
public:
    WebAppActionEditor(KDesktopFile *file);
    virtual ~WebAppActionEditor();

private Q_SLOTS:
    void openActionFile();
    void save();

    void addShowOnUrlLine();
    void addTriggerOnUrlLine();
    void addShowOnWildcardLine();
    void addTriggerOnWildcardLine();

    void removeItem(QListWidgetItem *item);

private:
    void setupMainWidget();
    void loadDesktopFile(KDesktopFile *file);
    QStringList getItems(QListWidget *listWidget);
    void setItems(QListWidget *listWidget, const QStringList &list);
    void dump();
    Ui::WebAppActionEditor actionUi;

    KDesktopFile *m_desktopFile;
};

#endif // WEBAPPACTIONEDITOR_H_