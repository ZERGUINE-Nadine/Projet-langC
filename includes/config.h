#ifndef CONFIG_H
#define CONFIG_H

extern int MAX_FICHES;
extern int MAX_QUESTIONS;
extern int CONFIG_WINDOW_WIDTH;
extern int CONFIG_WINDOW_HEIGHT;

typedef struct Question {
    char *question;
    char *reponse;
} Question;

typedef struct Fiche {
    char *titre;
    Question *questions;
} Fiche;

extern Fiche *fiches;
extern int fiche_count;

void on_destroy(GtkWidget *widget, gpointer data);
void on_deconnexion_clicked(GtkWidget *widget, gpointer data);
void setup_header(GtkWidget *header, GtkWidget *content_area);
void show_fiches(GtkWidget *widget, gpointer data);
void show_exam(GtkWidget *widget, gpointer data);
void save_fiches_file();
void load_fiches_from_file() ;
void read_config(const char *filename);

#endif
