/*
 *   Copyright 2009 by Paolo de Vathaire <paolo.devathaire@gmail.com>
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

#include <QtNetwork/QNetworkReply>

#include "mediawiki.h"
#include "job_p.h"

#include "job.h"

using namespace mediawiki;

Job::Job(JobPrivate & dd, QObject * parent)
    : KJob(parent)
    , d_ptr(&dd)
{
    setCapabilities(Job::Killable);
}

Job::~Job()
{
    delete d_ptr;
}

bool Job::doKill()
{
    Q_D(Job);
    if (d->reply != 0) {
        d->reply->abort();
    }
    return true;
}
