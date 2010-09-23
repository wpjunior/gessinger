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
#include "gessinger/xmlconfig.h"
#include "gessinger/midi.h"

G_DEFINE_TYPE (GessingerXmlconfig, gessinger_xmlconfig, G_TYPE_OBJECT);

static void
gessinger_xmlconfig_class_init (GessingerXmlconfigClass *klass)
{
}

static void
gessinger_xmlconfig_init (GessingerXmlconfig *self)
{
  self = GESSINGER_XMLCONFIG (self);
  self->doc = NULL;
  self->root = NULL;
}

static void
gessinger_xmlconfig_setup_key(GessingerXmlconfig *self,
			      GessingerPresetKey *key,
			      xmlNode            *node)
{
  xmlNode *cur_node;
  xmlAttr *attr;
  gint i;

  key->num_sources = 0;

  /* conting number of midi codes */
  for (cur_node=node->children; cur_node!=NULL; cur_node=cur_node->next) {
    if (cur_node->type != XML_ELEMENT_NODE) continue;
    if (g_str_equal(cur_node->name, "midi")) key->num_sources++;
  }

  if (key->num_sources > 0) {
    i = 0;
    key->sources = g_malloc0(sizeof(GessingerPresetKeySource) * key->num_sources);

    for (cur_node=node->children; cur_node!=NULL; cur_node=cur_node->next) {
      if (cur_node->type != XML_ELEMENT_NODE) continue;
      if (g_str_equal(cur_node->name, "midi")) {

	for (attr=cur_node->properties; attr!=NULL; attr=attr->next) {
	  if ((attr->children==NULL)||(attr->children->content==NULL)) continue;
	  else if (g_str_equal(attr->name, "source")) key->sources[i].source_id = atoi(attr->children->content);
	}
	key->sources[i].midi_code = atoi(cur_node->children->content);
	i++;
      }
    }
  }
}

static void
gessinger_xmlconfig_setup_auto_keys(GessingerXmlconfig *self,
				    GessingerPreset    *preset,
				    xmlNode            *node)
{
  xmlAttr *attr;
  gint source, tone, octave, num_keys, i, j, num_notes;
  source = 0;
  tone = 0; //C by default
  octave = 4;
  num_notes = 0;
  gchar **key_modes;
  gint *notes_interval;
  GessingerPresetKey *key;
  
  for (attr=node->properties; attr!=NULL; attr=attr->next) {
    if ((attr->children==NULL)||(attr->children->content==NULL)) continue;
    else if (g_str_equal(attr->name, "source")) source = atoi(attr->children->content);
    else if (g_str_equal(attr->name, "tone")) tone = atoi(attr->children->content);
    else if (g_str_equal(attr->name, "octave")) octave = atoi(attr->children->content);
    else if (g_str_equal(attr->name, "num_keys")) preset->num_keys = atoi(attr->children->content);
    else if (g_str_equal(attr->name, "mode")) {

      key_modes = g_strsplit(attr->children->content, " ", -1);
      for (i=0; key_modes[i]!=NULL; i++) num_notes++; //count number of notes
      notes_interval = g_malloc0 (sizeof(int) * num_notes);

      // transform char to int
      for (i=0; key_modes[i]!=NULL; i++) notes_interval[i] = atoi(key_modes[i]);
      g_strfreev(key_modes);
    }
  }

  preset->keys = g_malloc0 (sizeof(GessingerPresetKey) * preset->num_keys);

  for (i=0; i<preset->num_keys; i++) {
    key = &preset->keys[i]; //shortcut for current key

    key->id = i;
    key->num_sources = num_notes;
    key->sources = gessinger_midi_source_new (key->id,
					      notes_interval,
					      num_notes,
					      tone,
					      octave,
					      source);
  }
}

static void
gessinger_xmlconfig_add_preset(GessingerXmlconfig *self,
			       xmlNode            *node)
{
  xmlNode *cur_node;
  xmlAttr *attr;
  GessingerPreset *preset;
  GessingerPresetSource *source;
  gint i;

  preset = gessinger_preset_new();

  for (attr=node->properties; attr!=NULL; attr=attr->next) {
    if ((attr->children!=NULL)&&(attr->children->content!=NULL)) {

      if (g_str_equal(attr->name, "name"))
	preset->name=g_strdup(attr->children->content);

      else if (g_str_equal(attr->name, "mode")) {
	if (attr->children->content == "g")
	  preset->mode = GESSINGER_PRESET_GRAB_MODE;
      }
    }
  }
  if (node->children==NULL) return;

  preset->num_keys = 0;

  for (cur_node=node->children; cur_node; cur_node = cur_node->next) {
    if (cur_node->type != XML_ELEMENT_NODE) continue;
    if (g_str_equal(cur_node->name, "source")) {

      source = gessinger_preset_source_new();

      for (attr=cur_node->properties; attr!=NULL; attr=attr->next) {
	if ((attr->children==NULL)||(attr->children->content==NULL)) continue;
	else if (g_str_equal(attr->name, "id")) source->id = atoi(attr->children->content);
	else if (g_str_equal(attr->name, "font")) source->font = atoi(attr->children->content);
	else if (g_str_equal(attr->name, "bank")) source->bank = atoi(attr->children->content);
	else if (g_str_equal(attr->name, "preset")) source->preset = atoi(attr->children->content);
      }
      g_hash_table_insert (preset->sources, &source->id, source);
    }

    else if (g_str_equal(cur_node->name, "keys")) {
      preset->auto_keys = 1;
      gessinger_xmlconfig_setup_auto_keys(self, preset, cur_node);
    }

    else if (g_str_equal(cur_node->name, "key")) {
      preset->num_keys++;
    }
  }

  if ((preset->num_keys > 0)&&(!preset->auto_keys)) {
    i = 0;
    preset->keys = g_malloc0 (sizeof(GessingerPresetKey) * preset->num_keys);

    for (cur_node=node->children; cur_node; cur_node = cur_node->next) {
      if (cur_node->type != XML_ELEMENT_NODE) continue;
      else if (g_str_equal(cur_node->name, "key")) {

	for (attr=cur_node->properties; attr!=NULL; attr=attr->next) {
	  if ((attr->children==NULL)||(attr->children->content==NULL)) continue;
	  else if (g_str_equal(attr->name, "id")) preset->keys[i].id = atoi(attr->children->content);
	}
	gessinger_xmlconfig_setup_key(self, &preset->keys[i], cur_node);
	i++;
      }
    }
  }
}

static void
gessinger_xmlconfig_read_configs(GessingerXmlconfig *self)
{
  xmlNode *cur_node;
  
  self->doc = xmlReadFile(self->config_file, NULL, 0);
  if (self->doc == NULL) {
    g_error ("Failed to open config file");
    return;
  }

  self->root = xmlDocGetRootElement(self->doc);
  if (self->root == NULL) {
    g_warning ("config file is empty");
    return;
  }

  //load presets
  for (cur_node=self->root->children; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if (g_str_equal(cur_node->name, "preset"))
	    gessinger_xmlconfig_add_preset(self, cur_node);
    }
  }

}

GessingerXmlconfig *
gessinger_xmlconfig_new (gchar *config_file)
{
  GessingerXmlconfig *obj;
  obj = g_object_new (GESSINGER_XMLCONFIG_TYPE, NULL);
  obj->config_file = config_file;
  gessinger_xmlconfig_read_configs(obj);
  return obj;
}
