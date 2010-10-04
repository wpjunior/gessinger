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
  int r;
  r = fluid_synth_sfload(self->f_synth,
			 font->file,
			 0); //TODO: save fid
  if (r<1) {
    g_warning ("Failed to open %s soundfont", font->file);
    return;
  }

  font->loaded =1;
  font->load_id = r;
}

static void
gessinger_interface_setup_fluidsynth(GessingerInterface *self)
{
  int i, t;
  double d1, d2, d3, d4;

  self->f_settings = new_fluid_settings ();
  fluid_settings_setint (self->f_settings, "synth.midi-channels", 256);
  self->f_synth = new_fluid_synth (self->f_settings);

  if (!self->f_synth) g_error ("Failed to create fluidsynth instance");

  fluid_settings_setstr(self->f_settings, "audio.driver", "alsa");
  self->f_adriver = new_fluid_audio_driver(self->f_settings, self->f_synth);

  if (self->presets_config->list_fonts!=NULL)
    g_list_foreach (self->presets_config->list_fonts,
		    (GFunc) gessinger_interface_load_font,
		    self);

  /* load saved gain */
  d1 = 100.0f; //default gain

  if (g_key_file_has_key (self->main_settings, "general", "gain", NULL))
    d1 = g_key_file_get_double (self->main_settings, "general", "gain", NULL);

  fluid_synth_set_gain(self->f_synth, d1/100);

  /* load active preset id */
  if (g_key_file_has_key (self->main_settings, "general", "active_preset", NULL))
    self->active_preset_id = g_key_file_get_integer (self->main_settings,
						     "general", "active_preset", NULL);

  /* load reverb settings */
  if (g_key_file_has_key (self->main_settings, "reverb", "active", NULL)) {
    i = g_key_file_get_boolean (self->main_settings, "reverb", "active", NULL);

    fluid_settings_setint (self->f_settings, "synth.reverb.active", i);
    fluid_synth_set_reverb_on (self->f_synth, i);
  }
  
  /* default reverb values */
  d1 = FLUID_REVERB_DEFAULT_ROOMSIZE;
  d2 = FLUID_REVERB_DEFAULT_DAMP;
  d3 = FLUID_REVERB_DEFAULT_WIDTH;
  d4 = FLUID_REVERB_DEFAULT_LEVEL;

  if (g_key_file_has_key (self->main_settings, "reverb", "room", NULL))
    d1 = g_key_file_get_double (self->main_settings, "reverb", "room", NULL);

  if (g_key_file_has_key (self->main_settings, "reverb", "damp", NULL))
    d2 = g_key_file_get_double (self->main_settings, "reverb", "damp", NULL);

  if (g_key_file_has_key (self->main_settings, "reverb", "width", NULL))
    d3 = g_key_file_get_double (self->main_settings, "reverb", "width", NULL);

  if (g_key_file_has_key (self->main_settings, "reverb", "level", NULL))
    d4 = g_key_file_get_double (self->main_settings, "reverb", "level", NULL);

  fluid_synth_set_reverb (self->f_synth, d1, d2, d3, d4); //setup reverb


  /* load chorus settings */
  if (g_key_file_has_key (self->main_settings, "chorus", "active", NULL)) {
    i = g_key_file_get_boolean (self->main_settings, "chorus", "active", NULL);

    fluid_settings_setint (self->f_settings, "synth.chorus.active", i);
    fluid_synth_set_reverb_on (self->f_synth, i);
  }


  /* default chorus values */
  i = FLUID_CHORUS_DEFAULT_N;
  d1 = FLUID_CHORUS_DEFAULT_LEVEL;
  d2 = FLUID_CHORUS_DEFAULT_SPEED;
  d3 = FLUID_CHORUS_DEFAULT_DEPTH;
  t = FLUID_CHORUS_DEFAULT_TYPE;

  if (g_key_file_has_key (self->main_settings, "chorus", "n", NULL))
    i = g_key_file_get_integer (self->main_settings, "chorus", "n", NULL);

  if (g_key_file_has_key (self->main_settings, "chorus", "level", NULL))
    d1 = g_key_file_get_double (self->main_settings, "chorus", "level", NULL);

  if (g_key_file_has_key (self->main_settings, "chorus", "speed", NULL))
    d2 = g_key_file_get_double (self->main_settings, "chorus", "speed", NULL);

  if (g_key_file_has_key (self->main_settings, "chorus", "depth", NULL))
    d3 = g_key_file_get_double (self->main_settings, "chorus", "depth", NULL);

  if (g_key_file_has_key (self->main_settings, "chorus", "mode", NULL))
    t = g_key_file_get_integer (self->main_settings, "chorus", "mode", NULL);

  fluid_synth_set_chorus (self->f_synth, i, d1, d2, d3, t); //setup chorus
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

  for (i=0; i<pkey->num_sources; i++) {

    if (state)
      fluid_synth_noteon (self->f_synth,
			  pkey->sources[i].source_id,
			  pkey->sources[i].midi_code,
			  pkey->sources[i].vel);

    else if (preset->mode!=GESSINGER_PRESET_GRAB_MODE) //Dont stop note in leave in grab_mode
      fluid_synth_noteoff (self->f_synth,
			   pkey->sources[i].source_id,
			   pkey->sources[i].midi_code);
  }
}

GessingerInterface *
gessinger_interface_new (GessingerPresetsConfig *presets_config,
			 GessingerJscontrol *js_control)
{
  GessingerInterface *self;
  GError *error = NULL;

  self = g_object_new (GESSINGER_INTERFACE_TYPE, NULL);

  self->presets_config = presets_config;
  self->js_control = js_control;
  self->active_preset = NULL;
  self->grabed_key = NULL;
  self->main_settings = g_key_file_new ();

  if (!g_key_file_load_from_file (self->main_settings, "settings.conf",
				  0, &error)) {
    g_debug ("failed to read settings.conf: %s", error->message);
  }

  self->js_control->button_callback = (GessingerButtonCallback) gessinger_interface_js_button_callback;
  self->js_control->axis_callback = (GessingerAxisCallback) gessinger_interface_js_axis_callback;
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
					 GessingerPreset    *preset,
					 gboolean            save_settings)
{
  /* Clear Grabed Notes */
  gessinger_interface_clear_grabed_notes(self);
  
  /* Clear previous source */
  if (self->active_preset)
    g_hash_table_foreach (self->active_preset->sources,
			  (GHFunc) gessinger_interface_clear_source,
			  self);

  self->active_preset = preset;

  if (save_settings)
    g_key_file_set_integer(self->main_settings, "general", "active_preset",
			   self->active_preset->id);

  g_hash_table_foreach (preset->sources,
			(GHFunc) gessinger_interface_load_source,
			self);
  return TRUE;
}


void gessinger_interface_save_configs(GessingerInterface *self)
{
  GError *error = NULL;
  gsize lenght;
  gchar *content;
  content = g_key_file_to_data(self->main_settings, &lenght, &error);

  if (error) {
    g_warning ("Failed to render GKeyFile: %s", error->message);
    g_error_free(error);
    return;
  }

  if (!g_file_set_contents("settings.conf", content,
			   lenght, &error)) {
    g_warning ("Failed to write settings.conf: %s", error->message);
    g_error_free(error);
  }
}

