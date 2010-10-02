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
#include "gessinger/interface.h"
#include "gessinger/font.h"

G_DEFINE_TYPE (GessingerInterface, gessinger_interface, G_TYPE_OBJECT);

static void
gessinger_interface_class_init (GessingerInterfaceClass *klass)
{
}

static void
gessinger_interface_load_font(GessingerFont *font,
			      GessingerInterface *self)
{
  fluid_synth_sfload(self->f_synth,
		     font->file,
		     0); //TODO: save fid
}

static void
gessinger_interface_setup_fluidsynth(GessingerInterface *self)
{
  self->f_settings = new_fluid_settings ();
  fluid_settings_setint (self->f_settings, "synth.midi-channels", 256);
  fluid_settings_setint (self->f_settings, "synth.gain", 10);
  self->f_synth = new_fluid_synth (self->f_settings);

  if (!self->f_synth) g_error ("Failed to create fluidsynth instance");

  fluid_settings_setstr(self->f_settings, "audio.driver", "alsa");
  self->f_adriver = new_fluid_audio_driver(self->f_settings, self->f_synth);

  if (self->config->list_fonts!=NULL)
    g_list_foreach (self->config->list_fonts,
		    (GFunc) gessinger_interface_load_font,
		    self);
}

static void
gessinger_interface_init (GessingerInterface *self)
{
  self = GESSINGER_INTERFACE (self);
}

void
gessinger_interface_clear_grabed_notes(GessingerInterface *self)
{
  gint i;

  if (self->grabed_key!=NULL) {
    for (i=0; i < self->grabed_key->num_sources; i++)
      fluid_synth_noteoff (self->f_synth,
			   self->grabed_key->sources[i].source_id,
			   self->grabed_key->sources[i].midi_code);
  }
}

static void gessinger_interface_js_axis_callback(gint axis_id,
						 gint value,
						 GessingerInterface *self)
{
  int i;

  if (value > 0)
    i = fluid_synth_pitch_bend (self->f_synth, 0, 16383);
  else
    i = fluid_synth_pitch_bend (self->f_synth, 0, 0);
  fluid_synth_cc (self->f_synth, 0, 100, 0);
  fluid_synth_cc (self->f_synth, 0, 101, 0);
  fluid_synth_cc (self->f_synth, 0, 10, 15);
  g_print ("axis %d -> %d = %d\n", axis_id, value, i);
}

static void gessinger_interface_js_button_callback(gint button,
						   gint state,
						   GessingerInterface *self)
{
  GessingerPresetKey *pkey;
  GessingerPreset *preset;
  gint i;

  preset = self->active_preset;

  if (preset==NULL) {
    g_warning("active preset == NULL");
    return;
  }

  if (button > preset->num_keys) {
    g_warning("num key is not found in preset");
    return;
  }

  pkey = &preset->keys[button];

  //turn off previous notes in grab_mode
  if ((preset->mode == GESSINGER_PRESET_GRAB_MODE)&&(state)) {
    gessinger_interface_clear_grabed_notes(self);
    self->grabed_key = pkey;
  }
  //g_print ("%d\n", pkey->id);
  for (i=0; i<pkey->num_sources; i++) {
    //g_print("on/off %d %d\n", pkey->sources[i].source_id, pkey->sources[i].midi_code);
    if (state)
      fluid_synth_noteon (self->f_synth,
			  pkey->sources[i].source_id,
			  pkey->sources[i].midi_code,
			  127); //TODO modify it

    else if (preset->mode!=GESSINGER_PRESET_GRAB_MODE) //Dont stop note in leave in grab_mode
      fluid_synth_noteoff (self->f_synth,
			   pkey->sources[i].source_id,
			   pkey->sources[i].midi_code);
  }
}

GessingerInterface *
gessinger_interface_new (GessingerXmlconfig *config,
			 GessingerJscontrol *js_control)
{
  GessingerInterface *self;
  self = g_object_new (GESSINGER_INTERFACE_TYPE, NULL);
  self->config = config;
  self->js_control = js_control;
  self->active_preset = NULL;
  self->grabed_key = NULL;
  
  self->js_control->button_callback = gessinger_interface_js_button_callback;
  self->js_control->axis_callback = gessinger_interface_js_axis_callback;
  self->js_control->button_callback_data = self;
  self->js_control->axis_callback_data = self;

  gessinger_interface_setup_fluidsynth(self);

  return self;
}

static void gessinger_interface_load_source (gint *id,
					     GessingerPresetSource *source,
					     GessingerInterface *self)
{
  gint r;
  r = fluid_synth_program_select (self->f_synth,
				  source->id,
				  source->font,
				  source->bank,
				  source->preset);

  fluid_synth_pitch_wheel_sens (self->f_synth,
				source->id,
				2);

  if (r!= FLUID_OK) g_warning ("Failed to set source to channel %d from"
			       " Font %d Bank %d and Preset %d",
			       source->id, source->font,
			       source->bank, source->preset);
}

static void gessinger_interface_clear_source (gint *id,
					     GessingerPresetSource *source,
					     GessingerInterface *self)
{
  gint r;
  r = fluid_synth_unset_program (self->f_synth, source->id);

  if (r!= FLUID_OK) g_warning ("Failed to unset source channel %d", source->id);
}

gboolean gessinger_interface_set_preset (GessingerInterface *self,
					 GessingerPreset *preset)
{
  /* Clear Grabed Notes */
  gessinger_interface_clear_grabed_notes(self);
  
  /* Clear previous source */
  if (self->active_preset)
    g_hash_table_foreach (self->active_preset->sources,
			  (GHFunc) gessinger_interface_clear_source,
			  self);

  self->active_preset = preset;
  g_hash_table_foreach (preset->sources,
			(GHFunc) gessinger_interface_load_source,
			self);
}
