#include "setting.h"

Setting::Setting(QWidget *parent) :
    QObject(parent)
{
    settingWidget = new QWidget();
    settingUI.setupUi(settingWidget);
}


void Setting::dataUpdated(QString const &, Plasma::DataEngine::Data const & data) {
    this->settingUI.comboBox->clear();
    for (Plasma::DataEngine::Data::const_iterator it = data.begin(); it != data.end(); ++it) {
            this->settingUI.comboBox->addItem(it.key(), it.value());
    }
}
