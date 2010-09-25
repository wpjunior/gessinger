#include <glib.h>
#include <gessinger/gui.h>
#include <gessinger/jscontrol.h>
#include <gessinger/xmlconfig.h>
#include <gessinger/interface.h>

int main(int argc, char *argv[])
{
  GessingerGui *gui;
  GessingerJscontrol *jsct;
  GessingerXmlconfig *gxml_config;
  GError *error = NULL;
  GtkDialog *dialog;
  gtk_init(&argc, &argv);

  jsct = gessinger_jscontrol_new("/dev/input/js0",
				 &error);

  if ((jsct==NULL)&&(error!=NULL))
    {
      dialog = gtk_message_dialog_new (NULL,
				       GTK_DIALOG_DESTROY_WITH_PARENT,
				       GTK_MESSAGE_ERROR,
				       GTK_BUTTONS_CLOSE,
				       error->message);
      gtk_dialog_run (GTK_DIALOG (dialog));
      exit(1);
    }

  gxml_config = gessinger_xmlconfig_new("config.xml");

  gui = gessinger_gui_new(gxml_config);
  gessinger_gui_show(gui);
  gtk_main();
  return 0;
}
