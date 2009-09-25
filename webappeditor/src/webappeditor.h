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
 * This class represents the edit webapp page.
 *
 * @short WebApp Editor class
 * @author Sebastian Kuegler <sebas@kde.org>
 * @version 0.1
 */
class WebAppEditor : public KPageWidgetItem

{
    Q_OBJECT
public:
    WebAppEditor(KDesktopFile *file);
    virtual ~WebAppEditor();

private Q_SLOTS:
    void openActionFile();
    void save();
    void removeItem(QListWidgetItem *item);
    void addAllowedBase();

private:
    void setupMainWidget();
    void loadDesktopFile(KDesktopFile *file);
    void dump();
    QStringList getItems(QListWidget *listWidget);
    void setItems(QListWidget *listWidget, const QStringList &list);

    Ui::WebAppEditor webAppUi;

    QWidget *m_widget;

    QString m_desktopFileName;
    KDesktopFile *m_desktopFile;
};

#endif // WEBAPPEDITOR_H
