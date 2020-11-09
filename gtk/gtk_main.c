#include <gtk/gtk.h>

#include "g_window.h"
#include "../utils/utils.h"

static void create_window(GtkApplication *app);

// static void create_window(GtkApplication *app)
// {
//     GtkWidget *window;

//     GtkWidget *stack;

//     GtkWidget *layout;

//     GtkWidget *box;

//     GtkWidget *play;
//     GtkWidget *about;
//     GtkWidget *exit;

//     GtkWidget *layoutPlay;
//     GtkWidget *boxPlay;

//     GtkWidget *exitPlay;

//     window = gtk_application_window_new(app);

//     stack = gtk_stack_new();

//     gtk_window_set_title(GTK_WINDOW(window), "test-gtk");
//     gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
//     gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

//     layout = gtk_layout_new(NULL, NULL);
//     gtk_layout_set_size(GTK_LAYOUT(layout), 300, 150);

//     box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
//     gtk_container_add(GTK_CONTAINER(layout), box);
//     gtk_layout_move(GTK_LAYOUT(layout), box, 300, 150);

//     play = gtk_button_new_with_label("Play");
//     about = gtk_button_new_with_label("About");
//     exit = gtk_button_new_with_label("Exit");

//     gtk_box_pack_start(GTK_BOX(box), play, FALSE, FALSE, 0);
//     gtk_box_pack_start(GTK_BOX(box), about, FALSE, FALSE, 0);
//     gtk_box_pack_start(GTK_BOX(box), exit, FALSE, FALSE, 0);

//     g_signal_connect(G_OBJECT(play), "clicked", G_CALLBACK(open_play), stack);

//     gtk_stack_add_named(GTK_STACK(stack), layout, "home");

//     layoutPlay = gtk_layout_new(NULL, NULL);
//     gtk_layout_set_size(GTK_LAYOUT(layoutPlay), 300, 150);

//     boxPlay = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
//     gtk_container_add(GTK_CONTAINER(layoutPlay), boxPlay);
//     gtk_layout_move(GTK_LAYOUT(layoutPlay), boxPlay, 300, 150);

//     exitPlay = gtk_button_new_with_label("Play");

//     gtk_box_pack_start(GTK_BOX(boxPlay), exitPlay, FALSE, FALSE, 0);

//     gtk_stack_add_named(GTK_STACK(stack), layoutPlay, "play");

//     gtk_container_add(GTK_CONTAINER(window), stack);

//     gtk_widget_show_all(window);
// }

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.test.gtk", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(g_window_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}