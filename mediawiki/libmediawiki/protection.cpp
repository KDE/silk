/*
 *   Copyright 2010 by Vincent Garcia <xavier.vincent.garcia@gmail.com>
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
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "protection.h"
#include <algorithm>

namespace mediawiki {

    class ProtectionInfo {

    public:

        QString type;
        QString level;
        QString expiry;
        QString source;
    };
}
//un truc
using namespace mediawiki;

Protection::Protection()
    : d(new ProtectionInfo())
{
    d->type = "";
    d->level = "";
    d->expiry = "";
    d->source = "";
}

Protection::~Protection()
{
        delete d;
}

Protection & Protection::operator=(Protection other)
{
        std::swap(d, other.d);
        return *this;
}

bool operator==(const Protection & ptc, const Protection & ptc2)
{
return ptc.Type() == ptc2.Type() &&
       ptc.Level() == ptc2.Level() &&
       ptc.Expiry() == ptc2.Expiry() &&
       ptc.Source() == ptc2.Source() ;
}

void Protection::setType(const QString & type)
{
    d->type = type;
}

QString Protection::Type() const
{
    return d->type;
}

void Protection::setLevel(const QString & level)
{
    d->level = level;
}

QString Protection::Level() const
{
    return d->level;
}

void Protection::setExpiry(const QString & expiry)
{
    d->expiry = expiry;
}

QString Protection::Expiry() const
{
    return d->expiry;
}

void Protection::setSource(const QString & source)
{
    d->source = source;
}

QString Protection::Source() const
{
    return d->source;
}
