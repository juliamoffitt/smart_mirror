#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "util.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

void quit_app(GtkWidget *window){
  gtk_widget_destroy(window);
  gtk_main_quit();
}

int mod(int x, int y){
  return x%y;
}

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

// given filename
// returns ptr to first element in xml file
// returns NULL if error
// add root element name parameter to aid error handling
xmlDocPtr parse_doc(char *docname) {
  xmlDocPtr doc;
  xmlNodePtr cur;
  assert(docname);
  doc = xmlParseFile(docname);
  if (doc == NULL ) {
    fprintf(stderr,"Document not parsed successfully. \n");
    return NULL;
  } else {
    return doc;
  }
}

xmlNodePtr get_root_element(xmlDocPtr doc){
  xmlNodePtr cur; 
  cur = xmlDocGetRootElement(doc);
  if (cur == NULL) {
    fprintf(stderr,"empty document\n");
    xmlFreeDoc(doc);
    return NULL;
  }
  if (xmlStrcmp(cur->name, (const xmlChar *) "current")) {
    fprintf(stderr,"document of the wrong type, root node != current");
    xmlFreeDoc(doc);
    return NULL;
  }
  printf("node type: element, name: %s\n", cur->name);
  return cur;
}

// given element and attribute name and doc
// returns value of attribute
// returns null if element or attribute not found
xmlChar *get_property(xmlDocPtr doc, xmlChar *element, xmlChar *attr) {
  xmlNodePtr cur = find_element(doc, element);
  if (cur != NULL) {
    if(xmlHasProp(cur, attr)) {
      return xmlGetProp(cur, attr);
    } else {
      printf("attribute not found\n");
      return NULL;
    }
  } else {
    printf("element not found\n");
    return NULL;
  }
} 

// recursive tree search wrapper func
// to find element
// give doc
// return pointer to element
// if element DNE, returns NULL
xmlNodePtr find_element(xmlDocPtr doc, xmlChar *element_name) {
  xmlNodePtr cur, res;
  cur = get_root_element(doc);
  res = NULL;
  res = find_element_helper(cur, res, element_name);
  return res;
}

// recursive tree search helper func
// to find element
// give current node, result holder, element name to match
// returns pointer to element
xmlNodePtr find_element_helper(xmlNodePtr cur, xmlNodePtr res,
                               xmlChar *element_name) {  
  if (res != NULL) return res;
  if ((!xmlStrcmp(cur->name, element_name))) { //matches
    res = cur;
    return res;
  }
  if (xmlChildElementCount(cur) != 0) { //children
    res = find_element_helper(cur->xmlChildrenNode, res, element_name);
    if (res != NULL) return res;
  }
  if (cur->next != NULL) {
    res = find_element_helper(cur->next, res, element_name);
    if (res != NULL) return res;
  }
  return res;
}

void create_current_file(){
  struct addrinfo hints, *res;
  int sockfd;

  char buf[10000];
  int byte_count;

  //get host info, make socket and connect it
  memset(&hints, 0,sizeof hints);
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo("api.openweathermap.org","80", &hints, &res) != 0) {
    fprintf(stderr, "getaddrinfo failed\n");
  } else {
    sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    //printf("Connecting...\n");
    if (connect(sockfd,res->ai_addr,res->ai_addrlen) != 0){
      fprintf(stderr, "connect failed\n");
    } else {
      printf("Connected!\n");
      char *header = "GET /data/2.5/weather?zip=98101&units=imperial&mode=xml&appid=36f768cab0cdef430b2acf0ffbec6abb HTTP/1.1\r\nHost: api.openweathermap.org\r\n\r\n";
      if (send(sockfd,header,strlen(header),0) !=-1 ) {
        printf("now that we're connected, we can receive some data!");
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
        buf[i] = '\0';
        //printf("%s\n", buf);
      } else {
        fprintf(stderr, "send failed\n");
      }
    }
  }
// ---------------------------------------------------------------------

  FILE *fp;
  int i;
  fp = fopen("./current.xml", "w");
  if (fp!= NULL) {
    for (i=0; buf[i] != '\0'; i++){
      fprintf(fp, "%c", buf[i]);
    }
    if (fclose(fp)!=0) {
      printf("fclose error\n");
    }
  } else {
    printf("f error\n");
  }
// --------------------------------------------------------------------

}

char* get_icon_string(xmlChar *icon_code) {
  if( !strcmp(icon_code, "01d") ) {
    return "./weather_icons/icons8-bright-sunny-day.png";
  } else if ( !strcmp(icon_code, "02d") ) {
    return "./weather_icons/icons8-partly-cloudy-day.png";
  } else if ( !strcmp(icon_code, "03d") ) {
    return "./weather_icons/icons8-cloud-host.png";
  } else if ( !strcmp(icon_code, "04d") ) {
    return "./weather_icons/icons8-cloud-host.png";
  } else if ( !strcmp(icon_code, "09d") ) {
    return "./weather_icons/icons8-rainfall.png";
  } else if ( !strcmp(icon_code, "10d") ) {
    return "./weather_icons/icons8-rain-cloud.png";
  } else if ( !strcmp(icon_code, "11d") ) {
    return "./weather_icons/icons8-cloud-burst.png";
  } else if ( !strcmp(icon_code, "13d") ) {
    return "./weather_icons/icons8-snow-fall.png";
  } else if ( !strcmp(icon_code, "50d") ) {
    return "./weather_icons/icons8-haze.png";
  } else if ( !strcmp(icon_code, "01n") ) {
    return "./weather_icons/icons8-moon-and-stars.png";
  } else if ( !strcmp(icon_code, "02n") ) {
    return "./weather_icons/icons8-cloud-host.png";
  } else if ( !strcmp(icon_code, "03n") ) {
    return "./weather_icons/icons8-cloud-host.png";
  } else if ( !strcmp(icon_code, "04n") ) {
    return "./weather_icons/icons8-cloud-host.png";
  } else if ( !strcmp(icon_code, "09n") ) {
    return "./weather_icons/icons8-rainfall.png";
  } else if ( !strcmp(icon_code, "10n") ) {
    return "./weather_icons/icons8-rainfall.png";
  } else if ( !strcmp(icon_code, "11n") ) {
    return "./weather_icons/icons8-cloud-burst.png";
  } else if ( !strcmp(icon_code, "13n") ) {
    return "./weather_icons/icons8-snow-fall.png";
  } else if ( !strcmp(icon_code, "50n") ) {
    return "./weather_icons/icons8-fog.png";
  } else {  
    return NULL;
  }
}
