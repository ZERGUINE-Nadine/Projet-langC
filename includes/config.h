#ifndef CONFIG_H
#define CONFIG_H 
#define MAX_FICHES 10 
#define MAX_QUESTIONS 10 
#define CONFIG_WINDOW_WIDTH 800 
#define CONFIG_WINDOW_HEIGHT 600 

typedef struct  Question{ 
    char *question;
    char *reponse;
 } Question;
 
typedef struct Fiche{ 
    char *titre; 
    Question questions[MAX_QUESTIONS];
 } Fiche;
 
 
extern Fiche fiches[MAX_FICHES]; 
extern int fiche_count; 


void on_destroy(GtkWidget *widget, gpointer data); 
void on_deconnexion_clicked(GtkWidget *widget, gpointer data); 
void setup_header(GtkWidget *header, GtkWidget *content_area); 
void show_fiches(GtkWidget *widget, gpointer data); 
void show_exam(GtkWidget *widget, gpointer data); 
void load_fiches_from_file(); 
void save_fiches_to_file(); 
#endif
