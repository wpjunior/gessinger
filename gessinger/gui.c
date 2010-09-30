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

static void
gessinger_gui_class_init (GessingerGuiClass *klass)
{
}

static void
gessinger_gui_init (GessingerGui *self)
{

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
  gtk_builder_connect_signals(self->builder, self);
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
  gtk_list_store_append (self->liststore, &iter);
  gtk_list_store_set (self->liststore, &iter,
		      GESSINGER_COLUMN_PRESET, preset,
		      GESSINGER_COLUMN_PRESET_NAME, preset->name,
		      -1);
  g_print ("%s\n", preset->name);
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
  float f;
  f = fluid_synth_get_gain(self->interface->f_synth);
  g_print ("loading configs %f\n", f);
}

GessingerGui *
gessinger_gui_new (GessingerInterface *interface)
{
  GessingerGui *obj;
  obj = g_object_new (GESSINGER_GUI_TYPE, NULL);
  obj->interface = interface;

  /* Load Presets */
  if (obj->interface->config->list_presets!=NULL)
    g_list_foreach(g_list_first(obj->interface->config->list_presets),
		   (GFunc) gessinger_gui_load_preset,
		   obj);

  gessinger_gui_load_configs(obj);

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
  if ((preset!=NULL)&&(gessinger_interface_set_preset(self->interface, preset))) {
    gtk_label_set_text (GTK_LABEL(gtk_builder_get_object(self->builder, "active_preset")),
			preset->name);
  }
}
