#include <QtNetwork/QNetworkRequest>
#include <QtGui/QDesktopServices>

#include "view.h"
#include "page.h"

Page::Page( View *view )
    : QWebPage( view ) 
{
    this->view = view;
}

bool Page::acceptNavigationRequest( QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type )
{
    if ( type == QWebPage::NavigationTypeLinkClicked ) {
	bool inScope = false;
	QUrl base;
	foreach( base, view->options()->allowedBases ) {
	    if ( base.isParentOf( request.url() ) ) {
		inScope = true;
	    }
	}

	if ( !inScope ) {
	    QDesktopServices::openUrl( request.url() );
	    return false;
	}
    }

    return QWebPage::acceptNavigationRequest( frame, request, type );
}
