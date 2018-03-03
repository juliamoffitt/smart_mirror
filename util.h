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
xmlDocPtr parse_doc(char *docname);
xmlNodePtr get_root_element(xmlDocPtr doc);
xmlChar* get_property(xmlDocPtr doc, xmlChar *element, xmlChar *attr);
xmlNodePtr find_element_helper(xmlNodePtr Cur, xmlNodePtr res,
                               xmlChar *element_name);
xmlNodePtr find_element(xmlDocPtr doc, xmlChar *element_name);

