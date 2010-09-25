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

static int escale[7] = {0, 2, 4, 5, 7, 9, 11};
//C: 0, D: 2, E: 4, F: 5, G: 7, A: 9, B: 11

gint is_normal_note (int n)
{
  if (n>12) n=n%12;
  return ((n==0)||(n==2)||(n==4)||(n==5)||(n==7)||(n==9)||(n==11));
}

gint get_note_pos(int n)
{
  int p;
  p = -1;
  for (p=0; p<=6; p++) if (escale[p]==n) break;
  return p;
}
GessingerPresetKeySource* gessinger_midi_source_new (gint n_button,
						     gint *notes_interval,
						     gint num_notes,
						     gint tone,
						     gint octave,
						     gint source)
{
  GessingerPresetKeySource *obj;
  gint i, o, n, p, r, is_accident;
  obj = g_malloc0(sizeof(GessingerPresetKeySource) * num_notes);
  
  is_accident = !is_normal_note(n_button);

  for (i=0; i<num_notes; i++) {
    o = octave;
    obj[i].source_id = source;
    
    if (!is_accident) {
      p = get_note_pos(n_button);
      r = p + notes_interval[i]-1;
      if (r>=7) o = o +(r%7);
      obj[i].midi_code = ((o+1)*12)+tone+n_button;
      g_print ("b: %d -> %d %d\n", n_button, obj[i].midi_code, r%7);
    }
  }

  return obj;
}
