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
#include <fcntl.h>
#include <linux/joystick.h>
#include "gessinger/jscontrol.h"

G_DEFINE_TYPE (GessingerJscontrol, gessinger_jscontrol, G_TYPE_OBJECT);

GQuark
gessinger_jscontrol_error_quark (void)
{
  static GQuark quark;
  if (!quark)
    quark = g_quark_from_static_string ("gessinger_jscontrol_error");

  return quark;
}

static void
gessinger_jscontrol_class_init (GessingerJscontrolClass *klass)
{

}

static void
gessinger_jscontrol_init (GessingerJscontrol *self)
{
  self = GESSINGER_JSCONTROL (self);
}

static int*
gessinger_jscontrol_do_io (GIOChannel *source,
			   GIOCondition condition,
			   GessingerJscontrol *self)
{
  static struct js_event js;
  read(self->jsfd, &js, sizeof(struct js_event));
  
  if (js.type==JS_EVENT_BUTTON)
    self->buttons[js.number] = js.value;

  else if (js.type==JS_EVENT_AXIS)
    self->axes[js.number] = js.value;

  int i;
  for (i=0; i<self->numbuttons; i++)
    printf ("%d", self->buttons[i]);
  printf (" ");
  for (i=0; i<self->numaxes; i++)
    printf ("%d|", self->axes[i]);
    printf ("\n");

}

static void
gessinger_jscontrol_load_config(GessingerJscontrol *self)
{

  if (self->jsfd==-1) return;

  if (ioctl(self->jsfd, JSIOCGAXES, &self->numaxes)) {
    g_warning  ("failed to get num of axes");
    self->numaxes = 0;
  }

  if (ioctl(self->jsfd, JSIOCGBUTTONS, &self->numbuttons)) {
    g_warning ("error getting buttons");
    self->numbuttons = 0;
  }

  if (ioctl(self->jsfd, JSIOCGNAME(80), self->name_js)) {
    self->name_js = g_strdup_printf("Unknown joystick at %s",
				    self->jsdev);
  }

  if (self->numbuttons)
    {
      self->buttons = (int *) g_malloc0 (self->numbuttons * sizeof(int));
    }
  if (self->numaxes)
    self->axes = (int *) g_malloc0 (self->numaxes * sizeof(int));

  self->channel_io = g_io_channel_unix_new (self->jsfd);
  self->event_io = g_io_add_watch_full (self->channel_io,
					G_PRIORITY_HIGH,
					G_IO_IN | G_IO_PRI | G_IO_HUP | G_IO_ERR,
					(GIOFunc) gessinger_jscontrol_do_io,
					self,
					NULL);
}

GessingerJscontrol *
gessinger_jscontrol_new (char *dev,
			 GError **error)
{
  GessingerJscontrol *obj;

  /* reading joystick */
  int jsfd = -1;
  jsfd = open(dev, O_RDONLY);

  if (jsfd==-1) {
    *error = g_error_new (GESSINGER_JSCONTROL_ERROR,
			 1, "failed to open %s joystick",
			 dev);
    return NULL;
  }
  obj = g_object_new (GESSINGER_JSCONTROL_TYPE,
		      NULL);
  obj->jsfd = jsfd;
  obj->jsdev = dev;
  gessinger_jscontrol_load_config(obj);
  return obj;
}
