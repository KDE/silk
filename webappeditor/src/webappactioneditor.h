/*
 * webappactioneditor.h
 *
 * Copyright (C) 2008 Sebastian Kuegler <sebas@kde.org>
 */
#ifndef WEBAPPACTIONEDITOR_H
#define WEBAPPACTIONEDITOR_H


#include <kxmlguiwindow.h>
#include <KPageDialog>
//#include "ui_prefs_base.h"
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
 * @author Sebastian K?gler <sebas@kde.org>
 * @version 0.1
 */
class WebAppActionEditor : public KPageWidgetItem
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    WebAppActionEditor();

    /**
     * Default Destructor
     */
    virtual ~WebAppActionEditor();

private Q_SLOTS:
    void openActionFile();
    //void optionsPreferences();
    void save();

    void addShowOnUrlLine();
    void addTriggerOnUrlLine();
    void addShowOnWildcardLine();
    void addTriggerOnWildcardLine();

    void removeItem(QListWidgetItem *item);
    void removeShowOnUrlItem(QListWidgetItem *item);

private:
    //void setupActions();

private:
    void setupMainWidget();
    void showActionFile();
    QStringList getItems(QListWidget *listWidget);
    void setItems(QListWidget *listWidget, const QStringList &list);
    void dump();
    Ui::WebAppActionEditor actionUi;

    QString m_actionFile;
    QGridLayout *m_layout;
    QWidget *m_widget;

    KDesktopFile *m_desktopFile;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // WEBAPPACTIONEDITOR_H_
