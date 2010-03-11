/*
 * webappeditor.h
 *
 * Copyright (C) 2010 Sebastian Kügler <sebas@kde.org>
 */
#ifndef SELKIEPACKAGESTRUCTURE_H
#define SELKIEPACKAGESTRUCTURE_H


#include <Plasma/PackageStructure>



/**
 * This class represents the structure of a Selkie package.
 *
 * @short SelkiePackageStructure Structure for Selkie WebApp packages
 * @author Sebastian Kügler <sebas@kde.org>
 * @version 0.1
 */
class SelkiePackageStructure : public Plasma::PackageStructure

{
    Q_OBJECT
public:
    SelkiePackageStructure(QObject* parent = 0, QString name = "Selkie/WebApp");
    virtual ~SelkiePackageStructure();

};

#endif // SELKIEPACKAGESTRUCTURE_H
