/*
 * webappactioneditor.h
 *
 * Copyright (C) 2008 Sebastian Kuegler <sebas@kde.org>
 */
#ifndef WEBAPPACTIONEDITOR_H
#define WEBAPPACTIONEDITOR_H

#include <QDir>
#include <KPageDialog>

#include "ui_webappaction.h"

class QListWidget;
class KDesktopFile;
class KPageDialog;
class KPushButton;
class KToggleAction;
class KUrl;

namespace KTextEditor
{
    class Document;
}

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
    WebAppActionEditor(KDesktopFile *file, QDir dir);
    virtual ~WebAppActionEditor();

    void setDir(QDir dir);

private Q_SLOTS:
    void openActionFile();
    void save();

    void addShowOnUrlLine();
    void addTriggerOnUrlLine();
    void addShowOnWildcardLine();
    void addTriggerOnWildcardLine();
    void loadScriptFile();
    void removeItem(QListWidgetItem *item);

private:
    void setupMainWidget();
    void loadDesktopFile(KDesktopFile *file);
    KUrl scriptUrl();
    QStringList getItems(QListWidget *listWidget);
    void setItems(QListWidget *listWidget, const QStringList &list);
    void dump();
    Ui::WebAppActionEditor actionUi;
    KTextEditor::Document *m_editorPart;
    QDir m_dir;

    KDesktopFile *m_desktopFile;
};

#endif // WEBAPPACTIONEDITOR_H_