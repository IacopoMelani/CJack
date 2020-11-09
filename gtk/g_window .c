#include "g_window.h"

#include <gtk/gtk.h>

#include "view/g_home.h"

#ifdef DEBUG
#define APP_NAME "CJack-DEBUG"
#else
#define APP_NAME "CJack"
#endif

static void g_window_activate_about(GtkWidget *stack);
static void g_window_activate_home(GtkWidget *stack);
static void g_window_activate_play(GtkWidget *stack);

void g_window_activate(GtkApplication *app)
{
    GtkWidget *window;

    GtkWidget *stack;

    window = gtk_application_window_new(app);

    stack = gtk_stack_new();

    gtk_window_set_title(GTK_WINDOW(window), APP_NAME);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    g_window_activate_about(stack);
    g_window_activate_play(stack);
    g_window_activate_home(stack);

    gtk_container_add(GTK_CONTAINER(window), stack);

    gtk_widget_show_all(window);
}

static void g_window_activate_about(GtkWidget *stack)
{
}

static void g_window_activate_home(GtkWidget *stack)
{
    g_home_activate(stack);
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "home");
}

static void g_window_activate_play(GtkWidget *stack)
{
}
