/*
 *   Copyright 2011 by Manuel Campomanes <campomanes.manuel@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef GENERALINFO_H
#define GENERALINFO_H

#include "mediawiki_export.h"

namespace mediawiki {

/**
 * @brief A general info.
 */
class MEDIAWIKI_EXPORT Generalinfo {

public:

    /**
     * @brief Constructs a general info.
     */
    Generalinfo();

    /**
     * @brief Destructs a general info.
     */
    ~Generalinfo();

    /**
     * @brief Get the name of the main page.
     * @return the name of the main page
     */
    QString mainPage() const;

    /**
     * @brief Set the name of the main page.
     * @param mainPage the name of the main page
     */
    void setMainPage(QString mainPage);

    /**
     * @brief Get the url of the page.
     * @return the url of the page
     */
    QString url() const;

    /**
     * @brief Set the url of the page.
     * @param url the url of the page
     */
    void setUrl(QString url);

    /**
     * @brief Get the name of the web site.
     * @return the name of the web site
     */
    QString siteName() const;

    /**
     * @brief Set the name of the web site.
     * @param siteName the name of the web site
     */
    void setSiteName(QString siteName);

    /**
     * @brief Get the generator.
     * @return the generator
     */
    QString generator() const;

    /**
     * @brief Set the generator.
     * @param generator
     */
    void setGenerator(QString generator);

    /**
     * @brief Get the PHP version.
     * @return the PHP version
     */
    QString phpVersion() const;

    /**
     * @brief Set the PHP version.
     * @param phpVersion the PHP version
     */
    void setPhpVersion(QString phpVersion);

    /**
     * @brief Get the PHP API name.
     * @return the PHP API name
     */
    QString phpApi() const;

    /**
     * @brief Set the PHP API name.
     * @param phpApi the PHP API name
     */
    void setPhpApi(QString phpApi);

    /**
     * @brief Get the type of the database.
     * @return the type of the database
     */
    QString dataBaseType() const;

    /**
     * @brief Set the type of the database.
     * @param dataBaseType the type of the database
     */
    void setDataBaseType(QString dataBaseType);

    /**
     * @brief Get the version of the database.
     * @return the version of the database
     */
    QString dataBaseVersion() const;

    /**
     * @brief Set the version of the database.
     * @param dataBaseVersion the version of the database
     */
    void setDataBaseVersion(QString dataBaseVersion);

    /**
     * @brief Get the rev number.
     * @return the rev number
     */
    QString rev() const;

    /**
     * @brief Set the rev number.
     * @param rev the rev number
     */
    void setRev(QString rev);

    /**
     * @brief Get the case.
     * @return the case
     */
    QString cas() const;

    /**
     * @brief Set the case.
     * @param cas the case
     */
    void setCase(QString cas);

    /**
     * @brief Get the rights titled.
     * @return the rights titled
     */
    QString rights() const;

    /**
     * @brief Set the rights titled.
     * @param rights the rights titled
     */
    void setRights(QString rights);

    /**
     * @brief Get the language.
     * @return the language
     */
    QString language() const;

    /**
     * @brief Set the language.
     * @param language
     */
    void setLanguage(QString language);

    /**
     * @brief Get the fallBack8bitEncoding.
     * @return the fallBack8bitEncoding
     */
    QString fallBack8bitEncoding() const;

    /**
     * @brief Set the fallBack8bitEncoding.
     * @param fallBack8bitEncoding
     */
    void setFallBack8bitEncoding(QString fallBack8bitEncoding);

    /**
     * @brief Get the writeApi.
     * @return the writeApi
     */
    QString writeApi() const;

    /**
     * @brief Set the writeApi.
     * @param writeApi
     */
    void setWriteApi(QString writeApi);

    /**
     * @brief Get the timeZone.
     * @return the timeZone
     */
    QString timeZone() const;

    /**
     * @brief Set the timeZone.
     * @param timeZone
     */
    void setTimeZone(QString timeZone);

    /**
     * @brief Get the timeOffset.
     * @return the timeOffset
     */
    QString timeOffset() const;

    /**
     * @brief Set the timeOffset.
     * @param timeOffset
     */
    void setTimeOffSet(QString timeOffset);

    /**
     * @brief Get the path of the article.
     * @return the path of the article
     */
    QString articlePath() const;

    /**
     * @brief Set the path of the article.
     * @param articlePath the path of the article
     */
    void setArticlePath(QString articlePath);

    /**
     * @brief Get the path of the script.
     * @return the path of the script
     */
    QString scriptPath() const;

    /**
     * @brief Set the path of the script.
     * @param scriptPath the path of the script
     */
    void setScriptPath(QString scriptPath);

    /**
     * @brief Get the path of the script file.
     * @return the path of the script file
     */
    QString script() const;

    /**
     * @brief Set the path of the script file.
     * @param script the path of the script file
     */
    void setScript(QString script);

    /**
     * @brief Get the path of the variant article.
     * @return the path of the variant article
     */
    QString variantArticlePath() const;

    /**
     * @brief Set the path of the variant article.
     * @param variantArticlePath the path of the variant article
     */
    void setVariantArticlePath(QString variantArticlePath);

    /**
     * @brief Get the url of the server.
     * @return the url of the server
     */
    QString serverUrl() const;

    /**
     * @brief Set the url of the server.
     * @param serverUrl the url of the server
     */
    void setServerUrl(QString serverUrl);

    /**
     * @brief Get the id of the wiki.
     * @return the id of the wiki
     */
    QString wikiId() const;

    /**
     * @brief Set the id of the wiki.
     * @param wikiId the id of the wiki
     */
    void setWikiId(QString wikiId);

    /**
     * @brief Get the time.
     * @return the time
     */
    QString time() const;

    /**
     * @brief Set the time.
     * @param time
     */
    void setTime(QString time);

private:

    struct ImageinfoPrivate * d;

};

}


#endif // GENERALINFO_H
