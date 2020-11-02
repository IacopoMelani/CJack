#include <gtk/gtk.h>

static void activate(GtkApplication *app);
static void create_window(GtkApplication *app);
static void open_play(GtkWidget *play, GtkWidget *menu_window);

static void activate(GtkApplication *app)
{
    create_window(app);
}

static void create_window(GtkApplication *app)
{
    GtkWidget *window;

    GtkWidget *box;

    GtkWidget *layout;

    GtkWidget *play;
    GtkWidget *about;
    GtkWidget *exit;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "test-gtk");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    layout = gtk_layout_new(NULL, NULL);
    gtk_layout_set_size(GTK_LAYOUT(layout), 300, 150);
    gtk_container_add(GTK_CONTAINER(window), layout);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(layout), box);

    gtk_layout_move(GTK_LAYOUT(layout), box, 300, 150);

    play = gtk_button_new_with_label("Play");
    about = gtk_button_new_with_label("About");
    exit = gtk_button_new_with_label("Exit");

    gtk_box_pack_start(GTK_BOX(box), play, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), about, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), exit, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(play), "clicked", G_CALLBACK(open_play), window);

    gtk_widget_show_all(window);
}
static void open_play(GtkWidget *play, GtkWidget *menu)
{
    GtkWidget *window = gtk_application_window_new(gtk_window_get_application(GTK_WINDOW(menu)));

    gtk_widget_destroy(menu);

    gtk_window_set_title(GTK_WINDOW(window), "test-gtk");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.test.gtk", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}