/**
 * Copyright (C) 2010 Wilson Júnior
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <glib.h>

typedef struct _GessingerPreset GessingerPreset;
typedef struct _GessingerPresetSource GessingerPresetSource;
typedef struct _GessingerPresetKey GessingerPresetKey;
typedef struct _GessingerPresetKeySource GessingerPresetKeySource;

struct _GessingerPreset
{
  gint id;
  gchar *name;
  gint mode;
  GHashTable *sources;
  GessingerPresetKey *keys;
  gint num_keys;
  gint auto_keys;
};

struct _GessingerPresetSource
{
  gint id;
  gint font;
  gint bank;
  gint preset;
};

struct _GessingerPresetKey
{
  gint id;
  GessingerPresetKeySource *sources;
  gint num_sources;
};

struct _GessingerPresetKeySource
{
  gint source_id;
  gint midi_code;
  gint vel;
};

enum {
  GESSINGER_PRESET_MODE_0,
  GESSINGER_PRESET_GRAB_MODE,
};

GessingerPreset *
gessinger_preset_new (void);

GessingerPresetSource *
gessinger_preset_source_new (void);

GessingerPresetKey *
gessinger_preset_key_new (void);

GessingerPresetKeySource *
gessinger_preset_key_source_new (void);
