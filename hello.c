#define _XOPEN_SOURCE
#define _GNU_SOURCE
#define __USE_XOPEN
#include <assert.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "util.h"
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include <libxml2/libxml/xmlmemory.h>
#include <libxml2/libxml/parser.h>

int
main (int   argc,
      char *argv[])
{
  gtk_init(&argc, &argv);

  GtkWidget *window;
  GtkWidget *button;

//--------------------------------------------------------------------
//---------CSS provider-----------------------------------------------

  GtkCssProvider *Provider = gtk_css_provider_new();
  GdkDisplay *Display = gdk_display_get_default();
  GdkScreen *Screen = gdk_display_get_default_screen(Display);

  gtk_style_context_add_provider_for_screen(Screen,
    GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider),
    "./styles.css", NULL);


//--------------------------------------------------------------------
//---------configure window-------------------------------------------
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "GtkTextView");
  gtk_window_fullscreen(GTK_WINDOW(window));


  button = gtk_button_new_with_label ("Quit");
  g_signal_connect_swapped (button, "clicked",
    G_CALLBACK (quit_app), window);
  gtk_widget_set_name(button, "button");

  GtkWidget *main_container;
  GtkWidget *h_box_1;
  GtkWidget *h_box_2;
  GtkWidget *h_box_3;
  GtkWidget *h_box_4;

  GtkWidget *v_box_1;
  GtkWidget *v_box_2;

  main_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  h_box_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  h_box_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  h_box_3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  h_box_4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  v_box_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  v_box_2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  gtk_widget_set_name(v_box_1, "v_box_1");
  gtk_widget_set_name(v_box_2, "v_box_2");
  gtk_widget_set_name(h_box_1, "h_box_1");
  gtk_widget_set_name(h_box_2, "h_box_2");
  gtk_widget_set_name(h_box_3, "h_box_3");
  gtk_widget_set_name(h_box_4, "h_box_4");


  gtk_box_pack_start(GTK_BOX(main_container), h_box_1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(main_container), h_box_2, FALSE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(h_box_1), v_box_1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(h_box_1), v_box_2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(h_box_2), button, TRUE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(v_box_2), h_box_3, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(v_box_2), h_box_4, FALSE, TRUE, 0);

  gtk_container_add(GTK_CONTAINER(window), main_container);

//-------------------------------------------------------------------
//---------get time and date and pack--------------------------------

  gchar *t;
  gchar *d;
  gchar *p;
  GtkWidget *label_date;
  GtkWidget *label_time;
  GtkWidget *label_placeholder;

  d = get_date_string();
  label_date = gtk_label_new(d);
  g_free (d);
  gtk_widget_set_halign(label_date, GTK_ALIGN_START);
  gtk_box_pack_start(GTK_BOX(v_box_1), label_date, FALSE, FALSE, 0);
  gtk_widget_set_name(label_date, "label_date");

  t = get_time_string();
  label_time = gtk_label_new(t);
  g_free (t);
  gtk_widget_set_halign(label_time, GTK_ALIGN_START);
  gtk_box_pack_start(GTK_BOX(v_box_1), label_time, FALSE, FALSE, 0);
  gtk_widget_set_name(label_time, "label_time");

  g_timeout_add_seconds(0.5, update_label_time, label_time);
  //g_timeout_add_seconds(0.5, update_label_date, label_date);
  create_current_file();

//-------------------------------------------------------------------
//----------get temp string with deg symbol--------------------------

  xmlDocPtr doc = parse_doc("current.xml");
  xmlChar temp[100];
  strcpy(temp, get_property(doc, "temperature", "value"));

  int i = 0;
  while(temp[i] != '\0' && temp[i] != '.') {
    i++;
  }

  if(temp[i] == '.') temp[i] = '\0';

  xmlChar deg[10];
  strcpy(deg, "°");
  strcat(temp, deg);

  xmlChar *temp_string = temp;
  printf("temperature is %s\n", temp_string);

//-------------------------------------------------------------------
//---------get time of sunset in correct format----------------------

  xmlChar *sunset = get_property(doc, "sun", "set");
  printf("sunset is %s\n", sunset);
  struct tm sunset_time;
  xmlChar sunset_formatted[80];

  strptime(sunset, "%Y-%m-%dT%H:%M%S", &sunset_time);
  sunset_time.tm_hour = sunset_time.tm_hour+17%24;
  strftime(sunset_formatted, 80, "%l:%M %p", &sunset_time);
  printf("sunset time is: %s\n", sunset_formatted);
//-------------------------------------------------------------------
//---------pack sunset and temp--------------------------------------

  GtkWidget *label_sunset;
  label_sunset = gtk_label_new(sunset_formatted);
  //g_free(sunset);
  gtk_widget_set_halign(label_sunset, GTK_ALIGN_END);
  gtk_widget_set_valign(label_sunset, GTK_ALIGN_START);
  gtk_box_pack_end(GTK_BOX(h_box_3), label_sunset, FALSE, FALSE, 0);
  gtk_widget_set_name(label_sunset, "label_sunset");

  GtkWidget *label_temp;
  label_temp = gtk_label_new(temp_string);
  //g_free(temp_string);
  gtk_widget_set_halign(label_temp, GTK_ALIGN_END);
  gtk_widget_set_valign(label_temp, GTK_ALIGN_START);
  gtk_box_pack_end(GTK_BOX(h_box_4), label_temp, FALSE, FALSE, 0);
  gtk_widget_set_name(label_temp, "label_temp");


//-------------------------------------------------------------------
//---------display icons---------------------------------------------
  GtkWidget *image;
  char *icon_path;
  xmlChar *icon_code;

  icon_code = get_property(doc, "weather", "icon");
  if (icon_code != NULL) {
    icon_path = get_icon_string(icon_code);
    printf("icon code is %s\n", icon_code);
    if (icon_path != NULL) {
      image = gtk_image_new_from_file(icon_path);
      printf("icon path is %s\n", icon_path);
      gtk_widget_set_halign(image, GTK_ALIGN_END);
      gtk_widget_set_valign(image, GTK_ALIGN_START);
      gtk_box_pack_end(GTK_BOX(h_box_4), image, FALSE, FALSE, 0);
    } else printf ("icon path is null");
  } else printf ("icon code is null");

  GtkWidget *sunset_icon;
  sunset_icon = gtk_image_new_from_file("./weather_icons/icons8-sunset.png");
  gtk_widget_set_halign(sunset_icon, GTK_ALIGN_END);
  gtk_widget_set_valign(sunset_icon, GTK_ALIGN_START);
  gtk_box_pack_end(GTK_BOX(h_box_3), sunset_icon, FALSE, FALSE, 0);

/*
 * TO DO
 *   -change location to current location for xml
 *   -remove leading zero from time
 *   -adjust padding
 *   -error handling
 *   -clean code and comment
 *   -documentation
 */
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
