#include "g_home.h"

static void g_home_destroy();

void g_home_activate(GtkWidget *stack)
{
    GtkWidget *layout;

    GtkWidget *box;

    GtkWidget *play;
    GtkWidget *about;
    GtkWidget *exit;

    layout = gtk_layout_new(NULL, NULL);
    gtk_layout_set_size(GTK_LAYOUT(layout), 300, 150);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(layout), box);
    gtk_layout_move(GTK_LAYOUT(layout), box, 300, 150);

    play = gtk_button_new_with_label("Play");
    about = gtk_button_new_with_label("About");
    exit = gtk_button_new_with_label("Exit");

    gtk_box_pack_start(GTK_BOX(box), play, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), about, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), exit, FALSE, FALSE, 0);

    // g_signal_connect(G_OBJECT(play), "clicked", G_CALLBACK(void), stack);

    gtk_stack_add_named(GTK_STACK(stack), layout, "home");
}

void g_home_destroy(GtkWidget unused *widget)
{
    g_application_quit(gtk_get_window);
}