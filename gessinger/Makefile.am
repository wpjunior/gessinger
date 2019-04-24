
AUTOMAKE_OPTIONS = 1.9

AM_CFLAGS =				\
	$(WARN_CFLAGS)

MAINTAINERCLEANFILES =			\
	Makefile.in

EXTRA_DIST =				\
	font.c \
	font.h \
	gui.c \
	gui.h \
	interface.c \
	interface.h \
	jscontrol.c \
	jscontrol.h \
	main.c \
	midi.c \
	midi.h \
	preset.c \
	preset.h \
	presets_config.c \
	presets_config.h

bin_PROGRAMS =					\
	gessinger

gessinger_CPPFLAGS =			\
	-DDATADIR=\""$(datadir)"\"			\
	-I../ \
	$(GESSINGER_CFLAGS)

gessinger_LDFLAGS = \
	$(GESSINGER_CFLAGS)	\
	-Wl, -export-dynamic

gessinger_SOURCES =			\
	font.c \
	gui.c \
	interface.c \
	jscontrol.c \
	main.c \
	midi.c \
	preset.c \
	presets_config.c

gessinger_LDADD =				\
	$(GESSINGER_LIBS)


CLEANFILES =					\
	$(server_in_files)			\
	$(server_DATA)
