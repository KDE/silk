#ifndef PAGE_H
#define PAGE_H

#include <qwebpage.h>

class View;

class Page : public QWebPage
{
    Q_OBJECT

public:
    Page( View *view );

protected:
    bool acceptNavigationRequest( QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type );

private:
    View *view;
};

#endif // PAGE_H
