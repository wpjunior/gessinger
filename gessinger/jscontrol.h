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

#ifndef _GESSINGER_JSCONTROL_H
#define _GESSINGER_JSCONTROL_H

#define _GNU_SOURCE
#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS
#define GESSINGER_JSCONTROL_TYPE                  (gessinger_jscontrol_get_type ())
#define GESSINGER_JSCONTROL(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GESSINGER_JSCONTROL_TYPE, GessingerJscontrol))
#define GESSINGER_JSCONTROL_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GESSINGER_JSCONTROL_TYPE, GessingerJscontrolClass))
#define IS_GESSINGER_JSCONTROL(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GESSINGER_JSCONTROL_TYPE))
#define IS_GESSINGER_JSCONTROL_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GESSINGER_JSCONTROL_TYPE))
#define GESSINGER_JSCONTROL_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GESSINGER_JSCONTROL_TYPE, GessingerJscontrolClass))
#define GESSINGER_JSCONTROL_ERROR gessinger_jscontrol_error_quark()
GQuark gessinger_jscontrol_error_quark (void);

typedef struct _GessingerJscontrol GessingerJscontrol;
typedef struct _GessingerJscontrolClass GessingerJscontrolClass;

enum {
  GESSINGER_JSCONTROL_PROP_0=0,
  GESSINGER_JSCONTROL_PROP_FD,
  GESSINGER_JSCONTROL_PROP_NAXIS,
  GESSINGER_JSCONTROL_PROP_NBUTTONS
};



struct _GessingerJscontrol
{
  /*< private >*/
  GObject parent;
  gint jsfd;
  gint numaxes;
  gint numbuttons;
  gchar *name_js;
  gchar *jsdev;
  GIOChannel *channel_io;
  gint event_io;
  gint *buttons;
  gint *axes;

  void (*button_callback) (gint button,
			   gint state,
			   gpointer user_data);

  void (*axis_callback) (gint axis_id,
			 gint value,
			 gpointer user_data);

  gpointer button_callback_data;
  gpointer axis_callback_data;
};

struct _GessingerJscontrolClass
{
  /*< private >*/
  GObjectClass parent;
};

GType gessinger_jscontrol_get_type (void) G_GNUC_CONST;
GessingerJscontrol *gessinger_jscontrol_new (char *dev, GError **error);

G_END_DECLS

#endif /* _GESSINGER_JSCONTROL_H */
