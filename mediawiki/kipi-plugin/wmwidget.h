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
