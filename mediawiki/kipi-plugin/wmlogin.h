#ifndef WMLOGIN_H
#define WMLOGIN_H

// Qt includes

#include <QDialog>
#include <QComboBox>
#include <QUrl>

class QLabel;
class KLineEdit;

namespace KIPIWikiMediaPlugin
{

class WmLogin : public QDialog
{
    Q_OBJECT

public:

    WmLogin(QWidget* parent, const QString& header, const QString& _name=QString(),
                   const QString& _passwd=QString());
    ~WmLogin();

    QString name()     const;
    QString password() const;
    QString username() const;
    QUrl wiki() const;
    void setUsername(const QString&);
    void setPassword(const QString&);

private:

    QLabel*    m_headerLabel;
    KLineEdit* m_nameEdit;
    KLineEdit* m_passwdEdit;
    QComboBox* m_wikiSelect;
};

}

#endif // WMLOGIN_H
