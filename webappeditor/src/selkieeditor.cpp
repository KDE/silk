
#include "selkieeditor.h"
#include "webappeditor.h"
#include "webappactioneditor.h"

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
    loadWebApp("/home/sebas/kdesvn/src/project-silk/selkie/services/test/");
    //loadWebApp("/home/sebas/kdesvn/src/project-silk/selkie/services/silk/");
    setupActions();

    setCentralWidget(m_pages);
    kDebug() << "set central widget";
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
    kDebug() << "exporting goes here";
}

void SelkieEditor::open()
{
    QString path = KFileDialog::getExistingDirectory(KUrl("file:///home/sebas/kdesvn/src/project-silk/selkie/services/silk"), this, i18nc("the directory selection dialogue for the webapp", "Open Web Application Directory"));
    loadWebApp(path);
}

void SelkieEditor::loadWebApp(const QString &path)
{
    m_dir = QDir(path);
    QStringList files;
    const QString fileName = "*.desktop";
    files = m_dir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);

    // Clean up
    m_actionFiles.clear();
    // FIXME: clearing the pagewidget doesn't work :/
    QAbstractItemModel *model = m_pages->model();
    int rows = model->rowCount();
    for (int i = 0; i < rows; i++) {
        model->removeRow(i);
    }
    kDebug() << "m_pages flushed rows:" << rows;

    foreach (const QString &f, files) {
        QString fname = m_dir.absolutePath().append("/").append(f);
        //kDebug() << "Found file:" << fname;
        KDesktopFile *df = new KDesktopFile(fname);

        KConfigGroup group = df->group("Desktop Entry");

        QString type = group.readEntry("Type", QString());
        QString servicetype = group.readEntry("X-KDE-ServiceTypes", QString());
        QString pname = group.readEntry("X-KDE-PluginInfo-Name", QString());

        //kDebug() << type << servicetype << pname;

        if (type == "Service") {
            if (servicetype == "Silk/WebApp") {
                pname = group.readEntry("X-KDE-PluginInfo-Name", QString());
                kDebug() << "Found WebApp:" << pname;
                m_webAppPlugin = df;
            } else if (servicetype == "Silk/WebApp/Action") {
                pname = group.readEntry("X-KDE-PluginInfo-Name", QString());
                kDebug() << "Found Action:" << pname;
                m_actionFiles << df;
            } else {
                kDebug() << "ignoring" << fname;
                delete df;
            }
        } else if (type == "Application") {
            m_app = df;
            kDebug() << "Application:" << group.readEntry("Name", QString());
        } else {
            kDebug() << "ignoring" << fname;
            delete df;
        }

    }

    // Web App page on top
    m_webAppEditor = new WebAppEditor(m_webAppPlugin);
    m_pages->addPage(m_webAppEditor);

    // Now our actions
    foreach (KDesktopFile *f, m_actionFiles) {
        addAction(f);
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
