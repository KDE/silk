/*
 * package.h
 *
 * Copyright (C) 2010 Sebastian Kügler <sebas@kde.org>
 */
#ifndef SELKIEPACKAGE_H
#define SELKIEPACKAGE_H


#include <KUrl>
#include <KIO/ListJob>


/**
 * This class represents the structure of a Selkie package.
 *
 * @short Package Structure for Selkie WebApp packages
 * @author Sebastian Kügler <sebas@kde.org>
 * @version 0.1
 */
class Package : public QObject

{
    Q_OBJECT
    public:
        Package(QString path = QString(), QObject* parent = 0);
        virtual ~Package();

        static bool exportPackage(const KUrl &exportUrl, const KUrl &targetFile);
        static bool importPackage(const KUrl &importFile, const KUrl &targetUrl);

        void install();
        void unpack();
        void show();
        bool isValid();

        QString pluginName();

    private Q_SLOTS:
        void entries(KIO::Job* job, const KIO::UDSEntryList &list);

    private:
        void readDir();
        void ls(const QStringList &list);
        bool install(const QString &source, const QString &destination);

        QString m_pluginName;

        KUrl m_root;
        KUrl m_package;

        QStringList m_actionFiles;
        QStringList m_scriptFiles;
        QString m_appFile;
        QString m_pluginFile;

        QString m_appPath;
        QString m_pluginPath;
        QString m_dataPath;
};

#endif // SELKIEPACKAGESTRUCTURE_H
