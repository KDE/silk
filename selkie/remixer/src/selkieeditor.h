#ifndef SELKIEEDITOR_H
#define SELKIEEDITOR_H

#include <KDesktopFile>
#include <KPageDialog>
#include <KXmlGuiWindow>

#include <QDir>

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

private Q_SLOTS:
    void save();
    void open();
    void exportToFile();

private:
    void setupActions();
    void loadWebApp(const QString &path);
    void addAction(KDesktopFile *file);

    KPageWidget *m_pages;
    WebAppEditor *m_webAppEditor;
    QList<WebAppActionEditor*> m_actionEditors;
    QList<KDesktopFile*> m_actionFiles;
    KDesktopFile *m_webAppPlugin;
    KDesktopFile *m_app;

    QDir m_dir;
};

#endif // SELKIEEDITOR_H
