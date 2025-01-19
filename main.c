#include <gtk/gtk.h>
#include "includes/config.h"

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *content_area;
    GtkWidget *header;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Application de Gestion");
    gtk_window_set_default_size(GTK_WINDOW(window), CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), header, FALSE, FALSE, 0);
    setup_header(header, vbox);

    content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), content_area, TRUE, TRUE, 0);

    load_fiches_from_file();
    show_fiches(NULL, content_area);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}