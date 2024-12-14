#include <gtk/gtk.h> // Inclut la bibliothèque GTK
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Fonction de rappel appelée lorsque l'utilisateur clique sur le bouton
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Hello, World!\n");  // Affiche "Hello, World!" dans le terminal
}

int main(int argc, char *argv[]) {
    GtkWidget *window;  // La fenêtre principale
    GtkWidget *button;  // Le bouton à l'intérieur de la fenêtre

    // Initialise GTK
    gtk_init(&argc, &argv);

    // Crée la fenêtre principale
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hello, World!"); // Titre de la fenêtre
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100); // Taille par défaut
    gtk_container_set_border_width(GTK_CONTAINER(window), 10); // Bordure de 10px

    // Connecte le signal de fermeture de la fenêtre
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crée un bouton avec le label "Hello, World!"
    button = gtk_button_new_with_label("Hello, World!");

    // Connecte le signal de clic du bouton à la fonction de rappel
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // Ajoute le bouton dans la fenêtre
    gtk_container_add(GTK_CONTAINER(window), button);

    // Affiche tous les widgets de la fenêtre
    gtk_widget_show_all(window);

    // Lance la boucle principale de GTK
    gtk_main();

    return 0;
}
