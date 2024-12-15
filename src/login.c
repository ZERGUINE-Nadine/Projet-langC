#include <gtk/gtk.h>
#include <stdio.h>
#include "../includes/postgres_client.h"
#include "../includes/windows_utils.h"


void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);

    GtkEntry *email_entry = GTK_ENTRY(gtk_builder_get_object(builder, "email_entry"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));

    if (!email_entry || !password_entry) {
        g_printerr("Failed to retrieve one or more input fields.\n");
        return;
    }

    const char *email = gtk_entry_get_text(email_entry);
    const char *password = gtk_entry_get_text(password_entry);

    if (strlen(email) == 0 || strlen(password) == 0) {
        set_error_message(builder, "Email and password must not be empty.");
        return;
    }

    if (validate_user(email, password)) {
        g_print("Login successful for user: %s\n", email);
    } else {
        show_success_dialog(builder, "this user does not exist");
    }
}