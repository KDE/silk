#ifndef GOOGLESEARCH_H
#define GOOGLESEARCH_H

#include <qobject.h>
#include <qurl.h>

class QNetworkAccessManager;
class QNetworkReply;
class QIODevice;

class GoogleSearch : public QObject
{
    Q_OBJECT

public:
    /**
     * Contains information about a single match from the search.
     */
    class Result {
    public:
	Result() {
	};

	Result( const Result &r ) {
	    this->title = r.title;
	    this->url = r.url;
	};

	Result &operator= (const Result &other)  {
	    this->title = other.title;
	    this->url = other.url;

	    return *this;
	};

	/** The page title of the match. */
	QString title;
	/** The URL of the page containing the match. */
	QUrl url;	
    };

    GoogleSearch( QObject *parent );
    ~GoogleSearch();

    /**
     * Returns a list of matches.
     */
    QList<GoogleSearch::Result> results() const;

    void setNetworkAccessManager( QNetworkAccessManager *manager );
    QNetworkAccessManager *networkAccessManager() const;

public slots:
    void search( const QString &terms );
    void abort();

signals:
    void finished( bool success );

private slots:
    void finished( QNetworkReply *reply );

private:
    bool processSearchResult( QIODevice *source );

    struct GoogleSearchPrivate *d;
};

#endif // GOOGLESEARCH_H
