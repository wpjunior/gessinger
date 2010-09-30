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

#ifndef _GESSINGER_INTERFACE_H
#define _GESSINGER_INTERFACE_H

#define _GNU_SOURCE
#include <glib.h>
#include <glib-object.h>
#include <fluidsynth.h>
#include <gessinger/xmlconfig.h>
#include <gessinger/jscontrol.h>
#include <gessinger/preset.h>

G_BEGIN_DECLS
#define GESSINGER_INTERFACE_TYPE                  (gessinger_interface_get_type ())
#define GESSINGER_INTERFACE(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GESSINGER_INTERFACE_TYPE, GessingerInterface))
#define GESSINGER_INTERFACE_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GESSINGER_INTERFACE_TYPE, GessingerInterfaceClass))
#define IS_GESSINGER_INTERFACE(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GESSINGER_INTERFACE_TYPE))
#define IS_GESSINGER_INTERFACE_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GESSINGER_INTERFACE_TYPE))
#define GESSINGER_INTERFACE_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GESSINGER_INTERFACE_TYPE, GessingerInterfaceClass))

typedef struct _GessingerInterface GessingerInterface;
typedef struct _GessingerInterfaceClass GessingerInterfaceClass;

struct _GessingerInterface
{
  /*< private >*/
  GObject parent;
  GessingerXmlconfig *config;
  GessingerJscontrol *js_control;
  GessingerPreset *active_preset;
  fluid_settings_t *f_settings;
  fluid_synth_t *f_synth;
  fluid_audio_driver_t *f_adriver;
  GessingerPresetKey *grabed_key;
};

struct _GessingerInterfaceClass
{
  /*< private >*/
  GObjectClass parent;
};

GType gessinger_interface_get_type (void) G_GNUC_CONST;
GessingerInterface *gessinger_interface_new (GessingerXmlconfig *config,
					     GessingerJscontrol *jscontrol);

gboolean gessinger_interface_set_preset (GessingerInterface *self,
					 GessingerPreset *preset);

G_END_DECLS

#endif /* _GESSINGER_INTERFACE_H */
