#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "includes/config.h"



void save_fiches_to_file() {
    FILE *file = fopen("mes_fiches/fiches.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < fiche_count; i++) {
            fprintf(file, "Fiche: %s\n", fiches[i].titre);
            for (int j = 0; j < MAX_QUESTIONS; j++) {
                if (fiches[i].questions[j].question != NULL && fiches[i].questions[j].reponse != NULL) {
                    fprintf(file, "Q%d: %s\n", j + 1, fiches[i].questions[j].question);
                    fprintf(file, "R%d: %s\n", j + 1, fiches[i].questions[j].reponse);
                }
            }
        }
        fclose(file);
    }
}

void load_fiches_from_file() {
    FILE *file = fopen("mes_fiches/fiches.txt", "r");
    if (file != NULL) {
        char line[256];
        fiche_count = 0;
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "Fiche: ", 7) == 0) {
                fiches[fiche_count].titre = g_strdup(line + 7);
                fiches[fiche_count].titre[strcspn(fiches[fiche_count].titre, "\n")] = '\0';
                for (int j = 0; j < MAX_QUESTIONS; j++) {
                    if (fgets(line, sizeof(line), file) && strncmp(line, "Q", 1) == 0) {
                        fiches[fiche_count].questions[j].question = g_strdup(line + 3);
                        fiches[fiche_count].questions[j].question[strcspn(fiches[fiche_count].questions[j].question, "\n")] = '\0';
                        fgets(line, sizeof(line), file);
                        fiches[fiche_count].questions[j].reponse = g_strdup(line + 3);
                        fiches[fiche_count].questions[j].reponse[strcspn(fiches[fiche_count].questions[j].reponse, "\n")] = '\0';
                    } else {
                        fiches[fiche_count].questions[j].question = NULL;
                        fiches[fiche_count].questions[j].reponse = NULL;
                    }
                }
                fiche_count++;
            }
        }
        fclose(file);
    }
}

void create_fiche(GtkWidget *widget, gpointer data) {
    if (fiche_count < MAX_FICHES) {
        fiches[fiche_count].titre = g_strdup("New File");
        for (int i = 0; i < MAX_QUESTIONS; i++) {
            fiches[fiche_count].questions[i].question = NULL;
            fiches[fiche_count].questions[i].reponse = NULL;
        }
        fiche_count++;
        save_fiches_to_file();
    }
    GtkWidget *content_area = GTK_WIDGET(data);
    show_fiches(widget, content_area);
}

void delete_fiche(GtkWidget *widget, gpointer data) {
    int index = GPOINTER_TO_INT(data);
    if (index < fiche_count) {
        g_free(fiches[index].titre);
        for (int i = 0; i < MAX_QUESTIONS; i++) {
            if (fiches[index].questions[i].question != NULL) {
                g_free(fiches[index].questions[i].question);
            }
            if (fiches[index].questions[i].reponse != NULL) {
                g_free(fiches[index].questions[i].reponse);
            }
        }
        for (int i = index; i < fiche_count - 1; i++) {
            fiches[i] = fiches[i + 1];
        }
        fiche_count--;
        save_fiches_to_file();
        GtkWidget *content_area = GTK_WIDGET(gtk_widget_get_parent(gtk_widget_get_parent(widget)));
        show_fiches(NULL, content_area);
    }
}

void modify_fiche_title(GtkWidget *widget, gpointer data) {
    int index = GPOINTER_TO_INT(data);
    const gchar *new_title = gtk_entry_get_text(GTK_ENTRY(widget));
    g_free(fiches[index].titre);
    fiches[index].titre = g_strdup(new_title);
    save_fiches_to_file();
}

void modify_question(GtkWidget *widget, gpointer data) {
    int *indices = (int *)data;
    int fiche_index = indices[0];
    int question_index = indices[1];
    const gchar *new_question = gtk_entry_get_text(GTK_ENTRY(widget));
    g_free(fiches[fiche_index].questions[question_index].question);
    fiches[fiche_index].questions[question_index].question = g_strdup(new_question);
    save_fiches_to_file();
}

void modify_reponse(GtkWidget *widget, gpointer data) {
    int *indices = (int *)data;
    int fiche_index = indices[0];
    int reponse_index = indices[1];
    const gchar *new_reponse = gtk_entry_get_text(GTK_ENTRY(widget));
    g_free(fiches[fiche_index].questions[reponse_index].reponse);
    fiches[fiche_index].questions[reponse_index].reponse = g_strdup(new_reponse);
    save_fiches_to_file();
}

void add_question(GtkWidget *widget, gpointer data) {
    int fiche_index = GPOINTER_TO_INT(data);
    if (fiche_index < fiche_count) {
        for (int i = 0; i < MAX_QUESTIONS; i++) {
            if (fiches[fiche_index].questions[i].question == NULL && fiches[fiche_index].questions[i].reponse == NULL) {
                fiches[fiche_index].questions[i].question = g_strdup("Nouvelle Question");
                fiches[fiche_index].questions[i].reponse = g_strdup("Nouvelle Réponse");
                save_fiches_to_file();
                break;
            }
        }
    }
    GtkWidget *content_area = GTK_WIDGET(gtk_widget_get_parent(widget));
    show_fiches(NULL, content_area);
}

void remove_question(GtkWidget *widget, gpointer data) {
    int *indices = (int *)data;
    int fiche_index = indices[0];
    int question_index = indices[1];
    free(indices);

    if (fiche_index < fiche_count) {
        if (fiches[fiche_index].questions[question_index].question != NULL) {
            g_free(fiches[fiche_index].questions[question_index].question);
            fiches[fiche_index].questions[question_index].question = NULL;
        }
        if (fiches[fiche_index].questions[question_index].reponse != NULL) {
            g_free(fiches[fiche_index].questions[question_index].reponse);
            fiches[fiche_index].questions[question_index].reponse = NULL;
        }
        save_fiches_to_file();
    }

    GtkWidget *content_area = GTK_WIDGET(gtk_widget_get_parent(gtk_widget_get_parent(widget)));
    show_fiches(NULL, content_area);
}

void show_fiches(GtkWidget *widget, gpointer data) {
    GtkWidget *content_area = GTK_WIDGET(data);
    GtkWidget *entry;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *hbox;
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    gtk_container_add(GTK_CONTAINER(content_area), vbox);

    GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    setup_header(header, content_area);
    gtk_box_pack_start(GTK_BOX(vbox), header, FALSE, FALSE, 0);

    for (int i = 0; i < fiche_count; i++) {
        hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), fiches[i].titre);
        gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);
        g_signal_connect(entry, "changed", G_CALLBACK(modify_fiche_title), GINT_TO_POINTER(i));

        button = gtk_button_new_with_label("Delete");
        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
        g_signal_connect(button, "clicked", G_CALLBACK(delete_fiche), GINT_TO_POINTER(i));

        for (int j = 0; j < MAX_QUESTIONS; j++) {
            if (fiches[i].questions[j].question != NULL && fiches[i].questions[j].reponse != NULL) {
                GtkWidget *qhbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
                gtk_box_pack_start(GTK_BOX(vbox), qhbox, FALSE, FALSE, 0);

                label = gtk_label_new("Question :");
                gtk_box_pack_start(GTK_BOX(qhbox), label, FALSE, FALSE, 0);

                entry = gtk_entry_new();
                gtk_entry_set_text(GTK_ENTRY(entry), fiches[i].questions[j].question);
                gtk_box_pack_start(GTK_BOX(qhbox), entry, TRUE, TRUE, 0);
                int *q_indices = malloc(2 * sizeof(int));
                q_indices[0] = i;
                q_indices[1] = j;
                g_signal_connect(entry, "changed", G_CALLBACK(modify_question), q_indices);

                label = gtk_label_new("Réponse :");
                gtk_box_pack_start(GTK_BOX(qhbox), label, FALSE, FALSE, 0);

                entry = gtk_entry_new();
                gtk_entry_set_text(GTK_ENTRY(entry), fiches[i].questions[j].reponse);
                gtk_box_pack_start(GTK_BOX(qhbox), entry, TRUE, TRUE, 0);
                q_indices = malloc(2 * sizeof(int));
                q_indices[0] = i;
                q_indices[1] = j;
                g_signal_connect(entry, "changed", G_CALLBACK(modify_reponse), q_indices);

                button = gtk_button_new_with_label("Supprimer Question");
                gtk_box_pack_start(GTK_BOX(qhbox), button, FALSE, FALSE, 0);
                g_signal_connect(button, "clicked", G_CALLBACK(remove_question), q_indices);
            }
        }

        GtkWidget *add_question_button = gtk_button_new_with_label("Ajouter une question");
        g_signal_connect(add_question_button, "clicked", G_CALLBACK(add_question), GINT_TO_POINTER(i));
        gtk_box_pack_start(GTK_BOX(vbox), add_question_button, FALSE, FALSE, 0);
    }

    GtkWidget *add_fiche_button = gtk_button_new_with_label("Créer une nouvelle fiche");
    g_signal_connect(add_fiche_button, "clicked", G_CALLBACK(create_fiche), content_area);
    gtk_box_pack_start(GTK_BOX(vbox), add_fiche_button, FALSE, FALSE, 0);

    gtk_widget_show_all(content_area);
}
