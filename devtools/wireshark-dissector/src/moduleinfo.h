/* moduleinfo.h --- Define constants about wireshark plugin module
 *
 * Copyright (C) 2013 Yuto KAWAMURA(kawamuray) <kawamuray.dadada@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Yuto KAWAMURA(kawamuray) <kawamuray.dadada gmail.com>
 */

/* Included *after* config.h, in order to re-define these macros */

#ifdef PACKAGE
#undef PACKAGE
#endif

/* Name of package */
#define PACKAGE "libvirt"


#ifdef VERSION
#undef VERSION
#endif

/* Version number of package */
#define VERSION "0.0.1"
