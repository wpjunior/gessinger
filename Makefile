all:
	gcc -Wl,--export-dynamic  `pkg-config --libs --cflags gtk+-2.0` -I. main.c gessinger/jscontrol.c gessinger/gui.c -o main