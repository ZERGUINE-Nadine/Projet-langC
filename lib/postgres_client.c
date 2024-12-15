#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include "../includes/postgres_client.h"


PGconn *connect_to_db() {
    const char *conninfo = "dbname=flashbox user=psql password=psql host=localhost port=5432";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    printf("Connected to database successfully.\n");
    return conn;
}

int save_user(const char *username, const char *email, const char *password) {
    PGconn *conn = connect_to_db();
    if (!conn) {
        return 0; 
    }

    const char *query = "INSERT INTO users (username, email, password) VALUES ($1, $2, $3)";
    const char *paramValues[3] = {username, email, password};

    PGresult *res = PQexecParams(
        conn,                
        query,               
        3,                  
        NULL,               
        paramValues,         
        NULL,                
        NULL,                
        0                    
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Insert user failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return 0; 
    }

    printf("User saved successfully.\n");
    PQclear(res);
    PQfinish(conn);
    return 1; 
}

int validate_user(const char *email, const char *password) {
    PGconn *conn = connect_to_db();
    if (!conn) {
        return 0; 
    }

    const char *query = "SELECT COUNT(*) FROM users WHERE email = $1 AND password = $2";
    const char *paramValues[2] = {email, password};

    PGresult *res = PQexecParams(
        conn,                
        query,               
        2,                  
        NULL,               
        paramValues,         
        NULL,                
        NULL,                
        0                    
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return 0;
    }

    int user_exists = atoi(PQgetvalue(res, 0, 0));  

    PQclear(res);
    PQfinish(conn);

    return user_exists > 0;
}
