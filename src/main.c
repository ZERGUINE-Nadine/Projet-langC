#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "../includes/windows_utils.h"
#include "../includes/register.h"
#include "../includes/postgres_client.h"
#include <libpq-fe.h>




void on_main_window_destroy(GtkWidget *widget, gpointer user_data) {
    gtk_main_quit(); 
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    if (!load_window("./ui/register.glade", "register_window")) {
        g_printerr("Failed to initialize the register window.\n");
        return 1;
    }


    gtk_main();


    return EXIT_SUCCESS;
}
