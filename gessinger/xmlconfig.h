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

#ifndef _GESSINGER_XMLCONFIG_H
#define _GESSINGER_XMLCONFIG_H

#define _GNU_SOURCE
#include <glib.h>
#include <glib-object.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

G_BEGIN_DECLS
#define GESSINGER_XMLCONFIG_TYPE                  (gessinger_xmlconfig_get_type ())
#define GESSINGER_XMLCONFIG(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GESSINGER_XMLCONFIG_TYPE, GessingerXmlconfig))
#define GESSINGER_XMLCONFIG_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GESSINGER_XMLCONFIG_TYPE, GessingerXmlconfigClass))
#define IS_GESSINGER_XMLCONFIG(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GESSINGER_XMLCONFIG_TYPE))
#define IS_GESSINGER_XMLCONFIG_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GESSINGER_XMLCONFIG_TYPE))
#define GESSINGER_XMLCONFIG_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GESSINGER_XMLCONFIG_TYPE, GessingerXmlconfigClass))

typedef struct _GessingerXmlconfig GessingerXmlconfig;
typedef struct _GessingerXmlconfigClass GessingerXmlconfigClass;

struct _GessingerXmlconfig
{
  /*< private >*/
  GObject parent;
  gchar *config_file;
  xmlDoc *doc;
  xmlNode *root;
};

struct _GessingerXmlconfigClass
{
  /*< private >*/
  GObjectClass parent;
};

GType gessinger_xmlconfig_get_type (void) G_GNUC_CONST;
GessingerXmlconfig *gessinger_xmlconfig_new (gchar *config_file);

G_END_DECLS

#endif /* _GESSINGER_XMLCONFIG_H */
