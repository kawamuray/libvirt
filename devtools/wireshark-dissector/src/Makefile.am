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
ws_plugin_LTLIBRARIES     = libvirt.la
libvirt_la_SOURCES        = packet-libvirt.c plugin.c
libvirt_la_CPPFLAGS       = $(WS_DISSECTOR_CPPFLAGS)
libvirt_la_LDFLAGS        = -avoid-version

packet-libvirt.c: packet-libvirt.h libvirt/protocol.h

plugin.c: packet-libvirt.c
	$(srcdir)/../util/make-dissector-reg . plugin $<

libvirt/protocol.h: $(srcdir)/../util/genxdrstub.pl $(WS_DISSECTOR_PROTO_FILES)
	LIBVIRT_VERSION=$(LIBVIRT_VERSION) \
          $(PERL) $(srcdir)/../util/genxdrstub.pl $(WS_DISSECTOR_PROTO_FILES)

clean-local:
	-rm -rf plugin.c libvirt/*.h
