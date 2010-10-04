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

#ifndef _GESSINGER_GUI_H
#define _GESSINGER_GUI_H

#define _GNU_SOURCE
#include <glib.h>
#include <gtk/gtk.h>
#include <glib-object.h>
#include "gessinger/presets_config.h"
#include "gessinger/interface.h"

G_BEGIN_DECLS
#define GESSINGER_GUI_TYPE                  (gessinger_gui_get_type ())
#define GESSINGER_GUI(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GESSINGER_GUI_TYPE, GessingerGui))
#define GESSINGER_GUI_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GESSINGER_GUI_TYPE, GessingerGuiClass))
#define IS_GESSINGER_GUI(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GESSINGER_GUI_TYPE))
#define IS_GESSINGER_GUI_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GESSINGER_GUI_TYPE))
#define GESSINGER_GUI_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GESSINGER_GUI_TYPE, GessingerGuiClass))

typedef struct _GessingerGui GessingerGui;
typedef struct _GessingerGuiClass GessingerGuiClass;

struct _GessingerGui
{
  /*< private >*/
  GObject parent;
  GtkBuilder *builder;
  GtkWidget *window;
  GtkWidget *keyboard;
  GtkWidget *treeview;
  GessingerInterface *interface;
  GtkListStore *liststore;
  GtkTreeViewColumn *column;
};

struct _GessingerGuiClass
{
  /*< private >*/
  GObjectClass parent;
};

GType gessinger_gui_get_type (void) G_GNUC_CONST;
GessingerGui *gessinger_gui_new (GessingerInterface *interface);
void gessinger_gui_show (GessingerGui *self);

G_END_DECLS

#endif /* _GESSINGER_GUI_H */
