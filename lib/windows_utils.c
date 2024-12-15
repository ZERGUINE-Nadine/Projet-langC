#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

int load_window(const char *glade_file_path, const char *window_id) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    if (!gtk_builder_add_from_file(builder, glade_file_path, &error)) {
        g_printerr("Error loading Glade file: %s\n", error->message);
        g_clear_error(&error);
        return 0;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, window_id));
    if (!window) {
        g_printerr("Error: %s not found in Glade file.\n", window_id);
        return 0; 
    }

    gtk_builder_connect_signals(builder, builder);
    gtk_widget_show_all(window);

    return 1;
}

void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);

    gtk_widget_destroy(GTK_WIDGET(dialog));

    GtkWidget *current_window = GTK_WIDGET(gtk_builder_get_object(builder, "register_window"));
    if (!current_window) {
        current_window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    }

    const gchar *current_window_name = gtk_widget_get_name(current_window);
    if (g_strcmp0(current_window_name, "login") == 0) {
        g_print("Current window is login_window. Keeping it open.\n");
        return;
    }

    if (current_window) {
        gtk_widget_destroy(current_window);
    }

    if (!load_window("./ui/login.glade", "login_window")) {
        g_printerr("Failed to load login window.\n");
    }
}


void show_success_dialog(GtkBuilder *builder, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", 
        message
    );

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog_response), builder);
    gtk_widget_show_all(dialog);
}
