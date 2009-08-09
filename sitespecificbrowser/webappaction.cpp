
#include <KDebug>
#include <KIcon>
#include <KPluginInfo>
#include <KServiceTypeTrader>

#include "webappaction.h"

WebAppAction::WebAppAction( QObject *parent )
    : KAction(parent)
{
    m_options = new WebAppActionOptions;
}

KPluginInfo::List WebAppAction::listWebAppActions(const QString &name)
{
    QString constraint;
    if (!name.isEmpty()) {
        constraint.append(QString("[X-KDE-PluginInfo-Name] == '%1'").arg(name));
    }
    KService::List offers = KServiceTypeTrader::self()->query("Silk/WebApp/Action", constraint);
    return KPluginInfo::fromServices(offers);
}

bool WebAppAction::load(KPluginInfo info)
{
    QString comment = info.comment();

    if (comment.isEmpty()) {
        comment = i18n("No description available");
    }

    kDebug() << "Silk/WebApp/Action:" << comment << info.pluginName() << info.property("X-Silk-ShowOnUrl") <<  info.property("X-Silk-TriggerOnUrl") << info.icon();
    kDebug() << "Found plugin:" << info.pluginName() << info.name();

    m_options->name = info.pluginName();
    m_options->showOnUrl = info.property("X-Silk-ShowOnUrl").toString();
    m_options->triggerOnUrl = info.property("X-Silk-TriggerOnUrl").toString();
    m_options->scriptFiles = info.property("X-Silk-TriggerOnUrl").toStringList();
    m_options->icon = KIcon(info.icon());
    m_options->text = info.name();
    return true;
}

