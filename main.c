#include <glib.h>
#include <gessinger/gui.h>
#include <gessinger/jscontrol.h>

int main(int argc, char *argv[])
{
  GessingerGui *gui;
  GessingerJscontrol *jsct;
  GError *error = NULL;
  gtk_init(&argc, &argv);

  jsct = gessinger_jscontrol_new("/dev/input/js0",
				 &error);

  if ((jsct==NULL)&&(error!=NULL))
    {
      g_print ("ERRO: %s\n", error->message);
      g_error_free(error);
    }

  gui = gessinger_gui_new();
  gessinger_gui_show(gui);
  gtk_main();
  return 0;
}
