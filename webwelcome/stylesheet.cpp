/*
    Copyright 2009 by Sebastian Kügler <sebas@kde.org>

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

#include "stylesheet.h"

//Qt
#include <QFile>
#include <QPalette>

// KDE
#include <KDirWatch>
#include <KGlobalSettings>
#include <KStandardDirs>

// Plasma
#include <Plasma/Theme>


using namespace Plasma;

StyleSheet::StyleSheet(QObject *parent)
    : QObject(parent)
{
    //m_cssFile = "/home/sebas/kdesvn/src/playground-plasma/applets/crystal/user.css"; // For debugging quicker
    m_cssFile = KStandardDirs::locate("data", "plasma-applet-crystal/user.css");

    load(m_cssFile);
    m_cssWatch = new KDirWatch(this);
    m_cssWatch->addFile(m_cssFile);
    connect(m_cssWatch,SIGNAL(dirty(QString)),this,SLOT(load(QString)));
    connect(m_cssWatch,SIGNAL(created(QString)),this,SLOT(load(QString)));
    connect(m_cssWatch,SIGNAL(deleted(QString)),this,SLOT(load(QString)));

    connect(Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), SLOT(update()));
    connect(KGlobalSettings::self(), SIGNAL(kdisplayPaletteChanged()), SLOT(update()));
}

StyleSheet::~StyleSheet()
{
}

QString StyleSheet::styleSheet() const
{
    return m_styleSheet;
}

void StyleSheet::setFileName(const QString &cssFile)
{
    if (m_cssFile != cssFile) {
        // change watch file watch and load new CSS ...
        m_cssWatch->removeFile(m_cssFile);
        m_cssFile = cssFile;
        load(cssFile);
        m_cssWatch->addFile(m_cssFile);
    }
}

void StyleSheet::setStyleSheet(const QString &css)
{
    m_rawStyleSheet = css;
    update();
}

void StyleSheet::load(const QString &cssFile)
{
    QFile f(this);

    if (cssFile.isEmpty()) {
        f.setFileName(m_cssFile);
    } else {
        f.setFileName(cssFile);
    }
    m_baseDir = QString("file://").append(KUrl(m_cssFile).directory());

    kDebug() << "(Re)loading CSS" << cssFile;
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream t(&f);
        m_rawStyleSheet = t.readAll();
        f.close();
        update();
    } else {
        kDebug() << "CSS File not loaded, error reading file";
    }
}

/* borrowed from Plasma::Theme in 4.5, so we can depend on 4.4 */
const QString StyleSheet::processStyleSheet(const QString &css)
{
    QString stylesheet;
    if (css.isEmpty()) {
        stylesheet = QString("\n\
                    body {\n\
                        color: %textcolor;\n\
                        font-size: %fontsize;\n\
                        font-family: %fontfamily;\n\
                    }\n\
                    a:active  { color: %activatedlink; }\n\
                    a:link    { color: %link; }\n\
                    a:visited { color: %visitedlink; }\n\
                    a:hover   { color: %hoveredlink; text-decoration: none; }\n\
                    ");
    } else {
        stylesheet = css;
    }

    QHash<QString, QString> elements;
    // If you add elements here, make sure their names are sufficiently unique to not cause
    // clashes between element keys
    elements["%textcolor"] = Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor).name();
    elements["%backgroundcolor"] =
                Plasma::Theme::defaultTheme()->color(Plasma::Theme::BackgroundColor).name();
    elements["%visitedlink"] =
                Plasma::Theme::defaultTheme()->color(Plasma::Theme::VisitedLinkColor).name();
    elements["%activatedlink"] =
                Plasma::Theme::defaultTheme()->color(Plasma::Theme::HighlightColor).name();
    elements["%hoveredlink"] =
                Plasma::Theme::defaultTheme()->color(Plasma::Theme::HighlightColor).name();
    elements["%link"] = Plasma::Theme::defaultTheme()->color(Plasma::Theme::LinkColor).name();
    elements["%buttonbackgroundcolor"] =
                Plasma::Theme::defaultTheme()->color(Plasma::Theme::ButtonBackgroundColor).name();
    elements["%smallfontsize"] =
                QString("%1pt").arg(KGlobalSettings::smallestReadableFont().pointSize());

    QFont font = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DefaultFont);
    elements["%fontsize"] =
                QString("%1pt").arg(font.pointSize());
    elements["%fontfamily"] = font.family();

    foreach (const QString &k, elements.keys()) {
        stylesheet.replace(k, elements[k]);
    }
    return stylesheet;
}


void StyleSheet::update()
{
    m_styleSheet = processStyleSheet();
    kDebug() << "CSS:" << m_styleSheet;
    emit styleSheetChanged(m_styleSheet);
}

#include "stylesheet.moc"
