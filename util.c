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
xmlNodePtr parse_doc(char *docname) {
  xmlDocPtr doc;
  xmlNodePtr cur;
  assert(docname);
  doc = xmlParseFile(docname);
  if (doc == NULL ) {
    fprintf(stderr,"Document not parsed successfully. \n");
    return NULL;
  }
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
 // cur = cur->xmlChildrenNode;
 /* while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"storyinfo"))){
   //   parse_ (doc, cur);
      printf("not sure what to put here yet\n");
    }
    cur = cur->next;
  }
  xmlFreeDoc(doc); */
  printf("node type: element, name: %s\n", cur->name);
  return cur;
} 

// given ptr to doc, and ptr to root node
// returns ptr to node with specified element name
// returns NULL if no such element exists
// use xmlNodeListGetString to get value of element
xmlNodePtr get_element(xmlDocPtr doc, xmlNodePtr cur, 
                  const xmlChar *element_name) {
  xmlChar *element; 
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) element_name))) {
      element = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
      printf("element is: %s\n", element);
      xmlFree(element);
      return cur;
    } cur = cur->next;
  }
  cur = NULL;
  printf("element not found");
  return cur; 
}

// assume we are given proper ptr to element
// add error handling
xmlChar * get_attribute(xmlDocPtr doc, xmlNodePtr cur,
                       const xmlChar *attribute_name) {
  xmlChar *attribute_value = xmlGetProp(cur, attribute_name);
  printf("%s is %s\n", attribute_name, attribute_value);
  return attribute_value;
}
