#include"MIDI_joystick.h"
#include<string.h>

char** combos; //Matriz de combos reconheciveis
int* entradas; //Vetor que guarda os combos reconhecidos
int base; //Valor base para a escala de notas na manete
int note_vel; //Valor da dinâmica da mensagem MIDI enviada
char** note; //Vetor que guarda as notas enviadas para ser efetuado um NOTEOFF depois

//Variaveis da alavanca direita
int R3_cmd;
int R3toggle;

//Conexões atuais da controle
int ctrl1;
int ctrl2;
int ctrl3;

void joystick_callback(t_mosaic_button_event *msg){
	//Declaração de variáveis	
	int i, s1, s2, aux, port_id, p_atual, p_anterior;
	snd_seq_t* handle;
	//Interpretação da String de Combo	
	i = 0;
	s1 = 0;
	s2 = 0;
	if(msg->combo){
		while(msg->combo[i] != '\0'){
			if(msg->combo[i] == combos[0][s1]){
				s1++;
				if(s1 > 1){
					if(!entradas[1]){
						entradas[0]++;
						entradas[2]--;
						entradas[3]--;
					}
					s1 = 0;			
				}
			}else s1 = 0;
			if(msg->combo[i] == combos[1][s2]){
				s2++;
				if(s2 > 1){
					if(!entradas[0]){
						entradas[1]++;
						entradas[2]--;
						entradas[3]--;
					}
					s2 = 0;
				}
			}else s2 = 0;
			if(msg->combo[i] == combos[2][0]){
				entradas[2]++;
			}
			if(msg->combo[i] == combos[3][0]){
				entradas[3]++;
			}
			if(msg->combo[i] == combos[4][0]){
				entradas[4]++;
			}
			if(msg->combo[i] == combos[5][0]){
				entradas[5]++;
			}
			printf("%d ", msg->combo[i]);
			i++;
		}
		printf("\n");
	}
	//Interpretação do evento
	handle = msg->handle;
	port_id = msg->out_id;
	if(msg->type == JS_EVENT_BUTTON){				
		if(entradas[0]){
			for(i = 0; i < 128; i++){
				msg->combo[i] = 0;
			}	
		}
		while(entradas[0]){
			base += 12;
			entradas[0]--;
			printf("Base: %d\n", base);
		}
		if(entradas[1]){
			for(i = 0; i < 128; i++){
				msg->combo[i] = 0;
			}	
		}
		while(entradas[1]){
			base -= 12;
			entradas[1]--;
			printf("Base: %d\n", base);
		}
		for(i = 0; i < entradas[2]; i++){
			base++;
			printf("Base aumentada em 1 ponto\n");
		}
		for(i = 0; i < entradas[3]; i++){
			base--;
			printf("Base diminuida em 1 ponto\n");
		}
		switch(msg->id){
			case 0:
				//Triângulo				
				if(msg->value){
					send_note(note_vel, base + 9, 0, handle, port_id);
					note[0][0] = base + 9;
				}else{				
					send_note(0, note[0][0], 0, handle, port_id);
				}
			break;
			case 1:
				//Bolinha
				if(msg->value){
					send_note(note_vel, base + 12, 0, handle, port_id);
					note[1][0] = base + 12;
				}else{
					send_note(0, note[1][0], 0, handle, port_id);
				}
			break;
			case 2:
				//Xis					
				if(msg->value){
					send_note(note_vel, base + 11, 0, handle, port_id);
					note[2][0] = base + 11;
				}else{
					send_note(0, note[2][0], 0, handle, port_id);
				}
			break;
			case 3:
				//Quadrado
				if(msg->value){
					send_note(note_vel, base + 7, 0, handle, port_id);
					note[3][0] = base + 7;
				}else{
					send_note(0, note[3][0], 0, handle, port_id);
				}
			break;
			case 4:
				//L2
				if(msg->value){
					send_note(note_vel, base, 0, handle, port_id);
					note[4][0] = base;
				}else{
					send_note(0, note[4][0], 0, handle, port_id);
				}
			break;
			case 5:
				//R2
				if(msg->value){
					send_note(note_vel, base + 2, 0, handle, port_id);
					note[5][0] = base + 2;
				}else{
					send_note(0, note[5][0], 0, handle, port_id);
				}
			break;
			case 6:
				//L1
				if(msg->value){
					send_note(note_vel, base + 4, 0, handle, port_id);
					note[6][0] = base + 4;
				}else{
					send_note(0, note[6][0], 0, handle, port_id);
				}
			break;
			case 7:
				//R1
				if(msg->value){
					send_note(note_vel, base + 5, 0, handle, port_id);
					note[7][0] = base + 5;
				}else{
					send_note(0, note[7][0], 0, handle, port_id);
				}
			break;
			case 8:
				//Select
				//PANIC BUTTON				
				if(msg->value == 1){
					int c;
					for(c=0; c<128; c++){
						send_note(0, c, 0, handle, port_id);
					}
				}else{

				}
			break;
			case 9:
				//Start
				if(msg->value == 1){

				}else{

				}
			break;
			case 10:
				//L3
				if(msg->value == 1){
					
				}else{

				}
			break;
			case 11:
				//R3
				if(msg->value == 1){
					if(R3toggle == 0) R3toggle = 1;
					else R3toggle = 0;
				}else{

				}
			break;
		}
	}else if(msg->type == JS_EVENT_AXIS){
		switch(msg->id){
			case 0:
			//Para eixos horizontais, os valores crescem da esquerda para a direita, com o 0 com centro			
				//L3 Horizontal	
				if(msg->value > 0){
					//Direita
				}else if(msg->value < 0){
					//Esquerda
				}else{
					//Neutro
				}
			break;
			case 1:
			//Para eixos verticais, os valores crescem de cima para baixo, com o 0 como centro
				//L3 Vertical
				if(msg->value > 0){
					//Baixo
				}else if(msg->value < 0){
					//Cima
				}else{
					//Neutro
				}
			break;
			case 2:
				//R3 Horizontal
				if(!R3toggle){
					//Seleção de controlador para o eixo horizontal
					aux = -1;
					while(entradas[2]){
						aux += 4;
						entradas[2]--;					
					}
					while(entradas[3]){
						aux += 2;
						entradas[3]--;					
					}
					while(entradas[4]){
						aux += 1;
						entradas[4]--;					
					}
					while(entradas[5]){
						aux += 3;
						entradas[5]--;					
					}
					if(aux != -1){
						ctrl2 = aux;
						printf("Horizontal controlando %d\n", ctrl2);
					}
					//______________________________________________
					//Este eixo envia uma mensagem MIDI de controle
					aux = msg->value;
					aux += 32767;
					aux = (((double)aux / 65534.0) * 127);
					send_control(ctrl2, (int)aux, 0, handle, port_id);
					//______________________________________________
				}else{
					//Seleção de controlador para a rotação					
					aux = -1;
					while(entradas[2]){
						aux += 4;
						entradas[2]--;					
					}
					while(entradas[3]){
						aux += 2;
						entradas[3]--;					
					}
					while(entradas[4]){
						aux += 1;
						entradas[4]--;					
					}
					while(entradas[5]){
						aux += 3;
						entradas[5]--;					
					}
					if(aux != -1){
						ctrl3 = aux;
						printf("Rotação controlando %d\n", ctrl3);
					}
					//___________________________________________
					if(msg->axis[3] == 32767){
						p_atual = msg->value + 32767;
						p_atual = (((double)p_atual / 65534.0) * 4);
						p_anterior = msg->axis[2] + 32767;
						p_anterior = (((double)p_anterior / 65534.0) * 4);
						if(p_atual > p_anterior) R3_cmd--;
						if(p_atual < p_anterior) R3_cmd++;
					}
					if(msg->axis[3] == -32767){
						p_atual = msg->value + 32767;
						p_atual = (((double)p_atual / 65534.0) * 4);
						p_anterior = msg->axis[2] + 32767;
						p_anterior = (((double)p_anterior / 65534.0) * 4);
						if(p_atual > p_anterior) R3_cmd++;
						if(p_atual < p_anterior) R3_cmd--;
					}
					if(R3_cmd > 127) R3_cmd = 127;
					if(R3_cmd < 0) R3_cmd = 0;
					send_control(ctrl3, R3_cmd, 0, handle, port_id);
				}
				if(msg->value > 0){
					//Direita
				}else if(msg->value < 0){
					//Esquerda
				}else{
					//Neutro
				}
			break;
			case 3:
				//R3 Vertical
				if(!R3toggle){
					//Seleção de controlador para o eixo vertical
					aux = -1;
					while(entradas[2]){
						aux += 4;
						entradas[2]--;					
					}
					while(entradas[3]){
						aux += 2;
						entradas[3]--;					
					}
					while(entradas[4]){
						aux += 1;
						entradas[4]--;					
					}
					while(entradas[5]){
						aux += 3;
						entradas[5]--;					
					}
					if(aux != -1){
						ctrl1 = aux;
						printf("Vertical controlando %d\n", ctrl1);
					}
					//________________________________________________
					//Este exio envia uma mensagem MIDI de Pitchbend
					aux = msg->value;
					aux += 32767;
					aux = (((double)aux / 65534.0) * 127);
					send_control(ctrl1, (int)aux, 0, handle, port_id);
					//_______________________________________________
				}else{
					//Seleção de controlador para a rotação
					aux = -1;
					while(entradas[2]){
						aux += 4;
						entradas[2]--;					
					}
					while(entradas[3]){
						aux += 2;
						entradas[3]--;					
					}
					while(entradas[4]){
						aux += 1;
						entradas[4]--;					
					}
					while(entradas[5]){
						aux += 3;
						entradas[5]--;					
					}
					if(aux != -1){
						ctrl3 = aux;
						printf("Rotação controlando %d\n", ctrl3);
					}
					//_________________________________________
					if(msg->axis[2] == 32767){
						p_atual = msg->value + 32767;
						p_atual = (((double)p_atual / 65534.0) * 4);
						p_anterior = msg->axis[3] + 32767;
						p_anterior = (((double)p_anterior / 65534.0) * 4);
						if(p_atual > p_anterior) R3_cmd++;
						if(p_atual < p_anterior) R3_cmd--;
					}
					if(msg->axis[2] == -32767){
						p_atual = msg->value + 32767;
						p_atual = (((double)p_atual / 65534.0) * 4);
						p_anterior = msg->axis[3] + 32767;
						p_anterior = (((double)p_anterior / 65534.0) * 4);
						if(p_atual > p_anterior) R3_cmd--;
						if(p_atual < p_anterior) R3_cmd++;
					}
					if(R3_cmd > 127) R3_cmd = 127;
					if(R3_cmd < 0) R3_cmd = 0;
					send_control(ctrl3, R3_cmd, 0, handle, port_id);
				}
				if(msg->value > 0){
					//Baixo 
				}else if(msg->value < 0){
					//Cima
				}else{
					//Neutro
				}
			break;
			case 4:
				//D-Pad Horizontal
				if(msg->value > 0){
					//Direita
				}else if(msg->value < 0){
					//Esquerda
				}else{
					//Neutro
				}
			break;
			case 5:
				//D-pad Vertical
				if(msg->value > 0){
					//Baixo
				}else if(msg->value < 0){
					//Cima
				}else{
					//Neutro
				}
			break;
		}
	}
	while(entradas[2]){
		base--;
		entradas[2]--;
		printf("Base diminuida em 1 ponto\n");
	}
	while(entradas[3]){
		base++;
		entradas[3]--;
		printf("Base aumentada em 1 ponto\n");
	}
}

int main(){
	//Definição dos Combos e das Entradas	
	int i;
	base = 60;
	note_vel = 80;
	R3_cmd = 0;
	R3toggle = 0;
	ctrl1 = 0;
	ctrl2 = 1;
	ctrl3 = 0;
	note = (char**)malloc(8 * sizeof(char*));
	for(i = 0; i < 8; i++){
		note[i] = (char*)malloc(2 * sizeof(char));
		note[i][0] = 0;
		note[i][1] = -1;
	}
	entradas = (int*)malloc(20 * sizeof(int));
	combos = (char**)malloc(20 * sizeof(char*));
	for(i = 0; i < 20; i++){
		combos[i] = (char*)malloc(10 * sizeof(char));
		entradas[i] = 0; 
	}
	combos[0][0] = 2;
	combos[0][1] = 8;
	combos[1][0] = 8;
	combos[1][1] = 2;
	combos[2][0] = 8;
	combos[3][0] = 2;
	combos[4][0] = 6;
	combos[5][0] = 4;
	//Inicialização da Manete	
	joystick_inicialize("/dev/input/js0", &joystick_callback, NULL);
	while(1) usleep(10000);
	return 0;
}
