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
#include "gessinger/midi.h"

GessingerPresetKeySource* gessinger_midi_source_new (gint n_button,
						     gint *notes_interval,
						     gint num_notes,
						     gint tone,
						     gint octave,
						     gint source)
{
  GessingerPresetKeySource *obj;
  gint i, o;
  obj = g_malloc0(sizeof(GessingerPresetKeySource) * num_notes);
  static int escale[7] = {0, 2, 4, 5, 7, 9, 11};

  for (i=0; i<num_notes; i++) {
    o = octave;
    obj[i].source_id = source;

    if (notes_interval[i]>=0) {
      obj[i].midi_code = ((octave+1)*12);
      g_print ("b: %d -> %d\n", n_button, obj[i].midi_code);
    }
  }

  return obj;
}
