
#include "selkieeditor.h"
#include "webappeditor.h"
#include "webappactioneditor.h"

#include <KActionCollection>
#include <KApplication>
#include <KDebug>
#include <KDesktopFile>
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
    m_pages->setFaceType(KPageWidget::List);
    loadWebApp("/home/sebas/kdesvn/src/project-silk/selkie/services/test/");

    setCentralWidget(m_pages);
    kDebug() << "set central widget";
    setupGUI();
    setupActions();
}


void SelkieEditor::setupActions()
{
    KStandardAction::open(qApp, SLOT(open()), actionCollection());
    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());

    //KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
}

void SelkieEditor::open()
{
    // this slot is called whenever the File->New menu is selected,
    // the New shortcut is pressed (usually CTRL+N) or the New toolbar
    // button is clicked

    // create a new window
}

void SelkieEditor::loadWebApp(const QString &path)
{
    m_dir = QDir(path);
    QStringList files;
    const QString fileName = "*.desktop";
    files = m_dir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
    //kDebug() << "Directory:" << m_dir;

    // FIXME: leaks.
    m_actionFiles.clear();

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
    m_pages->addPage(editor);
}

void SelkieEditor::save()
{
    kDebug() << "TODO: implement saving here ...";
}


#include "selkieeditor.moc"
