#include "setting.h"

Setting::Setting(QWidget *parent) :
    QObject(parent)
{
    settingWidget = new QWidget();
    settingUI.setupUi(settingWidget);
}
