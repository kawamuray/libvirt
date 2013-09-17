## Process this file with automake to produce Makefile.in

# Copyright (C) 2013 Yuto KAWAMURA(kawamuray) <kawamuray.dadada@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# Author: Yuto KAWAMURA(kawamuray)
if WITH_WIRESHARK_DISSECTOR
SUBDIRS              = src

# I think wireshark plugin is special case that doesn't need to install *.la(libtool archive) files.
# Maybe each plugin functionality should correspond to single file in plugins directory.
# So this hook keeps plugins directory clean.
install-exec-hook:
	rm -f $(ws_plugindir)/libvirt.la
endif
