#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "includes/config.h"

int MAX_FICHES;
int MAX_QUESTIONS;
int CONFIG_WINDOW_WIDTH;
int CONFIG_WINDOW_HEIGHT;

Fiche *fiches;
int fiche_count = 0;

void read_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[64];
        int value;
        if (sscanf(line, "%[^=]=%d", key, &value) == 2) {
            if (strcmp(key, "MAX_FICHES") == 0) {
                MAX_FICHES = value;
            } else if (strcmp(key, "MAX_QUESTIONS") == 0) {
                MAX_QUESTIONS = value;
            } else if (strcmp(key, "CONFIG_WINDOW_WIDTH") == 0) {
                CONFIG_WINDOW_WIDTH = value;
            } else if (strcmp(key, "CONFIG_WINDOW_HEIGHT") == 0) {
                CONFIG_WINDOW_HEIGHT = value;
            }
        }
    }

    fclose(file);
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



int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    read_config("config.txt");

    // Allocation dynamique pour les fiches et les questions
    fiches = malloc(MAX_FICHES * sizeof(Fiche));
    if (fiches == NULL) {
        perror("Failed to allocate memory for fiches");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_FICHES; i++) {
        fiches[i].questions = malloc(MAX_QUESTIONS * sizeof(Question));
        if (fiches[i].questions == NULL) {
            perror("Failed to allocate memory for questions");
            exit(EXIT_FAILURE);
        }
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Application de Gestion");
    gtk_window_set_default_size(GTK_WINDOW(window), CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), header, FALSE, FALSE, 0);
    setup_header(header, vbox);

    GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), content_area, TRUE, TRUE, 0);

    load_fiches_from_file();
    show_fiches(NULL, content_area);

    gtk_widget_show_all(window);
    gtk_main();

    for (int i = 0; i < MAX_FICHES; i++) {
        free(fiches[i].questions);
    }
    free(fiches);

    return 0;
}
