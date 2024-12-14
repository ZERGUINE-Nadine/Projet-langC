#include <stdio.h>
#include <sqlite3.h>

// Fonction de rappel pour afficher les résultats des requêtes
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// Fonction principale pour afficher le contenu de la base de données
int main() {
    sqlite3 *db;
    char *errMsg = 0;
    int rc;

    // Ouvrir la base de données
    rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        printf("Base de données ouverte avec succès.\n");
    }

    // Afficher le contenu de chaque table
    const char *sql_user = "SELECT * FROM USER;";
    const char *sql_fiche = "SELECT * FROM FICHE;";
    const char *sql_question = "SELECT * FROM QUESTION;";
    const char *sql_answer = "SELECT * FROM ANSWER;";
    const char *sql_exam = "SELECT * FROM EXAM;";
    const char *sql_user_exam = "SELECT * FROM USER_EXAM;";
    const char *sql_user_answer = "SELECT * FROM USER_ANSWER;";

    // Exécuter les requêtes SQL et afficher les résultats
    printf("Contenu de la table USER:\n");
    rc = sqlite3_exec(db, sql_user, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la récupération des données: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    printf("Contenu de la table FICHE:\n");
    rc = sqlite3_exec(db, sql_fiche, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la récupération des données: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    printf("Contenu de la table QUESTION:\n");
    rc = sqlite3_exec(db, sql_question, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la récupération des données: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    printf("Contenu de la table ANSWER:\n");
    rc = sqlite3_exec(db, sql_answer, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la récupération des données: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    printf("Contenu de la table EXAM:\n");
    rc = sqlite3_exec(db, sql_exam, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la récupération des données: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    printf("Contenu de la table USER_EXAM:\n");
    rc = sqlite3_exec(db, sql_user_exam, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la récupération des données: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    printf("Contenu de la table USER_ANSWER:\n");
    rc = sqlite3_exec(db, sql_user_answer, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la récupération des données: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    // Fermer la base de données
    sqlite3_close(db);
    return 0;
}
