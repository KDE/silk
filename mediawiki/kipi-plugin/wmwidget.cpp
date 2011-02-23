#include "wmwidget.h"

// Qt includes

#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QProgressBar>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

// KDE includes

#include <KLocale>
#include <KDialog>
#include <KComboBox>
#include <KPushButton>

// LibKIPI includes

#include <libkipi/interface.h>
#include <libkipi/uploadwidget.h>

// Local includes

#include "imageslist.h"

namespace KIPIWikiMediaPlugin
{

WmWidget::WmWidget(QWidget* parent, KIPI::Interface *iface)
        : QWidget(parent)
{
    setObjectName("WmWidget");

    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // -------------------------------------------------------------------

    m_imgList  = new KIPIPlugins::ImagesList(iface, this);
    m_imgList->setControlButtonsPlacement(KIPIPlugins::ImagesList::ControlButtonsBelow);
    m_imgList->setAllowRAW(true);
    m_imgList->loadImagesFromCurrentSelection();
    m_imgList->listView()->setWhatsThis(
        i18n("This is the list of images to upload to your Wikimedia account."));

    QWidget* settingsBox           = new QWidget(this);
    QVBoxLayout* settingsBoxLayout = new QVBoxLayout(settingsBox);

    m_headerLbl = new QLabel(settingsBox);
    m_headerLbl->setWhatsThis(
        i18n("This is a clickable link to open the Wikimedia home page in a web browser."));
    m_headerLbl->setOpenExternalLinks(true);
    m_headerLbl->setFocusPolicy(Qt::NoFocus);

    // ------------------------------------------------------------------------

    QGroupBox* accountBox         = new QGroupBox(i18n("Account"), settingsBox);
    accountBox->setWhatsThis(
        i18n("This is the Wikimedia account that is currently logged in."));
    QGridLayout* accountBoxLayout = new QGridLayout(accountBox);

    QLabel *userNameLbl     = new QLabel(i18nc("wikimedia account settings", "Name:"), accountBox);
    m_userNameDisplayLbl    = new QLabel(accountBox);
    QLabel *permissionLbl   = new QLabel(i18n("Permission:"), accountBox);
    permissionLbl->setWhatsThis(
        i18n("Permission of KIPI Plugin application to upload photos directly. If not, user will need to manually approve uploaded photos in Wikimedia."));
    m_permissionLbl         = new QLabel(accountBox);
    m_changeUserBtn         = new KPushButton(
        KGuiItem(i18n("Change Account"), "system-switch-user",
                 i18n("Logout and change Wikimedia Account used for transfer")),
        accountBox);
    m_changePermBtn         = new KPushButton(
        KGuiItem(i18n("Change Permission"), "security-high",
                 i18n("Change permission for KIPI application for photo upload")),
        accountBox);

    accountBoxLayout->addWidget(userNameLbl,            0, 0, 1, 2);
    accountBoxLayout->addWidget(m_userNameDisplayLbl,   0, 2, 1, 2);
    accountBoxLayout->addWidget(permissionLbl,          1, 0, 1, 2);
    accountBoxLayout->addWidget(m_permissionLbl,        1, 2, 1, 2);
    accountBoxLayout->addWidget(m_changeUserBtn,        2, 0, 1, 2);
    accountBoxLayout->addWidget(m_changePermBtn,        2, 2, 1, 2);
    accountBoxLayout->setSpacing(KDialog::spacingHint());
    accountBoxLayout->setMargin(KDialog::spacingHint());

    // ------------------------------------------------------------------------

    QGroupBox* albBox    = new QGroupBox(settingsBox);
    albBox->setTitle(i18n("Destination"));
    albBox->setWhatsThis(
        i18n("This is the Wikimedia album to which selected photos will be uploaded."));
    QGridLayout* albumsBoxLayout  = new QGridLayout(albBox);

    QRadioButton* albMeRBtn = new QRadioButton(i18n("My &Album"), albBox);
    albMeRBtn->setWhatsThis(
        i18n("Download complete album of currently logged in user."));
    QRadioButton* albFrRBtn = new QRadioButton(i18n("Album &of My Friend"), albBox);
    albFrRBtn->setWhatsThis(
        i18n("Download complete album of selected friend."));
    QRadioButton* phMeRBtn  = new QRadioButton(i18n("Photos of &Me"), albBox);
    phMeRBtn->setWhatsThis(
        i18n("Download all photos of currently logged in user."));
    QRadioButton* phFrRBtn  = new QRadioButton(i18n("Photos of My &Friend"), albBox);
    phFrRBtn->setWhatsThis(
        i18n("Download all photos of selected friend."));

    m_dlGrp = new QButtonGroup(albBox);
    m_dlGrp->addButton(albMeRBtn, WmMyAlbum);
    m_dlGrp->addButton(albFrRBtn, WmFriendAlbum);
    m_dlGrp->addButton(phMeRBtn, WmPhotosMe);
    m_dlGrp->addButton(phFrRBtn, WmPhotosFriend);

    QLabel* frLbl   = new QLabel(i18n("Friend:"), albBox);
    m_friendsCoB    = new KComboBox(albBox);
    m_friendsCoB->setEditable(false);

    QLabel* albLbl  = new QLabel(i18n("Album:"), albBox);
    m_albumsCoB     = new KComboBox(albBox);
    m_albumsCoB->setEditable(false);

    m_newAlbumBtn       = new KPushButton(
            KGuiItem(i18n("New Album"), "list-add",
                     i18n("Create new Wikimedia album")), accountBox);
    m_reloadAlbumsBtn   = new KPushButton(
            KGuiItem(i18nc("wikimedia album list", "Reload"), "view-refresh",
                     i18n("Reload album list")), accountBox);

    albumsBoxLayout->addWidget(albMeRBtn,           0, 0, 1, 2);
    albumsBoxLayout->addWidget(albFrRBtn,           1, 0, 1, 2);
    albumsBoxLayout->addWidget(phMeRBtn,            0, 3, 1, 2);
    albumsBoxLayout->addWidget(phFrRBtn,            1, 3, 1, 2);
    albumsBoxLayout->addWidget(frLbl,               2, 0, 1, 1);
    albumsBoxLayout->addWidget(m_friendsCoB,        2, 1, 1, 4);
    albumsBoxLayout->addWidget(albLbl,              3, 0, 1, 1);
    albumsBoxLayout->addWidget(m_albumsCoB,         3, 1, 1, 4);
    albumsBoxLayout->addWidget(m_newAlbumBtn,       4, 3, 1, 1);
    albumsBoxLayout->addWidget(m_reloadAlbumsBtn,   4, 4, 1, 1);

    // ------------------------------------------------------------------------

    QGroupBox* uploadBox    = new QGroupBox(i18n("Destination"), settingsBox);
    uploadBox->setWhatsThis(
        i18n("This is the location to which Wikimedia images will be downloaded."));
    QVBoxLayout* uploadBoxLayout = new QVBoxLayout(uploadBox);
    m_uploadWidget = iface->uploadWidget(uploadBox);
    uploadBoxLayout->addWidget(m_uploadWidget);

    // ------------------------------------------------------------------------

    QGroupBox* optionsBox         = new QGroupBox(i18n("Options"), settingsBox);
    optionsBox->setWhatsThis(
        i18n("These are options that will be applied to photos before upload."));
    QGridLayout* optionsBoxLayout = new QGridLayout(optionsBox);

    m_resizeChB     = new QCheckBox(optionsBox);
    m_resizeChB->setText(i18n("Resize photos before uploading"));
    m_resizeChB->setChecked(false);

    m_dimensionSpB  = new QSpinBox(optionsBox);
    m_dimensionSpB->setMinimum(0);
    m_dimensionSpB->setMaximum(5000);
    m_dimensionSpB->setSingleStep(10);
    m_dimensionSpB->setValue(600);
    m_dimensionSpB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_dimensionSpB->setEnabled(false);
    QLabel* dimensionLbl = new QLabel(i18n("Maximum dimension:"), optionsBox);

    m_imageQualitySpB = new QSpinBox(optionsBox);
    m_imageQualitySpB->setMinimum(0);
    m_imageQualitySpB->setMaximum(100);
    m_imageQualitySpB->setSingleStep(1);
    m_imageQualitySpB->setValue(85);
    m_imageQualitySpB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QLabel* imageQualityLbl = new QLabel(i18n("JPEG quality:"), optionsBox);

    optionsBoxLayout->addWidget(m_resizeChB,        0, 0, 1, 5);
    optionsBoxLayout->addWidget(imageQualityLbl,    1, 1, 1, 1);
    optionsBoxLayout->addWidget(m_imageQualitySpB,  1, 2, 1, 1);
    optionsBoxLayout->addWidget(dimensionLbl,       2, 1, 1, 1);
    optionsBoxLayout->addWidget(m_dimensionSpB,     2, 2, 1, 1);
    optionsBoxLayout->setRowStretch(3, 10);
    optionsBoxLayout->setSpacing(KDialog::spacingHint());
    optionsBoxLayout->setMargin(KDialog::spacingHint());

    m_progressBar = new QProgressBar(settingsBox);
    m_progressBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_progressBar->hide();

    // ------------------------------------------------------------------------

    settingsBoxLayout->addWidget(m_headerLbl);
    settingsBoxLayout->addWidget(accountBox);
    settingsBoxLayout->addWidget(albBox);
    settingsBoxLayout->addWidget(uploadBox);
    settingsBoxLayout->addWidget(optionsBox);
    settingsBoxLayout->addWidget(m_progressBar);
    settingsBoxLayout->setSpacing(KDialog::spacingHint());
    settingsBoxLayout->setMargin(KDialog::spacingHint());

    // ------------------------------------------------------------------------

    mainLayout->addWidget(m_imgList);
    mainLayout->addWidget(settingsBox);
    mainLayout->setSpacing(KDialog::spacingHint());
    mainLayout->setMargin(0);

    updateLabels();  // use empty labels until login

    // ------------------------------------------------------------------------

    connect(m_dlGrp, SIGNAL( buttonClicked(int) ),
            this, SLOT( slotDownloadTypeChanged(int) ));

    connect(m_reloadAlbumsBtn, SIGNAL( clicked() ),
            this, SLOT( slotReloadAlbumsRequest() ));

    connect(m_friendsCoB, SIGNAL( currentIndexChanged(int) ),
            this, SLOT( slotFriendsIndexChanged(int) ));

    connect(m_resizeChB, SIGNAL( clicked() ),
            this, SLOT( slotResizeChecked() ));

    // ------------------------------------------------------------------------

    phMeRBtn->hide();
    phFrRBtn->hide();
    albMeRBtn->hide();
    albFrRBtn->hide();
    frLbl->hide();
    m_friendsCoB->hide();
    uploadBox->hide();

}

WmWidget::~WmWidget()
{
}

KIPIPlugins::ImagesList* WmWidget::imagesList() const
{
    return m_imgList;
}

QProgressBar* WmWidget::progressBar() const
{
    return m_progressBar;
}

QString WmWidget::getDestinationPath()
{
    return m_uploadWidget->selectedImageCollection().uploadPath().toLocalFile();
}

void WmWidget::updateLabels(const QString& name, const QString& url, bool uplPerm)
{
    QString web("http://commons.wikimedia.org");
    if (!url.isEmpty())
        web = url;
    m_headerLbl->setText(QString("<b><h2><a href='%1'>"
                                 "<font color=\"#3B5998\">wikimedia</font>"
                                 "</a></h2></b>").arg(web));
    if (name.isEmpty())
    {
        m_userNameDisplayLbl->clear();
        m_permissionLbl->clear();
    }
    else
    {
        m_userNameDisplayLbl->setText(QString("<b>%1</b>").arg(name));
        if (uplPerm)
        {
            m_permissionLbl->setText(i18n("Direct upload"));
            m_permissionLbl->setWhatsThis(
                i18n("Uploaded photos will not need manual approval by user."));
        }
        else
        {
            m_permissionLbl->setText(i18n("Manual upload approval"));
            m_permissionLbl->setWhatsThis(
                i18n("Uploaded photos will wait in pending state until manually approved by user."));
        }
    }
}

void WmWidget::slotDownloadTypeChanged(int dlType)
{
    m_friendsCoB->setEnabled(dlType == WmPhotosFriend || dlType == WmFriendAlbum);
    m_albumsCoB->setEnabled(dlType == WmMyAlbum || dlType == WmFriendAlbum);

    if (dlType == WmPhotosMe)
    {
        m_friendsCoB->setCurrentIndex(-1); // deselect friend
        m_albumsCoB->setCurrentIndex(-1); // deselect friend
    }

    if (dlType == WmMyAlbum)
    {
        m_friendsCoB->setCurrentIndex(-1); // deselect friend
        emit reloadAlbums(0);
    }

    if (dlType == WmFriendAlbum)
        emit reloadAlbums(m_friendsCoB->itemData(
                                      m_friendsCoB->currentIndex()).toLongLong());
}

void WmWidget::slotFriendsIndexChanged(int index)
{
    if (index < 0)
        return;

    if (m_dlGrp->checkedId() == WmFriendAlbum)
        emit reloadAlbums(m_friendsCoB->itemData(index).toLongLong());
}

void WmWidget::slotReloadAlbumsRequest()
{
    // always list user's album, unless FriendAlbum is selected
    long long usrID = 0;
    if (m_dlGrp->checkedId() == WmFriendAlbum)
        usrID = m_friendsCoB->itemData(m_friendsCoB->currentIndex()).toLongLong();

    emit reloadAlbums(usrID);
}

void WmWidget::slotResizeChecked()
{
    m_dimensionSpB->setEnabled(m_resizeChB->isChecked());
    m_imageQualitySpB->setEnabled(m_resizeChB->isChecked());
}

long long WmWidget::getFriendID()
{
    if (m_dlGrp->checkedId() == WmPhotosFriend)
        return m_friendsCoB->itemData(m_friendsCoB->currentIndex()).toLongLong();

    return 0;
}

QString WmWidget::getAlbumID()
{
    if (m_dlGrp->checkedId() == WmMyAlbum
        || m_dlGrp->checkedId() == WmFriendAlbum)
        return m_albumsCoB->itemData(m_albumsCoB->currentIndex()).toString();

    return QString();
}

} // namespace KIPIWikimediaPlugin
