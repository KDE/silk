
#include "selkieeditor.h"
#include "webappeditor.h"
#include "webappactioneditor.h"

#include <KDebug>
#include <KDesktopFile>
#include <KPageDialog>
#include <KPageWidgetItem>
#include <KPushButton>

SelkieEditor::SelkieEditor()
    : KPageDialog()
{
    // accept dnd
    setAcceptDrops(true);

    setFaceType(KPageDialog::List);

    loadWebApp();
}

void SelkieEditor::loadWebApp()
{
    m_dir = QDir("/home/sebas/kdesvn/src/project-silk/selkie/services/test/");
    QStringList files;
    const QString fileName = "*.desktop";
    files = m_dir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
    kDebug() << "Directory:" << m_dir;
    foreach (const QString &fname, files) {
        kDebug() << "Found file:" << fname;
    }

    //return;
    m_webAppEditor = new WebAppEditor(new KDesktopFile("/home/sebas/kdesvn/src/project-silk/selkie/services/test/silk-webapp-test.desktop"));
    addPage(m_webAppEditor);
    addAction();
}

SelkieEditor::~SelkieEditor()
{
}

void SelkieEditor::addAction()
{
    WebAppActionEditor *editor = new WebAppActionEditor();
    m_actionEditors << editor;
    addPage(editor);
}

void SelkieEditor::save()
{
    kDebug() << "---> Saving here ...";
}


#include "selkieeditor.moc"
