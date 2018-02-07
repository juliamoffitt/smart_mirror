#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "util.h"

char* get_date_string(){
  time_t time_var = time(NULL);
  struct tm *info;
  char *date_string = calloc(100, sizeof(char));
  info = localtime( &time_var );
  strftime(date_string, 100, "%A, %B %d", info);
  return date_string;
}

char* get_time_string(){
  time_t time_var = time(NULL);
  struct tm *info;
  char *time_string = calloc(100, sizeof(char));
  info = localtime( &time_var );
  strftime(time_string, 100, "%I:%M %p", info);
  return time_string;
}

char *local_time(){
  time_t time_var = time(NULL);
  struct tm *info;
  info = localtime(&time_var);
  return asctime(info);
}

gboolean update_label_time (gpointer user_data) {
  gchar *t = get_time_string();
  gtk_label_set_text(GTK_LABEL(user_data), t);
  g_free (t);
  return G_SOURCE_CONTINUE;
}

gboolean update_label_date (gpointer user_data) {
  gchar *d = get_date_string();
  gtk_label_set_text(GTK_LABEL(user_data), d);
  g_free (d);
  return G_SOURCE_CONTINUE;
}
