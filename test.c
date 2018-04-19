gchar *w;
GtkWidget *label_temp;

pp = get_temp_string();
label_temp = gtk_label_new(w);
g_free(w);
gtk_widget_set_halign(label_placeholder, GTK_ALIGN_END);
gtk_box_pack_start(GTK_BOX(v_box_2), label_temp, FALSE, FALSE, 1);
gtk_widget_set_name(label_temp, "label_temp");
