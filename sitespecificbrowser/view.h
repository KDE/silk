#ifndef VIEW_H
#define VIEW_H

#include <qwebview.h>
#include <qmap.h>
#include <qaction.h>

#include <KPluginInfo>

class QSignalMapper;
class Page;

/** Stuff that should be per-webapp */
struct WebAppOptions
{
    QUrl startUrl;
    QList<QUrl> allowedBases;
    QIcon windowIcon;
    QString windowTitle;
    QList<QAction *> actions;
};

class View : public QWebView
{
    Q_OBJECT

public:
    View( QWidget *parent=0 );

    void startApplication();

#if 0
    // This method is a temporary hack to setup a hard-coded application.
    void setupApplication();
#endif 

    WebAppOptions *options() const;
    QList<QAction *> actions() const;

    bool loadWebApp(const QString &name);

    static KPluginInfo::List listWebApps();

public slots:
    void evaluateScript( const QString &script );

protected slots:
    void iconLoaded();

private:
    QSignalMapper *m_mapper;
    WebAppOptions *m_options;
    Page *m_page;
};

#endif // VIEW_H
