all:
	gcc -Wl,--export-dynamic  `pkg-config --libs --cflags gtk+-2.0 libxml-2.0` -I. main.c gessinger/jscontrol.c gessinger/gui.c gessinger/xmlconfig.c gessinger/midi.c gessinger/preset.c gessinger/interface.c -o main
