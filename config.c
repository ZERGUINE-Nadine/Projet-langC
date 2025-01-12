#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "includes/config.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600 ;

User user_data;

Fiche fiches[MAX_FICHES];
int fiche_count = 0;

void load_user_data() {
    user_data.name = g_strdup("John Doe");
    user_data.email = g_strdup("john.doe@example.com");
}

void save_user_info() {
    FILE *file = fopen("user_info.txt", "w");
    if (file != NULL) {
        fprintf(file, "Name:%s\nEmail:%s\n", user_data.name, user_data.email);
        fclose(file);
    }
}

void on_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void on_deconnexion_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void setup_header(GtkWidget *header, GtkWidget *content_area) {
    GtkWidget *menubar;
    GtkWidget *menuitem_fiche;
    GtkWidget *menuitem_exam;
    GtkWidget *menuitem_deconnexion;

    menubar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(header), menubar, FALSE, FALSE, 0);

    menuitem_fiche = gtk_menu_item_new_with_label("Fiches");
    g_signal_connect(menuitem_fiche, "activate", G_CALLBACK(show_fiches), content_area);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem_fiche);

    menuitem_exam = gtk_menu_item_new_with_label("Examens");
    g_signal_connect(menuitem_exam, "activate", G_CALLBACK(show_exam), content_area);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem_exam);

    menuitem_deconnexion = gtk_menu_item_new_with_label("Déconnexion");
    g_signal_connect(menuitem_deconnexion, "activate", G_CALLBACK(on_deconnexion_clicked), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem_deconnexion);
}
