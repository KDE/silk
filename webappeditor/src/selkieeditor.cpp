
#include "selkieeditor.h"
#include "webappeditor.h"
#include "webappactioneditor.h"

#include <KApplication>
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

    // FIXME: leak.
    m_actionFiles.clear();

    foreach (const QString &f, files) {
        QString fname = m_dir.absolutePath().append("/").append(f);
        kDebug() << "Found file:" << fname;
        KDesktopFile *df = new KDesktopFile(fname);

        KConfigGroup group = df->group("Desktop Entry");

        QString type = group.readEntry("Type", QString());
        QString servicetype = group.readEntry("X-KDE-ServiceTypes", QString());
        QString pname = group.readEntry("X-KDE-PluginInfo-Name", QString());

        kDebug() << type << servicetype << pname;


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


    //return;


    m_webAppEditor = new WebAppEditor(m_webAppPlugin);
    addPage(m_webAppEditor);

    foreach (KDesktopFile *f, m_actionFiles) {
        addAction(f);
    }
}

SelkieEditor::~SelkieEditor()
{
}

void SelkieEditor::addAction(KDesktopFile *file)
{
    //KDesktopFile *f = new KDesktopFile("/home/sebas/kdesvn/src/project-silk/selkie/services/test/silk-webapp-test-editor.desktop");
    WebAppActionEditor *editor = new WebAppActionEditor(file);
    m_actionEditors << editor;
    addPage(editor);
}

void SelkieEditor::save()
{
    kDebug() << "---> Saving here ...";
}


#include "selkieeditor.moc"
