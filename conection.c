#include <gtk/gtk.h>

// Fonction de rappel pour fermer l'application
void on_quit_button_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

// Fonction de rappel pour afficher la page 2
void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **windows = (GtkWidget **)data;
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(windows[0]));  // Récupère le texte du champ "Nom"
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(windows[1]));  // Récupère le texte du champ "Mot de passe"

    // Vérifie si les informations de connexion sont correctes
    if (g_strcmp0(name, "Ethan") == 0 && g_strcmp0(password, "1234") == 0) {
        // Si c'est correct, passe à la page 2
        gtk_widget_hide(windows[2]); // Cache la page 1
        gtk_widget_show_all(windows[3]); // Affiche la page 2
    } else {
        // Sinon, affiche un message d'erreur
        GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(windows[2]),
                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                        GTK_MESSAGE_ERROR,
                                                        GTK_BUTTONS_OK,
                                                        "Nom ou mot de passe incorrect !");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
    }
}

// Fonction pour créer la page d'inscription
GtkWidget* create_page_1(GtkWidget **windows) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *name_label, *password_label;
    GtkWidget *name_entry, *password_entry;
    GtkWidget *login_button;

    // Création de la fenêtre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Page d'inscription");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la box verticale
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Label et champ pour le nom
    name_label = gtk_label_new("Nom :");
    gtk_box_pack_start(GTK_BOX(box), name_label, FALSE, FALSE, 0);
    name_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), name_entry, FALSE, FALSE, 0);

    // Label et champ pour le mot de passe
    password_label = gtk_label_new("Mot de passe :");
    gtk_box_pack_start(GTK_BOX(box), password_label, FALSE, FALSE, 0);
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Cache le mot de passe
    gtk_box_pack_start(GTK_BOX(box), password_entry, FALSE, FALSE, 0);

    // Bouton de connexion
    login_button = gtk_button_new_with_label("Se connecter");
    gtk_box_pack_start(GTK_BOX(box), login_button, TRUE, TRUE, 0);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), windows);

    // Stocker les éléments dans le tableau windows
    windows[0] = name_entry;
    windows[1] = password_entry;
    windows[2] = window;

    return window;
}

// Fonction pour créer la page 2
GtkWidget* create_page_2(GtkWidget **windows) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *quit_button;
    GtkWidget *title_label;

    // Création de la fenêtre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Page 2");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la box verticale
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Titre "BRAVO !"
    title_label = gtk_label_new("BRAVO !");
    gtk_box_pack_start(GTK_BOX(box), title_label, TRUE, TRUE, 0);

    // Bouton "Quitter"
    quit_button = gtk_button_new_with_label("Quitter");
    gtk_box_pack_start(GTK_BOX(box), quit_button, TRUE, TRUE, 0);
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit_button_clicked), NULL);

    windows[3] = window; // Stocke la page 2 dans le tableau

    return window;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Tableau de pointeurs pour les fenêtres
    GtkWidget *windows[4]; // 2 fenêtres + champs de texte + boutons

    // Création des pages
    windows[2] = create_page_1(windows);
    windows[3] = create_page_2(windows);

    gtk_widget_show_all(windows[2]); // Affiche la page 1 au démarrage

    gtk_main();

    return 0;
}
