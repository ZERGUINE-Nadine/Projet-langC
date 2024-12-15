#ifndef POSTGRES_CLIENT_H
#define POSTGRES_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

void check_connection(PGconn *conn);
int save_user(const char *username, const char *email, const char *password);


#endif