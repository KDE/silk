/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2011-02-11
 * Description : a kipi plugin to export images to WikiMedia web service
 *
 * Copyright (C) 2011 by Alexandre Mendes <alex dot mendes1988 at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef WMWIDGET_H
#define WMWIDGET_H

// Qt includes
#include <QWidget>

class QLabel;
class QSpinBox;
class QCheckBox;
class QButtonGroup;
class QProgressBar;
class KComboBox;
class KPushButton;

namespace KIPI
{
    class Interface;
    class UploadWidget;
}

namespace KIPIPlugins
{
    class ImagesList;
}

namespace KIPIWikiMediaPlugin
{
    enum WmDownloadType
    {
        WmMyAlbum = 0,
        WmFriendAlbum,
        WmPhotosMe,
        WmPhotosFriend
    };

class WmWidget : public QWidget
{
    Q_OBJECT

public:

    WmWidget(QWidget* parent, KIPI::Interface *iface);
    ~WmWidget();

    void updateLabels(const QString& name = "", const QString& url = "");

    KIPIPlugins::ImagesList* imagesList() const;

    QProgressBar* progressBar() const;


private Q_SLOTS:

    void slotResizeChecked();

private:

    KIPIPlugins::ImagesList* m_imgList;
    KIPI::UploadWidget*      m_uploadWidget;

    QLabel*                  m_headerLbl;
    QLabel*                  m_userNameDisplayLbl;
    KPushButton*             m_changeUserBtn;

    QCheckBox*               m_resizeChB;
    QSpinBox*                m_dimensionSpB;
    QSpinBox*                m_imageQualitySpB;

    QProgressBar*            m_progressBar;

    friend class WmWindow;
};

}

#endif
