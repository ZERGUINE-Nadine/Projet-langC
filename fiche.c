#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUESTIONS 100

typedef struct {
    GtkWidget *entry_title;
    GtkWidget *entry_questions[MAX_QUESTIONS];
    GtkWidget *entry_answers[MAX_QUESTIONS];
    GtkWidget *window;
    GtkWidget *vbox;
    int current_questions;
} AppWidgets;

void save_to_file(AppWidgets *app_wdgts) {
    const gchar *title = gtk_entry_get_text(GTK_ENTRY(app_wdgts->entry_title));
    FILE *file = fopen("fiche.txt", "w");

    if (file == NULL) {
        g_print("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(file, "Titre: %s\n\n", title);

    for (int i = 0; i < app_wdgts->current_questions; i++) {
        const gchar *question = gtk_entry_get_text(GTK_ENTRY(app_wdgts->entry_questions[i]));
        const gchar *answer = gtk_entry_get_text(GTK_ENTRY(app_wdgts->entry_answers[i]));
        fprintf(file, "Question %d: %s\n", i + 1, question);
        fprintf(file, "Réponse %d: %s\n\n", i + 1, answer);
    }

    fclose(file);
    g_print("Les données ont été sauvegardées dans 'fiche.txt'.\n");
}

void on_validate_button_clicked(GtkWidget *widget, AppWidgets *app_wdgts) {
    save_to_file(app_wdgts);
}

void on_add_button_clicked(GtkWidget *widget, AppWidgets *app_wdgts) {
    if (app_wdgts->current_questions >= MAX_QUESTIONS) {
        g_print("Nombre maximum de questions atteint.\n");
        return;
    }

    char question_label_text[25];
    snprintf(question_label_text, sizeof(question_label_text), "Question %d:", app_wdgts->current_questions + 1);
    GtkWidget *label_question = gtk_label_new(question_label_text);
    gtk_box_pack_start(GTK_BOX(app_wdgts->vbox), label_question, FALSE, FALSE, 5);

    app_wdgts->entry_questions[app_wdgts->current_questions] = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(app_wdgts->vbox), app_wdgts->entry_questions[app_wdgts->current_questions], FALSE, FALSE, 5);

    char answer_label_text[25];
    snprintf(answer_label_text, sizeof(answer_label_text), "Réponse %d:", app_wdgts->current_questions + 1);
    GtkWidget *label_answer = gtk_label_new(answer_label_text);
    gtk_box_pack_start(GTK_BOX(app_wdgts->vbox), label_answer, FALSE, FALSE, 5);

    app_wdgts->entry_answers[app_wdgts->current_questions] = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(app_wdgts->vbox), app_wdgts->entry_answers[app_wdgts->current_questions], FALSE, FALSE, 5);

    app_wdgts->current_questions++;
    gtk_widget_show_all(app_wdgts->window);
}

void on_delete_button_clicked(GtkWidget *widget, AppWidgets *app_wdgts) {
    if (app_wdgts->current_questions <= 0) {
        g_print("Aucune question à supprimer.\n");
        return;
    }

    app_wdgts->current_questions--;

    gtk_widget_destroy(app_wdgts->entry_questions[app_wdgts->current_questions]);
    gtk_widget_destroy(app_wdgts->entry_answers[app_wdgts->current_questions]);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    AppWidgets *app_wdgts = g_slice_new(AppWidgets);
    app_wdgts->current_questions = 0;

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Créer une Fiche de Révision");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 800);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    app_wdgts->window = window;

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    app_wdgts->vbox = vbox;

    GtkWidget *label_title = gtk_label_new("Titre:");
    gtk_box_pack_start(GTK_BOX(vbox), label_title, FALSE, FALSE, 5);

    app_wdgts->entry_title = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), app_wdgts->entry_title, FALSE, FALSE, 5);

    for (int i = 0; i < 10; i++) {
        on_add_button_clicked(NULL, app_wdgts);  // Ajouter initialement 10 questions
    }

    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 5);

    GtkWidget *validate_button = gtk_button_new_with_label("Valider");
    gtk_box_pack_start(GTK_BOX(button_box), validate_button, TRUE, TRUE, 5);
    g_signal_connect(validate_button, "clicked", G_CALLBACK(on_validate_button_clicked), app_wdgts);

    GtkWidget *add_button = gtk_button_new_with_label("Ajouter");
    gtk_box_pack_start(GTK_BOX(button_box), add_button, TRUE, TRUE, 5);
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_button_clicked), app_wdgts);

    GtkWidget *delete_button = gtk_button_new_with_label("Supprimer");
    gtk_box_pack_start(GTK_BOX(button_box), delete_button, TRUE, TRUE, 5);
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), app_wdgts);

    gtk_widget_show_all(window);
    gtk_main();

    g_slice_free(AppWidgets, app_wdgts);
    return 0;
}

