#ifndef KCM_H
#define KCM_H

#include <KCModule>
#include <QLabel>

class KcmWikipedia: public KCModule
{
  Q_OBJECT

  public:
    explicit KcmWikipedia(QWidget *parent = 0, const QVariantList &list = QVariantList());

    virtual void load();
    virtual void defaults();

  private:
    QLabel *lbl;
};

#endif
