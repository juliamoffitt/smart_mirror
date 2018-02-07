#include <gtk/gtk.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "util.h"


int
main (int   argc,
      char *argv[])
{
  gtk_init(&argc, &argv);

  GtkWidget *window;
  GtkWidget *view1;
  GtkWidget *view2;
  GtkWidget *view3;
  GtkWidget *view4;
  GtkWidget *button;

  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  GtkTextIter iter;

  GtkCssProvider *Provider = gtk_css_provider_new();
  GdkDisplay *Display = gdk_display_get_default();
  GdkScreen *Screen = gdk_display_get_default_screen(Display);

  gtk_style_context_add_provider_for_screen(Screen, 
    GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), 
    "/home/julia/Coding/smart_mirror/styles.css", NULL);
    
  // configure window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "GtkTextView");
  gtk_window_fullscreen(GTK_WINDOW(window));

  // view1 
  view1 = gtk_text_view_new();
  gtk_widget_set_name(view1, "view1");
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view1));
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  gtk_text_buffer_insert(buffer, &iter, get_date_string(), -1);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view1), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view1), FALSE);
  
  // view2 
  view2 = gtk_text_view_new();
  gtk_widget_set_name(view2, "view2");
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view2));
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  gtk_text_buffer_insert(buffer, &iter, local_time(), -1);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view2), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view2), FALSE);
  gtk_text_view_set_justification(GTK_TEXT_VIEW(view2), GTK_JUSTIFY_RIGHT);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view2), GTK_WRAP_WORD);
  
  // view3 
  view3 = gtk_text_view_new();
  gtk_widget_set_name(view3, "view3");
/*  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view3));
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  gtk_text_buffer_insert(buffer, &iter, get_time_string(), -1);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view3), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view3), FALSE);
*/
  

  // view4
  view4 = gtk_text_view_new();
  gtk_widget_set_name(view4, "view4");
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view4));
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  gtk_text_buffer_insert(buffer, &iter, local_time(), -1);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view4), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view4), FALSE);

  button = gtk_button_new_with_label ("Quit");
  g_signal_connect_swapped (button, "clicked", 
    G_CALLBACK (gtk_widget_destroy), window);
  gtk_widget_set_name(button, "button");

  GtkWidget *main_container;
  GtkWidget *h_box_1;
  GtkWidget *h_box_2;
  GtkWidget *v_box_1;
  GtkWidget *v_box_2;
  
  main_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  h_box_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  h_box_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  v_box_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  v_box_2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  gtk_box_pack_start(GTK_BOX(main_container), h_box_1, TRUE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(main_container), h_box_2, FALSE, TRUE, 5);

  gtk_box_pack_start(GTK_BOX(h_box_1), v_box_1, TRUE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(h_box_1), v_box_2, TRUE, TRUE, 1);

  gtk_box_pack_start(GTK_BOX(v_box_1), view1, FALSE, TRUE, 1);
  //gtk_box_pack_start(GTK_BOX(v_box_1), view3, TRUE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(v_box_2), view2, TRUE, TRUE, 1);

  gtk_box_pack_start(GTK_BOX(h_box_2), button, TRUE, FALSE, 1);

  gtk_container_add(GTK_CONTAINER(window), main_container);
  
  gchar *t;
  GtkWidget *label_time;
  t = get_time_string();
  label_time = gtk_label_new(t);
  g_free (t);
  gtk_box_pack_start(GTK_BOX(v_box_1), label_time, TRUE, TRUE, 1);
//  gtk_container_add(GTK_CONTAINER(v_box_1), label_time);
//  gtk_text_buffer_insert(buffer, &iter, label_time, -1);

  g_timeout_add_seconds(0.5, update_label_time, label_time);

  gtk_widget_show_all(window);
  gtk_main();
  
  return 0;
}
