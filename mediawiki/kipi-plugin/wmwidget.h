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

    QString getDestinationPath();

    void updateLabels(const QString& name = "", const QString& url = "", bool uplPerm = false);

    long long getFriendID();
    QString getAlbumID();

    KIPIPlugins::ImagesList* imagesList() const;

    QProgressBar* progressBar() const;

Q_SIGNALS:

    void reloadAlbums(long long userID);

private Q_SLOTS:

    void slotReloadAlbumsRequest();
    void slotDownloadTypeChanged(int dlType);
    void slotFriendsIndexChanged(int index);
    void slotResizeChecked();

private:

    KIPIPlugins::ImagesList* m_imgList;
    KIPI::UploadWidget*      m_uploadWidget;

    QLabel*                  m_headerLbl;
    QLabel*                  m_userNameDisplayLbl;
    QLabel*                  m_permissionLbl;
    KPushButton*             m_changeUserBtn;
    KPushButton*             m_changePermBtn;

    QButtonGroup*            m_dlGrp;
    KComboBox*               m_friendsCoB;
    KComboBox*               m_albumsCoB;
    KPushButton*             m_newAlbumBtn;
    KPushButton*             m_reloadAlbumsBtn;

    QCheckBox*               m_resizeChB;
    QSpinBox*                m_dimensionSpB;
    QSpinBox*                m_imageQualitySpB;

    QProgressBar*            m_progressBar;

    friend class WmWindow;
};

}

#endif
