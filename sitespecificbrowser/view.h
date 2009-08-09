#ifndef VIEW_H
#define VIEW_H

#include <qwebview.h>
#include <qmap.h>
#include <qaction.h>

#include <KPluginInfo>

#include "webappaction.h"

class QProgressBar;
class QSignalMapper;
class Page;
class ScriptApi;

/** Stuff that should be per-webapp */
struct WebAppOptions
{
    QString name;
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

    WebAppOptions *options() const;
    QList<QAction *> actions() const;

    static KPluginInfo::List listWebApps(const QString &name = QString());

    bool loadWebApp(const QString &name);
    void startApplication();

public slots:
    void evaluateScript( const QString &script );

protected slots:
    void iconLoaded();

private:
    bool loadWebAppActions();
    QSignalMapper *m_mapper;
    WebAppOptions *m_options;
    Page *m_page;
    ScriptApi *m_scriptapi;
    QProgressBar* m_progressBar;
    QTimer* m_progressTimer;
};

#endif // VIEW_H
