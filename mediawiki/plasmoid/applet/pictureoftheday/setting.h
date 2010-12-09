#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <Plasma/DataEngine>

#include "ui_setting.h"

class Setting : public QObject
{
    Q_OBJECT
public:
    explicit Setting(QWidget *parent = 0);
    Ui::Form settingUI;
    QWidget* settingWidget;

signals:

public slots:
    void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);
private:
};

#endif // SETTING_H
