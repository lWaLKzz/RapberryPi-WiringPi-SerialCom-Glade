#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdint.h>
#include <gtk/gtk.h>


GtkBuilder      *builder; 
GtkWidget       *window;
GtkLabel		*label;
GtkEntry		*entry;
//VARİABLES
int fd;

int variable;
int append[1000];
char readCom[1000];
int queue = 0;
int queue2 = 0;
//Function
void setup(void);
gboolean time_handler(GtkLabel *label);

//Thread
PI_THREAD (myThread)
{
	while(1){
		
		variable = serialGetchar(fd);
		
		
		if(variable==60){
			queue2 = queue;
		}
		if(variable != 60 && variable != 62 && variable != -1){
			
			append[queue] = variable;
			queue++;
		}
		
	}
		

}

int main(int argc, char *argv[])
{	
	setup();
    

    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "com.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "win"));
    label = GTK_LABEL(gtk_builder_get_object(builder, "read"));
    entry = GTK_ENTRY(gtk_builder_get_object(builder, "en"));
    gtk_builder_connect_signals(builder, NULL);
	
    g_object_unref(builder);
 
    gtk_widget_show(window);
    g_timeout_add(500, (GSourceFunc) time_handler,label);           
    gtk_main();
 
    
    return 0;
}
gboolean time_handler(GtkLabel *label) {
	char set[50];
	gtk_label_set_text(label, readCom);
	sprintf(set,"%s\r",gtk_entry_get_text(entry));
	
	serialPrintf(fd,set);
	
	int index = 0;
	for(int i=queue2;i<queue;i++){
		if(append[i]!=0){
			sprintf(&readCom[index], "%c", append[i]);
			index++;
		}
	}
	
	
	
	
    return TRUE;
}

void setup(void){
	wiringPiSetup();
	fd = serialOpen("/dev/ttyS0",9600);
	piThreadCreate(myThread);

}



