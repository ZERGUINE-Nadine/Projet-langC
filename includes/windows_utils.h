#ifndef WINDOWS_UTILS_H
#define WINDOWS_UTILS_H


void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data);
int load_window(const char *glade_file_path, const char *window_id);
void show_success_dialog(GtkBuilder *builder, const char *message);

#endif WINDOWS_UTILS_H