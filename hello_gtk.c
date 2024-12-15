#include <gtk/gtk.h>

// Callback pour le clic sur le bouton
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    // Création de la fenêtre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hello GTK");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Création du bouton
    button = gtk_button_new_with_label("Click Me");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(window), button);

    // Connexion du signal pour fermer la fenêtre
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
