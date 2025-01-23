#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "includes/config.h"

void start_exam(GtkWidget *widget, gpointer data);
void check_answer(GtkWidget *widget, gpointer data);
void show_exam(GtkWidget *widget, gpointer data);

typedef struct {
    int fiche_index;
    int question_index;
    GtkWidget *entry;
} AnswerData;

int current_question_index = 0;
int correct_answers = 0;
GList *answer_data_list = NULL;

void clear_container(GtkWidget *container) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(container));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        if (GTK_IS_WIDGET(iter->data)) {
            gtk_widget_destroy(GTK_WIDGET(iter->data));
        }
    }
    g_list_free(children);
}


void display_question(GtkWidget *content_area, int fiche_index) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    clear_container(content_area);

    gtk_container_add(GTK_CONTAINER(content_area), vbox);

    GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    setup_header(header, content_area);
    gtk_box_pack_start(GTK_BOX(vbox), header, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new("Question :");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    const char *question = fiches[fiche_index].questions[current_question_index].question;
    if (question != NULL) {
        label = gtk_label_new(question);
        gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

        GtkWidget *entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Votre réponse ici...");
        gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

        GtkWidget *button = gtk_button_new_with_label("Valider");
        g_signal_connect(button, "clicked", G_CALLBACK(check_answer), GINT_TO_POINTER(fiche_index));
        gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

        AnswerData *answer_data = g_malloc(sizeof(AnswerData));
        answer_data->fiche_index = fiche_index;
        answer_data->question_index = current_question_index;
        answer_data->entry = entry;
        answer_data_list = g_list_append(answer_data_list, answer_data);
    } else {
        label = gtk_label_new("Pas de question disponible.");
        gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    }

    gtk_widget_show_all(content_area);
}


void show_exam(GtkWidget *widget, gpointer data) {
    GtkWidget *content_area = GTK_WIDGET(data);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    clear_container(content_area);
    gtk_container_add(GTK_CONTAINER(content_area), vbox);

    GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    setup_header(header, content_area);
    gtk_box_pack_start(GTK_BOX(vbox), header, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new("Examen - Commencez votre test");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    if (fiche_count == 0) {
        label = gtk_label_new("Aucune fiche disponible.");
    } else {
        label = gtk_label_new("Sélectionnez une fiche pour l'examen");
        for (int i = 0; i < fiche_count; i++) {
            GtkWidget *button = gtk_button_new_with_label(fiches[i].titre);
            g_signal_connect(button, "clicked", G_CALLBACK(start_exam), GINT_TO_POINTER(i));
            gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
        }
    }
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    gtk_widget_show_all(content_area);
}

void start_exam(GtkWidget *widget, gpointer data) {
    int fiche_index = GPOINTER_TO_INT(data);
    current_question_index = 0;
    correct_answers = 0;
    g_list_free_full(answer_data_list, g_free);
    answer_data_list = NULL;

    GtkWidget *content_area = gtk_widget_get_toplevel(widget);
    display_question(content_area, fiche_index);
}

void check_answer(GtkWidget *widget, gpointer data) {
    int fiche_index = GPOINTER_TO_INT(data);

    if (fiche_index < 0 || fiche_index >= fiche_count) {
        g_warning("Invalid fiche index: %d", fiche_index);
        return;
    }

    GList *l;
    for (l = answer_data_list; l != NULL; l = l->next) {
        AnswerData *answer_data = (AnswerData *)l->data;
        if (GTK_IS_ENTRY(answer_data->entry)) {
            const gchar *user_answer = gtk_entry_get_text(GTK_ENTRY(answer_data->entry));
            const gchar *correct_answer = fiches[answer_data->fiche_index].questions[answer_data->question_index].reponse;

            if (strlen(user_answer) == 0) {
                GtkWidget *dialog = gtk_message_dialog_new(
                    GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_WARNING,
                    GTK_BUTTONS_CLOSE,
                    "Veuillez saisir une réponse avant de continuer."
                );
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                return;
            }

            gchar *trimmed_user_answer = g_strstrip(g_strdup(user_answer));
            gchar *trimmed_correct_answer = g_strstrip(g_strdup(correct_answer));

            if (g_strcmp0(trimmed_user_answer, trimmed_correct_answer) == 0) {
                correct_answers++;
            }

            g_free(trimmed_user_answer);
            g_free(trimmed_correct_answer);
        }
    }

    current_question_index++;

    GtkWidget *content_area = gtk_widget_get_toplevel(widget);
    if (current_question_index < MAX_QUESTIONS &&
        fiches[fiche_index].questions[current_question_index].question != NULL) {
        display_question(content_area, fiche_index);
    } else {
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        clear_container(content_area);
        gtk_container_add(GTK_CONTAINER(content_area), vbox);

        GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        setup_header(header, content_area);
        gtk_box_pack_start(GTK_BOX(vbox), header, FALSE, FALSE, 0);

        char score_text[50];
        snprintf(score_text, 50, "Examen terminé. Votre score : %d/%d", correct_answers, current_question_index);
        GtkWidget *label = gtk_label_new(score_text);
        gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

        GtkWidget *button = gtk_button_new_with_label("Retour à la sélection de fiche");
        g_signal_connect(button, "clicked", G_CALLBACK(show_exam), content_area);
        gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

        gtk_widget_show_all(content_area);
    }
}
