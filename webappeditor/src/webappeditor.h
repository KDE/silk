/*
 * webappeditor.h
 *
 * Copyright (C) 2008 Sebastian Kügler <sebas@kde.org>
 */
#ifndef WEBAPPEDITOR_H
#define WEBAPPEDITOR_H


#include <kxmlguiwindow.h>

#include "ui_prefs_base.h"

class QListWidget;
class KDesktopFile;
class KPushButton;
class KToggleAction;
class KUrl;

/**
 * This class serves as the main window for WebAppEditor.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Sebastian Kügler <sebas@kde.org>
 * @version 0.1
 */
class WebAppEditor : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    WebAppEditor();

    /**
     * Default Destructor
     */
    virtual ~WebAppEditor();

private Q_SLOTS:
    void openActionFile();
    void optionsPreferences();
    void save();

    void addShowLine();
    void addTriggerLine();

private:
    void setupActions();

private:
    void setupMainWidget();
    void showActionFile();
    QStringList getItems(QListWidget *listWidget);

    Ui::prefs_base ui_prefs_base ;

    QString m_actionFile;
    QGridLayout *m_layout;
    QWidget *m_widget;

    QListWidget *m_triggerUrls;
    QListWidget *m_showUrls;

    QLineEdit *m_showLine;
    QLineEdit *m_triggerLine;

    KPushButton *m_saveButton;

    QLabel *m_fileNameLabel;

    KDesktopFile *m_desktopFile;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // _WebAppEditor_H_