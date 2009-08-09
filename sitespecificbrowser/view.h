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

    void setupApplication();

    bool load( const QString &filename );
    bool save( const QString &filename );

    WebAppOptions *options() const;
    QList<QAction *> actions() const;

    KPluginInfo::List listWebApps();
    bool loadWebApp(const QString &name);

protected slots:
    void actionTriggered( const QString &script );
    void iconLoaded();

private:
    QSignalMapper *m_mapper;
    WebAppOptions *m_options;
    Page *m_page;
};

#endif // VIEW_H
