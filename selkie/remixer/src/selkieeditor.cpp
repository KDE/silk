
#include "selkieeditor.h"
#include "webappeditor.h"
#include "webappactioneditor.h"
#include "package.h"

#include <KAction>
#include <KActionCollection>
#include <KApplication>
#include <KDebug>
#include <KDesktopFile>
#include <KFileDialog>
#include <KPageDialog>
#include <KPageWidgetItem>
#include <KPushButton>
#include <KStandardAction>

#include <QLabel>

SelkieEditor::SelkieEditor()
    : KXmlGuiWindow()
{
    // accept dnd
    setAcceptDrops(true);

    m_pages = new KPageWidget(this);
    m_pages->setFaceType(KPageWidget::Auto);
    loadWebApp("/home/sebas/kdesvn/src/project-silk/selkie/packages/silk/");
    //loadWebApp("/home/sebas/kdesvn/src/project-silk/selkie/services/silk/");
    setupActions();

    setCentralWidget(m_pages);
    setupGUI();
}


void SelkieEditor::setupActions()
{
    KStandardAction::open(this, SLOT(open()), actionCollection());
    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());
    KStandardAction::save(this, SLOT(exportToFile()), actionCollection());
}

void SelkieEditor::exportToFile()
{
    kDebug() << "exporting goes here" << m_dir;
    Package package("/home/sebas/kdesvn/src/project-silk/selkie/packages/examplepackage/", this);
    package.show();
    kWarning() << "Export needs implementation";
    //Package::exportPackage(KUrl("/home/sebas/kdesvn/src/project-silk/selkie/packages/examplepackage/"), //KUrl("/tmp/examplepackage.selkie"));
    //return SelkieEditor::exportPackage(m_projectPath, url);

}

void SelkieEditor::open()
{
    QString path = KFileDialog::getExistingDirectory(KUrl("file:///home/sebas/kdesvn/src/project-silk/webappeditor/examplepackage"), this, i18nc("the directory selection dialogue for the webapp", "Open Web Application Directory"));
    loadWebApp(path);
}

void SelkieEditor::loadWebApp(const QString &path)
{
    m_dir = QDir(path);
    const QString fileName = "*.desktop";
    QDir actionsDir = QDir(path + "actions/");
    kDebug() << "actions are in : " << actionsDir;
    kDebug() << "files:" << actionsDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
    QStringList files;
    files = m_dir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);

    QStringList actionFiles = actionsDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);

    kDebug() << "===== All Files:";
    // Clean up
    m_actionFiles.clear();
    // FIXME: clearing the pagewidget doesn't work :/
    QAbstractItemModel *model = m_pages->model();
    int rows = model->rowCount();
    for (int i = 0; i < rows; i++) {
        model->removeRow(i);
    }
    //kDebug() << "m_pages flushed rows:" << rows;

    // Web App page on top
    QString fname = path + "metadata.desktop";
    KDesktopFile *metadataFile = new KDesktopFile(fname);
    m_webAppEditor = new WebAppEditor(metadataFile);
    m_pages->addPage(m_webAppEditor);

    // Now our actions
    //foreach (KDesktopFile *f, m_actionFiles) {
    foreach (const QString &f, actionFiles) {
        QString fname = actionsDir.absolutePath().append("/").append(f);
        kDebug() << "Found file:" << fname;
        KDesktopFile *df = new KDesktopFile(fname);
        addAction(df);
    }
}

SelkieEditor::~SelkieEditor()
{
}

void SelkieEditor::addAction(KDesktopFile *file)
{
    WebAppActionEditor *editor = new WebAppActionEditor(file, m_dir);
    m_actionEditors << editor;
    m_pages->addSubPage(m_webAppEditor, editor);
}

void SelkieEditor::save()
{
    kDebug() << "TODO: implement saving here ...";
}


#include "selkieeditor.moc"
