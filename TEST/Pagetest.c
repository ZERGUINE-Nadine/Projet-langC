#include <gtk/gtk.h>

// Fonction de rappel pour fermer la fenêtre
void on_quit_button_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void on_next_to_page2_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **windows = (GtkWidget **)data;

    gtk_widget_hide(windows[0]); // Cache la page 1
    gtk_widget_show_all(windows[1]); // Affiche la page 2
}

// Fonction de rappel pour afficher la page 1
void on_next_to_page1_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **windows = (GtkWidget **)data;
    gtk_widget_hide(windows[1]); // Cache la page 2
    gtk_widget_show_all(windows[0]); // Affiche la page 1
}

// Fonction de rappel appelée lorsque l'utilisateur clique sur le bouton1
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Hello, World!\n");  // Affiche "Hello, World!" dans le terminal
}

// Fonction de rappel appelée lorsque l'utilisateur clique sur le bouton1
static void on_button_clicked2(GtkWidget *widget, gpointer data) {
    g_print("World,HELLO!\n");  // Affiche "Hello, World!" dans le terminal
}

GtkWidget* create_page_1(GtkWidget **windows);
GtkWidget* create_page_2(GtkWidget **windows);

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Tableaux de pointeurs pour les deux fenêtres (page 1 et page 2)
    GtkWidget *windows[2]; // Suppression de l'élément pour le pipeline GStreamer

    // Création des deux fenêtres
    windows[0] = create_page_1(windows);
    windows[1] = create_page_2(windows);

    gtk_widget_show_all(windows[0]); // Affiche la page 1 au démarrage

    gtk_main();

    return 0;
}

GtkWidget* create_page_1(GtkWidget **windows) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *hello_button;
    GtkWidget *next_button;
    GtkWidget *quit_button;

    // Création de la fenêtre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Page 1");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la box verticale
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Bouton "Hello World"
    hello_button = gtk_button_new_with_label("Hello World");
    gtk_box_pack_start(GTK_BOX(box), hello_button, TRUE, TRUE, 0);
    // Connecte le signal de clic du bouton à la fonction de rappel
    g_signal_connect(hello_button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // Bouton "Suivant"
    next_button = gtk_button_new_with_label("Suivant");
    gtk_box_pack_start(GTK_BOX(box), next_button, TRUE, TRUE, 0);
    g_signal_connect(next_button, "clicked", G_CALLBACK(on_next_to_page2_clicked), windows);

    // Bouton "Quitter"
    quit_button = gtk_button_new_with_label("Quitter");
    gtk_box_pack_start(GTK_BOX(box), quit_button, TRUE, TRUE, 0);
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit_button_clicked), NULL);

    return window;
}

GtkWidget* create_page_2(GtkWidget **windows) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *world_hello_button;
    GtkWidget *next_button;

    // Création de la fenêtre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Page 2");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la box verticale
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Bouton "World Hello"
    world_hello_button = gtk_button_new_with_label("World Hello");
    gtk_box_pack_start(GTK_BOX(box), world_hello_button, TRUE, TRUE, 0);
    // Connecte le signal de clic du bouton à la fonction de rappel
    g_signal_connect(world_hello_button, "clicked", G_CALLBACK(on_button_clicked2), NULL);

    // Bouton "Suivant"
    next_button = gtk_button_new_with_label("Suivant");
    gtk_box_pack_start(GTK_BOX(box), next_button, TRUE, TRUE, 0);
    g_signal_connect(next_button, "clicked", G_CALLBACK(on_next_to_page1_clicked), windows);

    return window;
}
