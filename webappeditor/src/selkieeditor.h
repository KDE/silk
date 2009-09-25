#ifndef SELKIEEDITOR_H
#define SELKIEEDITOR_H

#include <KDesktopFile>
#include <KPageDialog>
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
class SelkieEditor : public KPageDialog
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    SelkieEditor();

    /**
     * Default Destructor
     */
    virtual ~SelkieEditor();

private Q_SLOTS:
    void save();

private:
    void loadWebApp();
    void addAction(KDesktopFile *file);
    WebAppEditor *m_webAppEditor;
    QList<WebAppActionEditor*> m_actionEditors;
    QList<KDesktopFile*> m_actionFiles;
    KDesktopFile *m_webAppPlugin;
    KDesktopFile *m_app;

    QDir m_dir;
};

#endif // SELKIEEDITOR_H
