/*
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
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

#ifndef MEDIAWIKI_EXPORT_H
#define MEDIAWIKI_EXPORT_H

#include <kdemacros.h>

#ifndef MEDIAWIKI_EXPORT
#  if defined(MEDIAWIKI_STATIC_LIBS)
#    define MEDIAWIKI_EXPORT
#  elif defined(MAKE_MEDIAWIKI_LIB)
#    define MEDIAWIKI_EXPORT KDE_EXPORT
#  else
#    define MEDIAWIKI_EXPORT KDE_IMPORT
#  endif
#endif

#endif