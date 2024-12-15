#include "../includes/register.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include "../includes/postgres_client.h"
#include "../includes/windows_utils.h"


void set_error_message(GtkBuilder *builder, const char *message) {
    GtkLabel *error_label = GTK_LABEL(gtk_builder_get_object(builder, "error_label"));
    if (!error_label) {
        g_printerr("failed to find error_label.\n");
        return;
    }
    gtk_label_set_text(error_label, message);
}



void on_register_button_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);

    GtkEntry *username_entry = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry"));
    GtkEntry *email_entry = GTK_ENTRY(gtk_builder_get_object(builder, "email_entry"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));
    GtkEntry *confirm_password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "confirm_password_entry"));

    if (!username_entry || !email_entry || !password_entry || !confirm_password_entry) {
        g_printerr("Failed to retrieve one or more input fields.\n");
        return;
    }

    const char *username = gtk_entry_get_text(username_entry);
    const char *email = gtk_entry_get_text(email_entry);
    const char *password = gtk_entry_get_text(password_entry);
    const char *confirm_password = gtk_entry_get_text(confirm_password_entry);


    if (strlen(username) == 0 && strlen(email) == 0 && strlen(password) == 0 && strlen(confirm_password) == 0) {
        g_print("Error: All fields are empty. Please fill out the form.\n");
        set_error_message(builder, "all fields must not be empty , retry");
        return;
    }

   if (strlen(password) < 6) {
        g_print("Error: Password must be at least 6 characters long.\n");
        set_error_message(builder, "Password must be at least 6 characters long");
        return;
    }

    if (strcmp(password, confirm_password) != 0) {
        g_print("Error: Password and Confirm Password do not match.\n");
        set_error_message(builder, "Password and Confirm Password do not match");
        return;
    }

    if (!save_user(username, email, password)) {
        set_error_message(builder, "Failed to save user to the database.");
        return;
    }

    show_success_dialog(builder, "Registration successful! Please log in.");

    
    
}