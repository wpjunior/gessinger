all:
	gcc -g -Wl,--export-dynamic  `pkg-config --libs --cflags fluidsynth gtk+-2.0 libxml-2.0` -I. main.c gessinger/jscontrol.c gessinger/gui.c gessinger/presets_config.c gessinger/midi.c gessinger/preset.c gessinger/interface.c gessinger/font.c -o main
