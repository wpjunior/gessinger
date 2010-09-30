#include <glib.h>
#include <gessinger/gui.h>
#include <gessinger/jscontrol.h>
#include <gessinger/xmlconfig.h>
#include <gessinger/interface.h>

int main(int argc, char *argv[])
{
  GessingerInterface *interface;
  GessingerGui *gui;
  GessingerJscontrol *jsct;
  GessingerXmlconfig *gxml_config;
  GError *error = NULL;
  GtkWidget *dialog;
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
  interface = gessinger_interface_new(gxml_config, jsct);

  gui = gessinger_gui_new(interface);
  gessinger_gui_show(gui);
  gtk_main();
  return 0;
}
