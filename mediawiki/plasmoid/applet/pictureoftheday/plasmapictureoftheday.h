#ifndef PictureOfTheDay_HEADER
#define PictureOfTheDay_HEADER
// We need the Plasma Applet headers
#include <KIcon>

#include <Plasma/Applet>
#include <Plasma/Svg>

class QSizeF;
class Picture;

// Define our plasma Applet
class PlasmaPictureOfTheDay : public Plasma::Applet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        PlasmaPictureOfTheDay(QObject *parent, const QVariantList &args);
        ~PlasmaPictureOfTheDay();

        // The paintInterface procedure paints the applet to screen
        void paintInterface(QPainter *p,
                const QStyleOptionGraphicsItem *option,
                const QRect& contentsRect);
        void init();
    private slots:
        void updatePicture();
        void reloadPicture();

    private:
        Picture* m_picture;
};

#endif
