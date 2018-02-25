#include <gtk/gtk.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "util.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>


int
main (int   argc,
      char *argv[])
{
  gtk_init(&argc, &argv);

  GtkWidget *window;
  GtkWidget *button;

  GtkCssProvider *Provider = gtk_css_provider_new();
  GdkDisplay *Display = gdk_display_get_default();
  GdkScreen *Screen = gdk_display_get_default_screen(Display);

  gtk_style_context_add_provider_for_screen(Screen, 
    GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), 
    "/home/julia/Coding/smart_mirror/smart_mirror/styles.css", NULL);
    
  // configure window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "GtkTextView");
  gtk_window_fullscreen(GTK_WINDOW(window));
  

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

  gtk_widget_set_name(v_box_1, "v_box_1");
  gtk_widget_set_name(v_box_2, "v_box_2");
  gtk_widget_set_name(h_box_1, "h_box_1");
  gtk_widget_set_name(h_box_2, "h_box_2");

  gtk_box_pack_start(GTK_BOX(main_container), h_box_1, TRUE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(main_container), h_box_2, FALSE, TRUE, 5);

  gtk_box_pack_start(GTK_BOX(h_box_1), v_box_1, TRUE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(h_box_1), v_box_2, TRUE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(h_box_2), button, TRUE, FALSE, 1);

  gtk_container_add(GTK_CONTAINER(window), main_container);
  
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
  gtk_box_pack_start(GTK_BOX(v_box_1), label_date, FALSE, FALSE, 1);
  gtk_widget_set_name(label_date, "label_date");

  t = get_time_string();
  label_time = gtk_label_new(t);
  g_free (t);
  gtk_widget_set_halign(label_time, GTK_ALIGN_START);
  gtk_box_pack_start(GTK_BOX(v_box_1), label_time, FALSE, FALSE, 1); 
  gtk_widget_set_name(label_time, "label_time");

  p = get_date_string();
  label_placeholder = gtk_label_new(p);
  g_free (p);
  gtk_widget_set_halign(label_placeholder, GTK_ALIGN_END);
  gtk_box_pack_start(GTK_BOX(v_box_2), label_placeholder, FALSE, FALSE, 1);
  gtk_widget_set_name(label_placeholder, "label_placeholder");

  g_timeout_add_seconds(0.5, update_label_time, label_time);
  //g_timeout_add_seconds(0.5, update_label_date, label_date);

// -----------------------------------------------------------------------------
  struct addrinfo hints, *res;
  int sockfd;

  char buf[10000];
  int byte_count;

  //get host info, make socket and connect it
  memset(&hints, 0,sizeof hints);
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo("api.openweathermap.org","80", &hints, &res) != 0) {
    printf("getaddrinfo failed\n");
  } else {
    sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    printf("Connecting...\n");
    if (connect(sockfd,res->ai_addr,res->ai_addrlen) != 0){
      printf("connect failed\n");
    } else {
      printf("Connected!\n");
      char *header = "GET /data/2.5/weather?q=London&mode=xml&appid=36f768cab0cdef430b2acf0ffbec6abb HTTP/1.1\r\nHost: api.openweathermap.org\r\n\r\n";
      if (send(sockfd,header,strlen(header),0) !=-1 ) {
        printf("GET Sent...\n");
        //all right ! now that we're connected, we can receive some data!
        byte_count = recv(sockfd,buf,sizeof(buf)-1,0);
        buf[byte_count] = 0;

        printf("recv()'d %d bytes of data in buf\n",byte_count);
        printf("%s\n",buf);
        int c=0; 
        while (buf[c] != '<') {
          c++;
        }
        printf("c is %d\n", c);
        int i=0; 
        while (buf[c] !='\0') {
          buf[i] = buf[c];
          i++;
          c++;
        }
        buf[c] = '\0';
        printf("%s\n", buf);
      } else {
        printf("send failed\n");
      }
    }
  }
// ----------------------------------------------------------------------------

  FILE *fp;
  int i;
  fp = fopen("./weather.txt", "w");
  if (fp!= NULL) {
    for (i=0; buf[i] != '\0'; i++){
      fprintf(fp, "%c", buf[i]);
    }
    if (fclose(fp)!=0) {
      printf("fclose error\n");
    }
  } else {
    printf("fopen error\n");
  }

  

/*
 * TO DO
 *   -implement xml parser
 *   -display weather
 *   -style weather
 *   -error handling
 *   -clean code and comment
 *   -documentation
 *   -write test cases
 *   -???
 */
  //gtk_widget_show_all(window);
  gtk_main();
  
  return 0;
}
