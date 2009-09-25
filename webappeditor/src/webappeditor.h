/*
 * webappeditor.h
 *
 * Copyright (C) 2008 Sebastian Kuegler <sebas@kde.org>
 */
#ifndef WEBAPPEDITOR_H
#define WEBAPPEDITOR_H


#include <kxmlguiwindow.h>
#include <KPageDialog>
//#include "ui_prefs_base.h"
#include "ui_webapp.h"

class QListWidget;
class KDesktopFile;
class KPageDialog;
class KPushButton;
class KToggleAction;
class KUrl;

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
    /**
     * Default Constructor
     */
    WebAppEditor(QWidget *widget, const QString &name);

    /**
     * Default Destructor
     */
    virtual ~WebAppEditor();

private Q_SLOTS:
    void openActionFile();
    //void optionsPreferences();
    void save();

private:
    //void setupActions();

private:
    void setupMainWidget();
    void showActionFile();
    void dump();
    Ui::WebAppEditor webAppUi;

    //KPageDialog *m_pageDialog;

    QString m_actionFile;
    QGridLayout *m_layout;
    QWidget *m_widget;

    KDesktopFile *m_desktopFile;
};

#endif // WEBAPPEDITOR_H
