/**
 * Copyright (C) 2010 Wilson Pinto Júnior <wilsonpjunior@gmail.com>
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
#include <gtk/gtk.h>
#include <gessinger/gui.h>

enum {
  GESSINGER_COLUMN_PRESET,
  GESSINGER_COLUMN_PRESET_NAME
};

G_DEFINE_TYPE (GessingerGui, gessinger_gui, G_TYPE_OBJECT);

void gessinger_gui_draw_keyboard(GessingerGui *self);
void gessinger_gui_next_preset_cb(GessingerGui *self);
void gessinger_gui_prev_preset_cb(GessingerGui *self);

static void
gessinger_gui_class_init (GessingerGuiClass *klass)
{
}

static void
gessinger_gui_init (GessingerGui *self)
{

  GObject *obj;

  self->builder = gtk_builder_new ();

  gtk_builder_add_from_file(self->builder,
			    "gessinger.ui",
			    NULL);

  self->window = GTK_WIDGET(gtk_builder_get_object(self->builder,
						   "window"));
  self->keyboard = GTK_WIDGET(gtk_builder_get_object(self->builder,
						     "keyboard"));
  self->liststore = gtk_list_store_new (2, G_TYPE_POINTER, G_TYPE_STRING);
  self->treeview = GTK_WIDGET (gtk_builder_get_object(self->builder,
						      "treeview"));

  gtk_tree_view_set_model(GTK_TREE_VIEW(self->treeview),
			  GTK_TREE_MODEL(self->liststore));
}

void gessinger_gui_show (GessingerGui *self)
{
  gtk_widget_show_all (self->window);
  gessinger_gui_draw_keyboard(self);
}

void gessinger_gui_load_preset (GessingerPreset *preset,
				GessingerGui    *self)
{
  GtkTreeIter iter;
  GtkTreePath *path;
  gtk_list_store_append (self->liststore, &iter);
  gtk_list_store_set (self->liststore, &iter,
		      GESSINGER_COLUMN_PRESET, preset,
		      GESSINGER_COLUMN_PRESET_NAME, preset->name,
		      -1);

  if (preset->id == self->interface->active_preset_id) {
    path = gtk_tree_model_get_path (GTK_TREE_MODEL(self->liststore), &iter);

    if (gessinger_interface_set_preset(self->interface, preset, FALSE)) {
      gtk_tree_view_set_cursor (GTK_TREE_VIEW(self->treeview), path, NULL, FALSE);
      gtk_label_set_text (GTK_LABEL(gtk_builder_get_object(self->builder, "active_preset")),
			  preset->name);
    }
  }
}

void gessinger_gui_new_clicked (GtkWidget *widget, GessingerGui *self)
{
  g_print ("new clicked\n");
}

void gessinger_gui_show_about (GtkWidget *widget, GessingerGui *self)
{
  GtkWidget *about_dialog;
  about_dialog = GTK_WIDGET(gtk_builder_get_object(self->builder,
						   "aboutdialog"));
  gtk_dialog_run(GTK_DIALOG(about_dialog));
  gtk_widget_hide(GTK_WIDGET(about_dialog));
}

static void
gessinger_gui_load_configs (GessingerGui *self)
{
  GObject *obj;
  float f;
  int i, r;
  double d;
  /* Gain */
  f = fluid_synth_get_gain(self->interface->f_synth);
  obj = gtk_builder_get_object(self->builder, "gain");
  gtk_adjustment_set_value (GTK_ADJUSTMENT(obj), f*100);

  /* Reverb */
  r = fluid_settings_getint(self->interface->f_settings,
			    "synth.reverb.active",
			    &i);
  if (r) {
    obj = gtk_builder_get_object(self->builder, "reverb_table");
    gtk_widget_set_sensitive(GTK_WIDGET(obj), i);

    obj = gtk_builder_get_object(self->builder, "reverb_button");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(obj), i);
  }

  d = fluid_synth_get_reverb_roomsize(self->interface->f_synth);
  obj = gtk_builder_get_object(self->builder, "reverb_room");
  gtk_adjustment_set_value(GTK_ADJUSTMENT(obj), d);

  d = fluid_synth_get_reverb_damp(self->interface->f_synth);
  obj = gtk_builder_get_object(self->builder, "reverb_damp");
  gtk_adjustment_set_value(GTK_ADJUSTMENT(obj), d);

  d = fluid_synth_get_reverb_level(self->interface->f_synth);
  obj = gtk_builder_get_object(self->builder, "reverb_level");
  gtk_adjustment_set_value(GTK_ADJUSTMENT(obj), d);

  d = fluid_synth_get_reverb_width(self->interface->f_synth);
  obj = gtk_builder_get_object(self->builder, "reverb_width");
  gtk_adjustment_set_value(GTK_ADJUSTMENT(obj), d);

  /* Chorus */
  r = fluid_settings_getint(self->interface->f_settings,
			    "synth.chorus.active",
			    &i);
  if (r) {
    obj = gtk_builder_get_object(self->builder, "chorus_table");
    gtk_widget_set_sensitive(GTK_WIDGET(obj), i);

    obj = gtk_builder_get_object(self->builder, "chorus_button");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(obj), i);
  }

  d = fluid_synth_get_chorus_nr(self->interface->f_synth);
  obj = gtk_builder_get_object(self->builder, "chorus_n");
  gtk_adjustment_set_value(GTK_ADJUSTMENT(obj), d);

  d = fluid_synth_get_chorus_level(self->interface->f_synth);
  obj = gtk_builder_get_object(self->builder, "chorus_level");
  gtk_adjustment_set_value(GTK_ADJUSTMENT(obj), d);

  d = fluid_synth_get_chorus_speed_Hz(self->interface->f_synth);
  obj = gtk_builder_get_object(self->builder, "chorus_speed");
  gtk_adjustment_set_value(GTK_ADJUSTMENT(obj), d);

  d = fluid_synth_get_chorus_depth_ms(self->interface->f_synth);
  obj = gtk_builder_get_object(self->builder, "chorus_depth");
  gtk_adjustment_set_value(GTK_ADJUSTMENT(obj), d);

  i = fluid_synth_get_chorus_type(self->interface->f_synth);
  if (i==FLUID_CHORUS_MOD_TRIANGLE) {
    obj = gtk_builder_get_object(self->builder, "chorus_triangle");
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(obj), TRUE);
  }
}

GessingerGui *
gessinger_gui_new (GessingerInterface *interface)
{
  GessingerGui *obj;
  obj = g_object_new (GESSINGER_GUI_TYPE, NULL);
  obj->interface = interface;

  /* Next and Prev callbacks from interface */
  interface->next_preset_callback = gessinger_gui_next_preset_cb;
  interface->prev_preset_callback = gessinger_gui_prev_preset_cb;

  interface->next_preset_callback_data = obj;
  interface->prev_preset_callback_data = obj;

  /* Load Presets */
  if (interface->presets_config->list_presets!=NULL)
    g_list_foreach(g_list_first(interface->presets_config->list_presets),
		   (GFunc) gessinger_gui_load_preset,
		   obj);

  gessinger_gui_load_configs(obj);

  /* connecting signals */
  gtk_builder_connect_signals(obj->builder, obj);
  return obj;
}

void gessinger_gui_draw_keyboard(GessingerGui *self)
{

}

void gessinger_gui_add_preset (GtkWidget *widget, GessingerGui *self)
{
  g_print ("add_preset\n");
}

void gessinger_gui_del_preset (GtkWidget *widget, GessingerGui *self)
{
  g_print ("del_preset\n");
}

void gessinger_gui_edit_preset (GtkWidget *widget, GessingerGui *self)
{
  g_print ("edit_preset\n");
}

void treeview_row_activated_cb (GtkTreeView       *tree_view,
				GtkTreePath       *path,
				GtkTreeViewColumn *column,
				GessingerGui      *self)
{
  GessingerPreset *preset;
  GtkTreeIter iter;
  if (!gtk_tree_model_get_iter (GTK_TREE_MODEL(self->liststore), &iter, path)) return;
  gtk_tree_model_get (GTK_TREE_MODEL(self->liststore), &iter,
		      GESSINGER_COLUMN_PRESET, &preset,
		      -1);
  if ((preset!=NULL)&&(gessinger_interface_set_preset(self->interface, preset, TRUE))) {
    gtk_label_set_text (GTK_LABEL(gtk_builder_get_object(self->builder, "active_preset")),
			preset->name);
  }
}

void gain_value_changed_cb(GtkAdjustment *adjustment,
			   GessingerGui *self)
{
  float f;
  f = gtk_adjustment_get_value(adjustment);
  fluid_synth_set_gain(self->interface->f_synth, f/100);
  g_key_file_set_double (self->interface->main_settings,
			 "general", "gain", f);
}

void reverb_button_toggled_cb(GtkToggleButton *button,
			      GessingerGui    *self)
{
  int i;
  GObject *obj;

  i = gtk_toggle_button_get_active(button);

  fluid_settings_setint (self->interface->f_settings,
			 "synth.reverb.active", i);

  g_key_file_set_boolean(self->interface->main_settings,
			 "reverb", "active", i);
  fluid_synth_set_reverb_on (self->interface->f_synth, i);
  obj = gtk_builder_get_object(self->builder, "reverb_table");
  gtk_widget_set_sensitive(GTK_WIDGET(obj), i);
}
void chorus_button_toggled_cb(GtkToggleButton *button,
			      GessingerGui    *self)
{
  int i;
  GObject *obj;

  i = gtk_toggle_button_get_active(button);

  fluid_settings_setint (self->interface->f_settings,
			 "synth.chorus.active", i);

  g_key_file_set_boolean(self->interface->main_settings,
			 "chorus", "active", i);
  fluid_synth_set_chorus_on (self->interface->f_synth, i);
  obj = gtk_builder_get_object(self->builder, "chorus_table");
  gtk_widget_set_sensitive(GTK_WIDGET(obj), i);
}

void on_reverb_value_changed(GtkWidget    *widget,
			     GessingerGui *self)
{
  int r;
  double room, damp, level, width;
  GObject *obj;

  obj = gtk_builder_get_object(self->builder, "reverb_room");
  room = gtk_adjustment_get_value(GTK_ADJUSTMENT(obj));

  obj = gtk_builder_get_object(self->builder, "reverb_damp");
  damp = gtk_adjustment_get_value(GTK_ADJUSTMENT(obj));

  obj = gtk_builder_get_object(self->builder, "reverb_level");
  level = gtk_adjustment_get_value(GTK_ADJUSTMENT(obj));

  obj = gtk_builder_get_object(self->builder, "reverb_width");
  width = gtk_adjustment_get_value(GTK_ADJUSTMENT(obj));

  /* Save too in config file */
  g_key_file_set_double (self->interface->main_settings, "reverb", "room", room);
  g_key_file_set_double (self->interface->main_settings, "reverb", "damp", damp);
  g_key_file_set_double (self->interface->main_settings, "reverb", "level", level);
  g_key_file_set_double (self->interface->main_settings, "reverb", "width", width);

  fluid_synth_set_reverb(self->interface->f_synth, room,
			     damp, width, level);
}
void on_chorus_value_changed(GtkWidget    *widget,
			     GessingerGui *self)
{
  int type, n;
  double level, speed, depth;
  GObject *obj;

  obj = gtk_builder_get_object(self->builder, "chorus_n");
  n = gtk_adjustment_get_value(GTK_ADJUSTMENT(obj));

  obj = gtk_builder_get_object(self->builder, "chorus_level");
  level = gtk_adjustment_get_value(GTK_ADJUSTMENT(obj));

  obj = gtk_builder_get_object(self->builder, "chorus_speed");
  speed = gtk_adjustment_get_value(GTK_ADJUSTMENT(obj));

  obj = gtk_builder_get_object(self->builder, "chorus_depth");
  depth = gtk_adjustment_get_value(GTK_ADJUSTMENT(obj));

  obj = gtk_builder_get_object(self->builder, "chorus_triangle");
  type = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj));

  g_key_file_set_integer (self->interface->main_settings, "chorus", "n", n);
  g_key_file_set_double (self->interface->main_settings, "chorus", "level", level);
  g_key_file_set_double (self->interface->main_settings, "chorus", "speed", speed);
  g_key_file_set_double (self->interface->main_settings, "chorus", "n", depth);
  g_key_file_set_integer (self->interface->main_settings, "chorus", "mode", type);

  fluid_synth_set_chorus(self->interface->f_synth, n, level,
			 speed, depth, type);
}

void gessinger_gui_next_preset_cb (GessingerGui *self)
{
  GtkTreePath *path = NULL;
  GtkTreeIter iter;

  gtk_tree_view_get_cursor (GTK_TREE_VIEW (self->treeview), &path, NULL);

  if (path==NULL) return;
  if (!gtk_tree_model_get_iter(GTK_TREE_MODEL(self->liststore), &iter, path)) return;
  if (!gtk_tree_model_iter_next ((GtkTreeModel *) self->liststore, &iter)) return;

  gtk_tree_path_next (path); //set next path
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(self->treeview), path, NULL, FALSE);
  gtk_tree_view_row_activated (GTK_TREE_VIEW (self->treeview), path, NULL);
}

void gessinger_gui_prev_preset_cb(GessingerGui *self)
{
  GtkTreePath *path = NULL;

  gtk_tree_view_get_cursor (GTK_TREE_VIEW (self->treeview), &path, NULL);

  if (path==NULL) return;

  gtk_tree_path_prev (path); //set prev
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(self->treeview), path, NULL, FALSE);
  gtk_tree_view_row_activated (GTK_TREE_VIEW (self->treeview), path, NULL);
}

gboolean gessinger_quit (GtkWidget *widget,
			 GdkEvent  *event,
			 GessingerGui *self)
{
  gessinger_interface_save_configs(self->interface);
  gtk_main_quit();

  return FALSE;
}
