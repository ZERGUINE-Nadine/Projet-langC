#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define NUM_QUESTIONS 10

typedef struct {
    GtkWidget *entry_questions[NUM_QUESTIONS];
    GtkWidget *entry_answers[NUM_QUESTIONS];
    GtkWidget *file_list;
    GtkWidget *notebook;
} AppData;

void clear_entries(AppData *data) {
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        gtk_entry_set_text(GTK_ENTRY(data->entry_questions[i]), "");
        gtk_entry_set_text(GTK_ENTRY(data->entry_answers[i]), "");
    }
}

void refresh_file_list(AppData *data) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(data->file_list));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    DIR *dir = opendir(".");
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name, "fiche_") && strstr(entry->d_name, ".txt")) {
                gtk_list_box_insert(GTK_LIST_BOX(data->file_list), gtk_label_new(entry->d_name), -1);
            }
        }
        closedir(dir);
    }
    gtk_widget_show_all(GTK_WIDGET(data->file_list));
}

void load_file(GtkWidget *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(data->file_list));
    if (row) {
        GtkWidget *label = gtk_bin_get_child(GTK_BIN(row));
        const char *filename = gtk_label_get_text(GTK_LABEL(label));

        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("Unable to open file");
            return;
        }

        char line[256];
        for (int i = 0; i < NUM_QUESTIONS; i++) {
            if (fgets(line, sizeof(line), file)) {
                gtk_entry_set_text(GTK_ENTRY(data->entry_questions[i]), strchr(line, ':') + 2);
            }
            if (fgets(line, sizeof(line), file)) {
                gtk_entry_set_text(GTK_ENTRY(data->entry_answers[i]), strchr(line, ':') + 2);
            }
        }

        fclose(file);
        gtk_notebook_set_current_page(GTK_NOTEBOOK(data->notebook), 0);
    }
}

void save_file(GtkWidget *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;

    char filename[256];
    snprintf(filename, sizeof(filename), "fiche_%ld.txt", time(NULL));

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Unable to create file");
        return;
    }

    for (int i = 0; i < NUM_QUESTIONS; i++) {
        const char *question = gtk_entry_get_text(GTK_ENTRY(data->entry_questions[i]));
        const char *answer = gtk_entry_get_text(GTK_ENTRY(data->entry_answers[i]));
        fprintf(file, "Q%d: %s\nA%d: %s\n", i + 1, question, i + 1, answer);
    }

    fclose(file);
    refresh_file_list(data);
    clear_entries(data);
}

void delete_file(GtkWidget *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(data->file_list));
    if (row) {
        GtkWidget *label = gtk_bin_get_child(GTK_BIN(row));
        const char *filename = gtk_label_get_text(GTK_LABEL(label));
        if (remove(filename) == 0) {
            gtk_widget_destroy(GTK_WIDGET(row));
        } else {
            perror("Unable to delete file");
        }
    }
}

void setup_page(GtkWidget *notebook, AppData *data) {
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    for (int i = 0; i < NUM_QUESTIONS; i++) {
        char label_text[20];
        snprintf(label_text, sizeof(label_text), "Question %d", i + 1);
        GtkWidget *label_question = gtk_label_new(label_text);
        GtkWidget *entry_question = gtk_entry_new();
        GtkWidget *entry_answer = gtk_entry_new();

        data->entry_questions[i] = entry_question;
        data->entry_answers[i] = entry_answer;

        gtk_grid_attach(GTK_GRID(grid), label_question, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), entry_question, 1, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), entry_answer, 2, i, 1, 1);
    }

    GtkWidget *save_button = gtk_button_new_with_label("Save");
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_file), data);
    gtk_grid_attach(GTK_GRID(grid), save_button, 1, NUM_QUESTIONS, 1, 1);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), grid, gtk_label_new("Create/Modify Form"));
}

void setup_file_list(GtkWidget *notebook, AppData *data) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    data->file_list = gtk_list_box_new();

    GtkWidget *load_button = gtk_button_new_with_label("Load");
    GtkWidget *delete_button = gtk_button_new_with_label("Delete");

    g_signal_connect(load_button, "clicked", G_CALLBACK(load_file), data);
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_file), data);

    gtk_box_pack_start(GTK_BOX(box), data->file_list, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), load_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), delete_button, FALSE, FALSE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box, gtk_label_new("File List"));

    refresh_file_list(data);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    AppData data;
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Fiche App");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *notebook = gtk_notebook_new();
    data.notebook = notebook;

    setup_page(notebook, &data);
    setup_file_list(notebook, &data);

    gtk_container_add(GTK_CONTAINER(window), notebook);
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}
