
#include "selkieeditor.h"
#include "webappeditor.h"
#include "webappactioneditor.h"

#include <KDebug>
#include <KPageDialog>
#include <KPageWidgetItem>
#include <KPushButton>

SelkieEditor::SelkieEditor()
    : KPageDialog()
{
    // accept dnd
    setAcceptDrops(true);

    setFaceType(KPageDialog::List);

    QWidget *widget = new QWidget(this);
    m_webAppEditor = new WebAppEditor(widget, "WebApp Settings");
    //KPageWidgetItem *editorItem = new KPageWidgetItem(m_webAppEditor, "WebApp Settings");
    addPage(m_webAppEditor);

    addAction();
}

SelkieEditor::~SelkieEditor()
{
}

void SelkieEditor::addAction()
{
    WebAppActionEditor *editor = new WebAppActionEditor();
    //KPageWidgetItem *actionItem = new KPageWidgetItem(editor, "My Action");
    m_actionEditors << editor;
    addPage(editor);
}


void SelkieEditor::save()
{
    // ...
    kDebug() << "---> Saving here ...";
}


#include "selkieeditor.moc"
