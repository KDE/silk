/*
 *   Copyright 2010 by de Vathaire Paolo <paolo.devathaire@gmail.com>
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

#include "setting.moc"

Setting::Setting(const Plasma::DataEngine::Data &, QWidget * parent)
    : QWidget(parent)
{
    setupUi(this);
//    for (Plasma::DataEngine::Data::const_iterator it = data.begin(); it != data.end(); ++it) {
//        comboBox->addItem(it.key(), it.value());
//    }
}
