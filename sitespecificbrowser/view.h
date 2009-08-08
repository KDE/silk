#ifndef VIEW_H
#define VIEW_H

#include <qwebview.h>
#include <qmap.h>

class Page;

/** Stuff that should be per-webapp */
struct WebAppOptions
{
    QUrl startUrl;
    QList<QUrl> allowedBases;
    QIcon windowIcon;
    QString windowTitle;
    QMap<QString, QString> jsActions;
};

class View : public QWebView
{
    Q_OBJECT

public:
    View( QWidget *parent=0 );

    bool load( const QString &filename );
    bool save( const QString &filename );

    WebAppOptions *options() const;

protected slots:
    void iconLoaded();

private:
    void setupApplication();

private:
    WebAppOptions *m_options;
    Page *m_page;
};

#endif // VIEW_H
