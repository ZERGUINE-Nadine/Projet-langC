#include <gtk/gtk.h>

// Fonction pour charger le fichier CSS
void load_css() {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(provider, "style.css", NULL);

    g_object_unref(provider);
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;

    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Charger le CSS
    load_css();

    // Charger l'interface Glade
    builder = gtk_builder_new_from_file("inscription.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "inscription"));

    // Connecter le signal "destroy" pour fermer l'application
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Afficher la fenêtre
    gtk_widget_show_all(window);

    // Lancer la boucle principale de GTK
    gtk_main();

    return 0;
}
