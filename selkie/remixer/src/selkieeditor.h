#ifndef SELKIEEDITOR_H
#define SELKIEEDITOR_H

#include <KDesktopFile>
#include <KPageDialog>
#include <KXmlGuiWindow>

#include <QDir>

class DashBoard;
class WebAppEditor;
class WebAppActionEditor;

/**
 * This class serves as the main window for Selkie Editor.
 *
 * @short Main window class
 * @author Sebastian Kügler <sebas@kde.org>
 * @version 0.1
 */
class SelkieEditor : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    SelkieEditor(const QString &path);

    /**
     * Default Destructor
     */
    virtual ~SelkieEditor();

public Q_SLOTS:
    void init(const QString &path);

private Q_SLOTS:
    void save();
    void open();
    void exportToFile();

private:
    void setupActions();
    bool loadWebApp(const QString &path);
    void addAction(KDesktopFile *file);

    KPageWidget *m_pages;
    DashBoard *m_dashboard;
    WebAppEditor *m_webAppEditor;
    QList<WebAppActionEditor*> m_actionEditors;
    QList<KDesktopFile*> m_actionFiles;
    KDesktopFile *m_webAppPlugin;
    KDesktopFile *m_app;

    QDir m_dir;
};

#endif // SELKIEEDITOR_H
