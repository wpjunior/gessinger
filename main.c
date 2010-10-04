#include <glib.h>
#include <gessinger/gui.h>
#include <gessinger/jscontrol.h>
#include <gessinger/presets_config.h>
#include <gessinger/interface.h>

void show_error(GError *error) {
  GtkWidget *dialog;
  if (error==NULL) return;

  dialog = gtk_message_dialog_new (NULL,
				   GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,
				   GTK_BUTTONS_CLOSE,
				   error->message);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}
int main(int argc, char *argv[])
{
  GessingerInterface *interface;
  GessingerGui *gui;
  GessingerJscontrol *jsct;
  GessingerPresetsConfig *presets_config;
  GError *error = NULL;
  GtkWidget *dialog;
  gtk_init(&argc, &argv);

  jsct = gessinger_jscontrol_new("/dev/input/js0",
				 &error);

  if ((jsct==NULL)&&(error!=NULL)) {
    show_error(error);
    return 1;
  }

  presets_config = gessinger_presets_config_new("presets.xml", &error);

  if ((presets_config==NULL)&&(error!=NULL)) {
    show_error(error);
    return 1;
  }

  interface = gessinger_interface_new(presets_config, jsct);

  gui = gessinger_gui_new(interface);
  gessinger_gui_show(gui);
  gtk_main();
  return 0;
}
