#  Copyright (C) 2010 Wilson Pinto Júnior
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_INIT([Gessinger],[0.1],[http://gitorious.org/gessinger],[gessinger])
AM_INIT_AUTOMAKE([1.7])
AM_MAINTAINER_MODE

#GETTEXT_PACKAGE=openlh-server
#AC_SUBST([GETTEXT_PACKAGE])
#AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],["$GETTEXT_PACKAGE"],[Gettext package])
#AC_PROG_INTLTOOL([0.35.0])
#AM_GLIB_GNU_GETTEXT

AM_PROG_LIBTOOL

dnl gtk dependencie
GTK_REQUIRED=2.12.0
FLUID_REQUIRED=1.1.1

PKG_CHECK_MODULES(GESSINGER, [
    gtk+-2.0 >= $GTK_REQUIRED,
    fluidsynth >= $FLUID_REQUIRED,
    libxml-2.0 >= 2.7,
    gmodule-export-2.0
])

AC_SUBST(GESSINGER_CFLAGS)
AC_SUBST(GESSINGER_LIBS)

dnl output files
AC_CONFIG_FILES([
Makefile
gessinger/Makefile
data/Makefile
data/ui/Makefile
])

AC_OUTPUT

echo "
gessinger-$VERSION:

	prefix:                 ${prefix}

"
