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
  gtk_builder_connect_signals(self->builder, self);
}

void gessinger_gui_show (GessingerGui *self)
{
  gtk_widget_show_all (self->window);
  gessinger_gui_draw_keyboard(self);
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

GessingerGui *
gessinger_gui_new (void)
{
  GessingerGui *obj;
  obj = g_object_new (GESSINGER_GUI_TYPE, NULL);
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
