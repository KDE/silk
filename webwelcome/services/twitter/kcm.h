/*
    Copyright 2010 by Sebastian Kügler <sebas@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KCM_H
#define KCM_H

#include <KCModule>
#include <QLabel>

#include "ui_config.h"

namespace KWallet
{
    class Wallet;
}

class KcmTwitter: public KCModule
{
  Q_OBJECT

  public:
    explicit KcmTwitter(QWidget *parent = 0, const QVariantList &list = QVariantList());

  public Q_SLOTS:
    virtual void load();
    virtual void save();
    virtual void defaults();
    void emitChanged();

  private Q_SLOTS:
    void  readWallet();
    void  writeWallet();
    void loadPlasmoid();

  private:
    Ui::Config m_twitterConfig;
    KWallet::Wallet *m_wallet;
    QString m_walletFolder;
    QString m_username;
    QString m_password;

};

#endif
