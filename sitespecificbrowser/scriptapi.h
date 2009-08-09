#ifndef SCRIPTAPI_H
#define SCRIPTAPI_H

#include <QtCore/QObject>
#include <QVariant>

class QWebFrame;
class View;

class ScriptApi : public QObject
{
    Q_OBJECT

public:
    ScriptApi( QObject *parent=0 );

    void setWebView( View *view );

    void setTrusted( bool yes=false );

public slots:
    //
    // Methods used to implement the greasemonkey api
    //

    /** GM_log - log messages to the JavaScript Console */
    void GM_log( const QString &message );

    /** GM_getValue - get script-specific configuration value */
    QVariant GM_getValue( const QString &key, const QVariant &defaultVal = QVariant() );

    /** GM_setValue - set script-specific configuration value */
    void GM_setValue( const QString &key, const QVariant &value );

    /** GM_registerMenuCommand - add a menu item to the User Script Commands submenu */
    // Not sure how to implement this

    /** GM_xmlhttpRequest - make an arbitrary HTTP request */
    // Not sure if we want to implement this

    //
    // Methods for a fluid-like api
    //

    void showNotification( const QString &text );

    // Only works if trusted
    bool include( const QString &filename );

private slots:
    void attachObject();

private:
    View *m_view;
    bool m_trusted;
};

#endif // SCRIPTAPI_H

