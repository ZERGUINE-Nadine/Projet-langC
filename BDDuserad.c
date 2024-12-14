#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

// Fonction de hachage simple du mot de passe (optionnel, pour plus de sécurité)
void hash_password(const char *password, char *hashed_password, size_t size) {
    unsigned int hash = 5381;
    int c;
    while ((c = *password++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    snprintf(hashed_password, size, "%u", hash);
}

// Fonction pour insérer un utilisateur dans la table USER
int insert_user(sqlite3 *db, const char *name, const char *email, const char *password) {
    char *errMsg = 0;
    char sql[512];
    char hashed_password[128];

    // Hachage du mot de passe avant l'insertion (optionnel, améliore la sécurité)
    hash_password(password, hashed_password, sizeof(hashed_password));

    // Préparer la requête d'insertion
    snprintf(sql, sizeof(sql), 
        "INSERT INTO USER (Name, Mail, Pwd) VALUES ('%s', '%s', '%s');", 
        name, email, hashed_password
    );

    // Exécuter la requête SQL
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'insertion de l'utilisateur : %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    }

    printf("Utilisateur ajouté avec succès !\n");
    return 0;
}

int main() {
    sqlite3 *db;
    int rc;
    char name[100];
    char email[100];
    char password[100];

    // Ouvrir la base de données SQLite
    rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    printf("Base de données ouverte avec succès.\n");

    // Demander les informations de l'utilisateur
    printf("\n=== Ajout d'un nouvel utilisateur ===\n");
    printf("Nom d'utilisateur : ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Supprime le caractère de nouvelle ligne

    printf("Email : ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = 0; // Supprime le caractère de nouvelle ligne

    printf("Mot de passe : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0; // Supprime le caractère de nouvelle ligne

    // Vérification des champs vides
    if (strlen(name) == 0 || strlen(email) == 0 || strlen(password) == 0) {
        fprintf(stderr, "Erreur : Tous les champs doivent être remplis.\n");
        sqlite3_close(db);
        return 1;
    }

    // Insertion de l'utilisateur dans la base de données
    int result = insert_user(db, name, email, password);
    if (result != 0) {
        fprintf(stderr, "Erreur lors de l'ajout de l'utilisateur.\n");
    }

    // Fermer la base de données
    sqlite3_close(db);
    return 0;
}
