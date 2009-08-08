#ifndef TOPLEVEL_H
#define TOPLEVEL_H

#include <qwebview.h>
#include <qwebpage.h>
#include <qmap.h>

class TopLevel;

/** Stuff that should be per-webapp */
struct WebAppOptions
{
    QUrl startUrl;
    QList<QUrl> allowedBases;
    QIcon windowIcon;
    QString windowTitle;
    QMap<QString, QString> jsActions;
};

class Page : public QWebPage
{
    Q_OBJECT

public:
    Page( TopLevel *toplevel );

protected:
    bool acceptNavigationRequest( QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type );

private:
    TopLevel *toplevel;
};

class TopLevel : public QWebView
{
    Q_OBJECT

public:
    TopLevel();

    bool load( const QString &filename );
    bool save( const QString &filename );

    WebAppOptions *options() const;

protected slots:
    void iconLoaded();

private:
    struct WebAppOptions *m_options;
    Page *m_page;
};

#endif // TOPLEVEL_H
