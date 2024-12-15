#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char window_title[256];
    int window_width;
    int window_height;
    gboolean window_resizable;
    char theme_name[256];
    char icon_theme_name[256];
    char button_label[256];
    char entry_placeholder[256];
    int font_size;
    int auto_save_interval;
    gboolean enable_logging;
} AppConfig;

void read_config(const char *filename, AppConfig *config) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Impossible d'ouvrir le fichier de configuration");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "window_title=%[^\n]", config->window_title) == 1) continue;
        if (sscanf(line, "window_width=%d", &config->window_width) == 1) continue;
        if (sscanf(line, "window_height=%d", &config->window_height) == 1) continue;
        if (sscanf(line, "window_resizable=%s", line) == 1) {
            config->window_resizable = (strcmp(line, "true") == 0) ? TRUE : FALSE;
            continue;
        }
        if (sscanf(line, "theme_name=%[^\n]", config->theme_name) == 1) continue;
        if (sscanf(line, "icon_theme_name=%[^\n]", config->icon_theme_name) == 1) continue;
        if (sscanf(line, "button_label=%[^\n]", config->button_label) == 1) continue;
        if (sscanf(line, "entry_placeholder=%[^\n]", config->entry_placeholder) == 1) continue;
        if (sscanf(line, "font_size=%d", &config->font_size) == 1) continue;
        if (sscanf(line, "auto_save_interval=%d", &config->auto_save_interval) == 1) continue;
        if (sscanf(line, "enable_logging=%s", line) == 1) {
            config->enable_logging = (strcmp(line, "true") == 0) ? TRUE : FALSE;
            continue;
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    AppConfig config = {0};

    read_config("config_app.txt", &config);

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), config.window_title);
    gtk_window_set_default_size(GTK_WINDOW(window), config.window_width, config.window_height);
    gtk_window_set_resizable(GTK_WINDOW(window), config.window_resizable);

    GtkWidget *button = gtk_button_new_with_label(config.button_label);
    gtk_container_add(GTK_CONTAINER(window), button);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
