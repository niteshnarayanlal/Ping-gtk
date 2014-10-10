#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>

int stop_ping_flag = 0;
struct widgets {
	GtkWidget *window;
	GtkWidget *destination_entry;
	GtkWidget *nop_entry;
	GtkWidget *size_entry;
	GtkWidget *start_button;
	GtkWidget *stop_button;
};

void cb_quit(void)
{
	gtk_main_quit();
}

void stop_ping(GtkWidget *button, struct widgets* widgets)
{
	stop_ping_flag = 1;
}
void start_ping(GtkWidget *button, struct widgets* widgets)
{
        gchar *destination;
	gchar *nop;
	gchar *size;
	char cmd[5];
 
	stop_ping_flag = 0;       
	strcpy(cmd, "ping ");
	destination = gtk_entry_get_text(GTK_ENTRY(widgets->destination_entry));
	nop = gtk_entry_get_text(GTK_ENTRY(widgets->nop_entry));
	size = gtk_entry_get_text(GTK_ENTRY(widgets->size_entry));
	printf("\n will ping now\n");
	if (destination != NULL) {
        	printf("\n Destination:%s\n", destination);
		strcat(cmd, destination);
		printf("\n Command:%s\n", cmd);
		/*if ( nop != NULL) {
			strcat(cmd, " -c ");
			strcat(cmd, nop);
		}		
		if ( size != NULL) {
			strcat(cmd, " -s ");
			strcat(cmd, size);
		}*/		
		printf("\n Command:%s\n", cmd);
		if (stop_ping_flag != 1)
		system(cmd);
	}
}

static void get_widgets(struct widgets *widgets, GtkBuilder *builder)
{
	widgets->window = GTK_WIDGET(gtk_builder_get_object(builder,
				    "mainwindow"));
	widgets->destination_entry = GTK_WIDGET(gtk_builder_get_object(builder,
                                		"destination_entry"));
	widgets->nop_entry = GTK_WIDGET(gtk_builder_get_object(builder,
                                	"nop_entry"));
	widgets->size_entry = GTK_WIDGET(gtk_builder_get_object(builder,
                                	"size_entry"));
	widgets->start_button = GTK_WIDGET(gtk_builder_get_object(builder,
                              		   "start_button"));
	widgets->stop_button = GTK_WIDGET(gtk_builder_get_object(builder,
                              		  "stop_button"));
}

int main(int argc, char **argv)
{
	GtkBuilder *builder;
	GError *error = NULL;
	struct widgets *widgets;
	
	gtk_init(&argc, &argv);


	builder = gtk_builder_new();
	if (!gtk_builder_add_from_file(builder, "ping.glade", &error)) {
		g_warning("%s", error->message);
		exit(EXIT_FAILURE);
	}

	widgets = g_slice_new(struct widgets);
	get_widgets(widgets, builder);
	gtk_builder_connect_signals(builder, widgets);
	g_object_unref(G_OBJECT(builder));
	g_signal_connect(G_OBJECT(widgets->start_button), "clicked", G_CALLBACK(start_ping), widgets);
	g_signal_connect(G_OBJECT(widgets->stop_button), "clicked", G_CALLBACK(stop_ping), widgets);
	gtk_widget_show(widgets->window);

	gtk_main();

	g_slice_free(struct widgets, widgets);

	exit(EXIT_SUCCESS);
}
