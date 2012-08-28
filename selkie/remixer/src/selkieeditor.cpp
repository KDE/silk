
#include "dashboard.h"
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

SelkieEditor::SelkieEditor(const QString &path)
    : KXmlGuiWindow(),
    m_pages(0),
    m_dashboard(0)
{
    // accept dnd
    setAcceptDrops(true);
    //if (loadWebApp(path)) {
        init(path);
    /*} else {
        m_dashboard = new DashBoard(this);
        connect(m_dashboard, SIGNAL(openWebApp(const QString&)), this, SLOT(init(const QString)));
        setCentralWidget(m_dashboard);
    }
    */
    setupActions();
    setupGUI();
}

void SelkieEditor::init(const QString &path)
{
    QString p = path;


    if (m_pages) {
        delete m_pages;
        m_pages = 0;
    } else {
        //loadWebApp(p);
    }
    /*
    if (m_dashboard) {
        delete m_dashboard;
    }
    */
    if (!p.endsWith('/')) {
        p.append('/');
    }
    if (!m_pages) {
        m_pages = new KPageWidget(this);
        m_pages->setFaceType(KPageWidget::Auto);
    }
    //loadWebApp("/home/sebas/kdesvn/src/project-silk/selkie/services/silk/");
    if (!loadWebApp(p)) {
        kDebug() << "loading failed";
        //delete m_pages;
        if (!m_dashboard) {
            m_dashboard = new DashBoard(this);
            connect(m_dashboard, SIGNAL(openWebApp(const QString&)), this, SLOT(init(const QString&)));
        }
        setCentralWidget(m_dashboard);
        return;
    }
    kDebug() << "loading succeeded";

    setCentralWidget(m_pages);
}


void SelkieEditor::setupActions()
{
    KStandardAction::open(this, SLOT(open()), actionCollection());
    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());
    KStandardAction::save(this, SLOT(exportToFile()), actionCollection());
    KStandardAction::saveAs(this, SLOT(exportToFile()), actionCollection());
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
    QString path = KFileDialog::getExistingDirectory(KUrl("file:///home/sebas/kdesvn/src/project-silk/selkie/packages/"), this, i18nc("the directory selection dialogue for the webapp", "Open Web Application Directory"));
    init(path);
}

bool SelkieEditor::loadWebApp(const QString &path)
{
    m_dir = QDir(path);
    if (!m_dir.exists()) {
        kDebug() << "Directory doesn't exist, cannot load webapp";
        return false;
    }
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
    QString fname = m_dir.absolutePath().append("/metadata.desktop");
    if (!QFile::exists(fname)) {
        kDebug() << "metadata file doesn't exist:" << fname;
        return false;
    }
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
    return true;
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
