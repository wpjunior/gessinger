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
#include "gessinger/preset.h"

GessingerPreset *
gessinger_preset_new (void)
{
  GessingerPreset *obj;
  obj = g_malloc(sizeof(GessingerPreset));
  obj->name = NULL;
  obj->sources = g_hash_table_new_full (g_int_hash, g_int_equal, NULL, g_free);
  return obj;
}

GessingerPresetSource *
gessinger_preset_source_new (void)
{
  GessingerPresetSource *obj;
  obj = g_malloc(sizeof(GessingerPresetSource));
  return obj;
}

GessingerPresetKey *
gessinger_preset_key_new (void)
{
  GessingerPresetKey *obj;
  obj = g_malloc(sizeof(GessingerPresetKey));
  return obj;
}

GessingerPresetKeySource *
gessinger_preset_key_source_new (void)
{
  GessingerPresetKeySource *obj;
  obj = g_malloc(sizeof(GessingerPresetKeySource));
  return obj;
}

