#include "joystick.h"
#include "MIDI_functions.c"
#include <gtk/gtk.h>

snd_seq_t* handle;
int in_id, out_id;


//As variáveis abaixo guardam informações sobre os botões da manete
//button[x] == 0 <--> o botão x não está pressionado
int button[12];
//axis[x] = valor da posição do eixo x (esse eixo pode ser horizontal ou vertical)
int axis[6];

char note[8];

int base = 60;

int menor = 0;

long int aux;

int note_vel = 65;

int mark = 0;

void joystick_callback(t_mosaic_button_event *event){
if(event->type == JS_EVENT_BUTTON){
//event->id identifica o botão, event->value diz se ele foi pressinado (1) ou solto (0)		
		button[event->id] = event->value;
//Este switch mapeia os botões da manete (excluindo o D-Pad)
//Esse mapeamento foi feito para a manete B-MAX (os valores variam de uma para a outra)		
		switch(event->id){
			case 0:
				//Triângulo
				if(event->value){
					//printf("SQUARE PRESSED\n");
					if(!menor){
						send_note(note_vel, base + 9, 0, handle, out_id);
						note[3] = base + 9;
					}else{
						send_note(note_vel, base + 8, 0, handle, out_id);
						note[3] = base + 8;
					}
				}else{
					//printf("SQUARE RELEASED\n");
					send_note(0, note[3], 0, handle, out_id);
				}
			break;
			case 1:
				//Bolinha
				if(event->value){
					//printf("TRIANGLE PRESSED\n");
					send_note(note_vel, base + 12, 0, handle, out_id);
					note[0] = base + 12;
				}else{
					//printf("TRIANGLE RELEASED\n");
					send_note(0, note[0], 0, handle, out_id);
				}
			break;
			case 2:
				//Xis
				if(event->value){
					//printf("CIRCLE PRESSED\n");
					if(!menor){
						send_note(note_vel, base + 11, 0, handle, out_id);
						note[1] = base + 11;
					}else{
						send_note(note_vel, base + 10, 0, handle, out_id);
						note[1] = base + 10;
					}
				}else{
					//printf("CIRCLE RELEASED\n");
					send_note(0, note[1], 0, handle, out_id);
				}
			break;
			case 3:
				//Quadrado
				if(event->value){
					//printf("X PRESSED\n");
					send_note(note_vel, base + 7, 0, handle, out_id);
					note[2] = base + 7;
				}else{
					//printf("X RELEASED\n");
					send_note(0, note[2], 0, handle, out_id);
				}
			break;
			case 4:
				//L2			
				if(event->value){
					//printf("L2 PRESSED\n");
					if(!menor){
						send_note(note_vel, base + 4, 0, handle, out_id);
						note[4] = base + 4;
					}else{
						send_note(note_vel, base + 3, 0, handle, out_id);
						note[4] = base + 3;
					}
				}else{
					//printf("L2 RELEASED\n");
					send_note(0, note[4], 0, handle, out_id);
				}
			break;
			case 5:
				//R2
				if(event->value){
					//printf("R2 PRESSED\n");
					send_note(note_vel, base + 5, 0, handle, out_id);
					note[5] = base + 5;
				}else{
					//printf("R2 RELEASED\n");
					send_note(0, note[5], 0, handle, out_id);
				}
			break;
			case 6:
				//L1
				if(event->value){
					//printf("L1 PRESSED\n");
					send_note(note_vel, base, 0, handle, out_id);
					note[6] = base;
				}else{
					//printf("L1 RELEASED\n");
					send_note(0, note[6], 0, handle, out_id);
				}
			break;
			case 7:
				//R1
				if(event->value){
					//printf("R1 PRESSED\n");
					send_note(note_vel, base + 2, 0, handle, out_id);
					note[7] = base + 2;
				}else{
					//printf("R1 RELEASED\n");
					send_note(0, note[7], 0, handle, out_id);
				}
			break;
			case 8:
				//Select			
				if(event->value){
					//printf("SELECT PRESSED\n");
					if(menor == 0) menor = 1;
					else menor = 0;
				}else{
					//printf("SELECT RELEASED\n");
				}
			break;
			case 9:
				//Start
				//PANIC BUTTON				
				if(event->value == 1){
					//printf("START PRESSED\n");
					int c;
					for(c=0; c<128; c++){
						send_note(0, c, 0, handle, out_id);
					}
				}else{
					//printf("START RELEASED\n");
				}
			break;
			case 10:
				//L3
				if(event->value){
					//printf("L3 PRESSED\n");
					base -= 12;
					printf("Base atual = %d\n", base);
				}else{
					//printf("L3 RELEASED\n");
				}
			break;
			case 11:
				//R3
				if(event->value){					
					//printf("R3 PRESSED\n");
					base += 12;
					printf("Base atual = %d\n", base);
					
				}else{
					//printf("R3 RELEASED\n");
				}
			break;
		}
	}else if(event->type == JS_EVENT_AXIS){
//Nesse caso, event->value guarda a posição do eixo		
		axis[event->id] == event->value;
//Este switch mapeia as alavancas e o D-PAD
		switch(event->id){
		case 0:
//Para eixos horizontais, os valores crescem da esquerda para a direita, com o 0 com centro
			aux = event->value;
			aux += 32767;
			aux = (((double)aux / 65534.0) * 127);
			send_control(1, (int)aux, 0, handle, out_id);
			if(event->value > 0){
				//printf("AXIS 2 DOWN\n");
			}else if(event->value < 0){
				//printf("AXIS 2 UP\n");
			}else{
				//printf("AXIS 2 NEUTRAL\n");
			}
		break;
		case 1:
//Para eixoos verticais, os valores crescem de cima para baixo, com o 0 como centro
			aux = event->value;
			aux = (((double)aux / 32767) * 8191);
			send_pitchbend(3, (int)aux, 0, handle, out_id);
			//printf("C3= %d\n", (int)aux);
			if(event->value > 0){
				//printf("AXIS 3 DOWN\n"); 
			}else if(event->value < 0){
				//printf("AXIS 3 UP\n");
			}else{
				//printf("AXIS 3 NEUTRAL\n");
			}
		break;
		case 2:
			aux += 32767;
			aux = (((double)aux / 65534.0) * 97);
			note_vel = (aux + 30);
			if(event->value > 0){
				//printf("AXIS 0 RIGHT\n");
			}else if(event->value < 0){
				//printf("AXIS 0 LEFT\n");
			}else{
				//printf("AXIS 0 NEUTRAL\n");
			}
		break;
		case 3:
			//L3 Vertical
			aux = event->value;
			aux += 32767;
			aux = (((double)aux / 65534.0) * 127);
			send_control(0, (int)aux, 0, handle, out_id);
			if(event->value > 0){
				//printf("AXIS 1 DOWN\n");
			}else if(event->value < 0){
				//printf("AXIS 1 UP\n");
			}else{
				//printf("AXIS 1 NEUTRAL\n");
			}
		break;
		case 4:
			//D-Pad Horizontal
			if(event->value > 0){
				//printf("AXIS 4 RIGHT\n");
			}else if(event->value < 0){
				//printf("AXIS 4 LEFT\n");
			}else{
				//printf("AXIS 4 NEUTRAL\n");
			}
		break;
		break;
		case 5:
			//D-pad Vertical
			if(event->value > 0){
				//printf("AXIS 5 DOWN\n");
				base--;
				mark = 1;
			}else if(event->value < 0){
				//printf("AXIS 5 UP\n");
				base++;
				mark = 2;
			}else{
				//printf("AXIS 5 NEUTRAL\n");
				if(mark == 1) base ++;
				else if(mark == 2) base --; 
			}
		break;
		}
	}
}

char* int_to_string(int num){
	int x, i, j;
	char* string;
	x = num;
	for(i = 0; x; i++){
		x /= 10;
	}
	string = (char*)malloc((i)*sizeof(char));
	string[i--] = '\0';
	for(; i >= 0; i--){
		string[i] = (num % 10) + '0';
		num /= 10;
	}
	return string;
}

GtkWidget* pack_box(char* text, char* value){
	GtkWidget* box;
	GtkWidget* label;
	GtkWidget* entry;

	box = gtk_hbox_new(TRUE, 0);
	label = gtk_label_new(text);
	gtk_widget_show(label);
	gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry), value);
	gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
	gtk_widget_show(entry);
	gtk_box_pack_start(GTK_BOX(box), entry, FALSE, TRUE, 0);
	gtk_widget_show(box);
	return box;
}

static gboolean delete_event(GtkWidget* widget,
				GdkEvent* event,
				gpointer data){
	gtk_main_quit();
	return FALSE;
}

void* gtk_main_thread(){
	gtk_main();
}

void register_callback(char * device,
                char * name,
                char number_of_axes,
                char number_of_buttons,
                int driver_version){
	GtkWidget* window;
	GtkWidget* box;
	GtkWidget* box2;
	GtkWidget* label;
	GtkWidget* entry;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "ControlTunes");
	g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);
	
	box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), box); 
	
	gtk_box_pack_start(GTK_BOX(box), pack_box("Device:", device), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), pack_box("Name:", name), FALSE, FALSE, 0);	
	gtk_box_pack_start(GTK_BOX(box), pack_box("Number of axes", int_to_string((int)number_of_axes)), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), pack_box("Number of buttons", int_to_string((int)number_of_buttons)), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), pack_box("Driver version", int_to_string(driver_version)), FALSE, FALSE, 0);

	gtk_widget_show(box);
	gtk_widget_show(window);

	pthread_t tid;
	pthread_create(&tid, NULL, gtk_main_thread, NULL);
}


int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);
	
	//Abre um cliente com o sequenciador MIDI
	open_client(&handle, &in_id, &out_id);

	int i;
	//inicialização dos vetores do início do código	
	for(i=0; i<12; i++){
		button[i] = 0;
		if(i<6) axis[i] = 0;
		if(i<8) note[i] = -2;
	}

	joystick_inicialize("/dev/input/js0", &joystick_callback, &register_callback);
	while(1) usleep(10000);
	return 0;
}
