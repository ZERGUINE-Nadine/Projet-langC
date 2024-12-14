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

// Fonction pour afficher la structure d'une table spécifique
void display_table_structure(sqlite3 *db, const char *table_name) {
    char *errMsg = 0;
    char sql[256];
    sqlite3_stmt *stmt;

    printf("\nStructure de la table %s:\n", table_name);
    printf("---------------------------------\n");

    // Prépare la requête PRAGMA pour obtenir les informations sur la structure de la table
    snprintf(sql, sizeof(sql), "PRAGMA table_info(%s);", table_name);
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête pour %s: %s\n", table_name, sqlite3_errmsg(db));
        return;
    }

    printf("| %-15s | %-10s | %-10s | %-10s | %-10s |\n", "Nom", "Type", "NotNull", "Clé primaire", "Par défaut");
    printf("|-----------------|------------|------------|------------|------------|\n");

    // Boucle sur les résultats de PRAGMA table_info
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *col_name = (const char *)sqlite3_column_text(stmt, 1); // Nom de la colonne
        const char *col_type = (const char *)sqlite3_column_text(stmt, 2); // Type de la colonne
        int not_null = sqlite3_column_int(stmt, 3); // 1 si NOT NULL, 0 sinon
        int pk = sqlite3_column_int(stmt, 5); // 1 si colonne fait partie de la clé primaire, 0 sinon
        const char *default_value = (const char *)sqlite3_column_text(stmt, 4); // Valeur par défaut (si existe)

        printf("| %-15s | %-10s | %-10d | %-10d | %-10s |\n", 
            col_name ? col_name : "NULL", 
            col_type ? col_type : "NULL", 
            not_null, 
            pk, 
            default_value ? default_value : "NULL");
    }

    printf("\n");

    sqlite3_finalize(stmt);
}

// Fonction pour récupérer et afficher la liste de toutes les tables de la base de données
void list_and_display_tables(sqlite3 *db) {
    char *errMsg = 0;
    const char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la récupération de la liste des tables: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("Liste des tables dans la base de données:\n");
    printf("------------------------------------------\n");

    // Parcours des noms de table
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *table_name = (const char *)sqlite3_column_text(stmt, 0);
        printf("Table: %s\n", table_name);
        display_table_structure(db, table_name);
    }

    sqlite3_finalize(stmt);
}

int main() {
    sqlite3 *db;
    int rc;

    // Ouvrir la base de données
    rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        printf("Base de données ouverte avec succès.\n");
    }

    // Lister et afficher les tables et leurs structures
    list_and_display_tables(db);

    // Fermer la base de données
    sqlite3_close(db);
    return 0;
}
