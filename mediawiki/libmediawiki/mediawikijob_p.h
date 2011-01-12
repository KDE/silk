#ifndef MEDIAWIKIJOB_P_H
#define MEDIAWIKIJOB_P_H

#include "mediawiki.h"

namespace mediawiki {


class MediaWikiJobPrivate {

public:

    explicit MediaWikiJobPrivate(const MediaWiki & mediawiki)
        : mediawiki(mediawiki)
    {}

    MediaWiki const & mediawiki;

};

}
#endif // MEDIAWIKIJOB_P_H
