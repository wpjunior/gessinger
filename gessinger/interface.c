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

G_DEFINE_TYPE (GessingerInterface, gessinger_interface, G_TYPE_OBJECT);

static void
gessinger_interface_class_init (GessingerInterfaceClass *klass)
{
}

static void
gessinger_interface_init (GessingerInterface *self)
{
  self = GESSINGER_INTERFACE (self);
}

GessingerInterface *
gessinger_interface_new (void)
{
  GessingerInterface *obj;
  obj = g_object_new (GESSINGER_INTERFACE_TYPE, NULL);
  return obj;
}
