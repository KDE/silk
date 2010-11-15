#ifndef SETTING_H
#define SETTING_H

#include <QWidget>

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
private:
};

#endif // SETTING_H
