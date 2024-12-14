#include <stdio.h>
#include <sqlite3.h>

// Fonction pour exécuter une requête SQL
int execute_sql(sqlite3 *db, const char *sql) {
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    return SQLITE_OK;
}

int main() {
    sqlite3 *db;
    int rc;

    // Ouvrir (ou créer si elle n'existe pas) la base de données SQLite
    rc = sqlite3_open("database.db", &db);  // Remplace "database.db" par le nom de ta base de données
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }

    // Créer les tables
    const char *create_user_table = 
    "CREATE TABLE IF NOT EXISTS USER ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "Name TEXT NOT NULL, "
    "Mail TEXT UNIQUE NOT NULL, "
    "Pwd TEXT NOT NULL, "
    "email_verified BOOLEAN DEFAULT FALSE, "
    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
    ");";
    
    rc = execute_sql(db, create_user_table);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    const char *create_fiche_table = 
    "CREATE TABLE IF NOT EXISTS FICHE ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "Title TEXT NOT NULL, "
    "Date DATETIME DEFAULT CURRENT_TIMESTAMP, "
    "ID_creator INTEGER NOT NULL, "
    "FOREIGN KEY (ID_creator) REFERENCES USER(ID) ON DELETE CASCADE"
    ");";
    
    rc = execute_sql(db, create_fiche_table);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    const char *create_question_table = 
    "CREATE TABLE IF NOT EXISTS QUESTION ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "Fiche_ID INTEGER NOT NULL, "
    "Text TEXT NOT NULL, "
    "`Order` INTEGER DEFAULT 0, "
    "FOREIGN KEY (Fiche_ID) REFERENCES FICHE(ID) ON DELETE CASCADE"
    ");";
    
    rc = execute_sql(db, create_question_table);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    const char *create_answer_table = 
    "CREATE TABLE IF NOT EXISTS ANSWER ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "Question_ID INTEGER NOT NULL, "
    "Text TEXT NOT NULL, "
    "FOREIGN KEY (Question_ID) REFERENCES QUESTION(ID) ON DELETE CASCADE"
    ");";
    
    rc = execute_sql(db, create_answer_table);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    const char *create_exam_table = 
    "CREATE TABLE IF NOT EXISTS EXAM ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "Title TEXT NOT NULL, "
    "Date DATETIME DEFAULT CURRENT_TIMESTAMP"
    ");";
    
    rc = execute_sql(db, create_exam_table);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    const char *create_user_exam_table = 
    "CREATE TABLE IF NOT EXISTS USER_EXAM ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "User_ID INTEGER NOT NULL, "
    "Exam_ID INTEGER NOT NULL, "
    "Score FLOAT, "
    "Date DATETIME DEFAULT CURRENT_TIMESTAMP, "
    "FOREIGN KEY (User_ID) REFERENCES USER(ID) ON DELETE CASCADE, "
    "FOREIGN KEY (Exam_ID) REFERENCES EXAM(ID) ON DELETE CASCADE"
    ");";
    
    rc = execute_sql(db, create_user_exam_table);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    const char *create_user_answer_table = 
    "CREATE TABLE IF NOT EXISTS USER_ANSWER ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "User_ID INTEGER NOT NULL, "
    "Exam_ID INTEGER NOT NULL, "
    "Question_ID INTEGER NOT NULL, "
    "Answer TEXT NOT NULL, "
    "FOREIGN KEY (User_ID) REFERENCES USER(ID) ON DELETE CASCADE, "
    "FOREIGN KEY (Exam_ID) REFERENCES EXAM(ID) ON DELETE CASCADE, "
    "FOREIGN KEY (Question_ID) REFERENCES QUESTION(ID) ON DELETE CASCADE"
    ");";
    
    rc = execute_sql(db, create_user_answer_table);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    printf("Tables successfully created in SQLite database.\n");

    // Fermer la base de données
    sqlite3_close(db);
    
    return 0;
}
