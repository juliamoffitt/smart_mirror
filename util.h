#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

char* get_date_string();
char* get_time_string();
char* local_time();
gboolean update_label_time(gpointer user_data);
gboolean update_label_date(gpointer user_data);
xmlNodePtr parse_doc(char *docname);
xmlNodePtr get_element(xmlDocPtr doc, xmlNodePtr cur, 
                      const xmlChar *element_name);
xmlChar * get_attribute(xmlDocPtr doc, xmlNodePtr cur,
                       const xmlChar* attribute_name);
