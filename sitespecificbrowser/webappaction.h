#ifndef WEBAPPACTION_H
#define WEBAPPACTION_H


#include <KAction>
#include <KPluginInfo>

class QSignalMapper;
class QProgressBar;
class Page;
class ScriptApi;

/** Stuff that should be per-webapp */
struct WebAppActionOptions
{
    QString name;
    QString plugin;
    QString text;
    KIcon icon;
    QString triggerOnUrl;
    QString showOnUrl;
    QString script;
    QStringList scriptFiles;
};

class WebAppAction: public KAction
{
    Q_OBJECT

public:
    WebAppAction( QObject *parent=0 );
    WebAppActionOptions* options() const;
    static KPluginInfo::List listWebAppActions(const QString &name = QString());

public slots:
    bool load(KPluginInfo info);

private:
    WebAppActionOptions *m_options;

};

#endif // WEBAPPACTION_H
