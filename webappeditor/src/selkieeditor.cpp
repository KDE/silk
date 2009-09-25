
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

    m_webAppEditor = new WebAppEditor();
    KPageWidgetItem *editorItem = new KPageWidgetItem(m_webAppEditor, "WebApp Settings");
    addPage(editorItem);

    addAction();
}

SelkieEditor::~SelkieEditor()
{
}

void SelkieEditor::addAction()
{
    WebAppActionEditor *editor = new WebAppActionEditor(this);
    KPageWidgetItem *actionItem = new KPageWidgetItem(editor, "My Action");
    m_actionEditors << editor;
    addPage(actionItem);
}


void SelkieEditor::save()
{
    // ...
    kDebug() << "---> Saving here ...";
}


#include "selkieeditor.moc"
