#include"MIDI_joystick.h"
#include<string.h>

char** combos; //Matriz de combos reconheciveis
int* entradas; //Vetor que guarda os combos reconhecidos
int base; //Valor base para a escala de notas na manete
int note_vel; //Valor da dinâmica da mensagem MIDI enviada
char** note; //Vetor que guarda as notas enviadas para ser efetuado um NOTEOFF depois

void joystick_callback(t_mosaic_button_event *msg){
	//Declaração de variáveis	
	int i, s1, s2, aux;
	//Interpretação da String de Combo	
	i = 0;
	s1 = 0;
	s2 = 0;
	if(msg->combo){
		while(msg->combo[i] != '\0'){
			if(msg->combo[i] == combos[0][s1]){
				s1++;
				if(s1 > 2){
					entradas[0]++;
					s1 = 0;			
				}
			}else s1 = 0;
			if(msg->combo[i] == combos[1][s2]){
				s2++;
				if(s2 > 2){
					entradas[1]++;
					s2 = 0;
				}
			}else s2 = 0;
			printf("%d ", msg->combo[i]);
			i++;
		}
		printf("\n");
	}
	//Interpretação do evento
	handle = msg->handle;
	port_id = msg->out_id;
	if(msg->type == JS_EVENT_BUTTON){				
		switch(msg->id){
			case 0:
				//Triângulo				
				if(msg->value){
					send_note(note_vel, base + 9, 0, handle, port_id);
					note[0][0] = base + 9;
					//Combos
					if(entradas[0]){
						send_note(note_vel, base + 13, 0, handle, port_id);
						send_note(note_vel, base + 16, 0, handle, port_id);
						note[0][1] = 1;
						entradas[0] = 0;  
					}
					if(entradas[1]){
						send_note(note_vel, base + 12, 0, handle, port_id);
						send_note(note_vel, base + 16, 0, handle, port_id);
						note[0][1] = 2;
						entradas[1] = 0;  
					}
				}else{				
					send_note(0, note[0][0], 0, handle, port_id);
					//Combos
					if(note[0][1] & 1){
						send_note(0, note[0][0] + 4, 0, handle, port_id);
						send_note(0, note[0][0] + 7, 0, handle, port_id);
						note[0][1] = 0;
					}
					if(note[0][1] & 2){
						send_note(0, note[0][0] + 3, 0, handle, port_id);
						send_note(0, note[0][0] + 7, 0, handle, port_id);
						note[0][1] = 0;
					}
				}
			break;
			case 1:
				//Bolinha
				if(msg->value){
					send_note(note_vel, base + 12, 0, handle, port_id);
					note[1][0] = base + 12;
					//Combos
					if(entradas[0]){
						send_note(note_vel, base + 16, 0, handle, port_id);
						send_note(note_vel, base + 19, 0, handle, port_id);
						note[1][1] = 1;
						entradas[0] = 0;  
					}
					if(entradas[1]){
						send_note(note_vel, base + 15, 0, handle, port_id);
						send_note(note_vel, base + 19, 0, handle, port_id);
						note[1][1] = 2;
						entradas[1] = 0;  
					}
				}else{
					send_note(0, note[1][0], 0, handle, port_id);
					//Combos
					if(note[1][1] & 1){
						send_note(0, note[1][0] + 4, 0, handle, port_id);
						send_note(0, note[1][0] + 7, 0, handle, port_id);
						note[1][1] = 0;
					}
					if(note[1][1] & 2){
						send_note(0, note[1][0] + 3, 0, handle, port_id);
						send_note(0, note[1][0] + 7, 0, handle, port_id);
						note[1][1] = 0;
					}
				}
			break;
			case 2:
				//Xis
				if(msg->value){
					send_note(note_vel, base + 11, 0, handle, port_id);
					note[2][0] = base + 11;
					//Combos
					if(entradas[0]){
						send_note(note_vel, base + 15, 0, handle, port_id);
						send_note(note_vel, base + 18, 0, handle, port_id);
						note[2][1] |= 1;
						entradas[0] = 0;  
					}
					if(entradas[1]){
						send_note(note_vel, base + 14, 0, handle, port_id);
						send_note(note_vel, base + 18, 0, handle, port_id);
						note[2][1] |= 2;
						entradas[1] = 0;  
					}
				}else{
					send_note(0, note[2][0], 0, handle, port_id);
					//Combos
					if(note[2][1] & 1){
						send_note(0, note[2][0] + 4, 0, handle, port_id);
						send_note(0, note[2][0] + 7, 0, handle, port_id);
						note[2][1] -= 1;
					}
					if(note[2][1] & 2){
						send_note(0, note[2][0] + 3, 0, handle, port_id);
						send_note(0, note[2][0] + 7, 0, handle, port_id);
						note[2][1] -= 2;
					}
				}
			break;
			case 3:
				//Quadrado
				if(msg->value){
					send_note(note_vel, base + 7, 0, handle, port_id);
					note[3][0] = base + 7;
					//Combos
					if(entradas[0]){
						send_note(note_vel, base + 11, 0, handle, port_id);
						send_note(note_vel, base + 14, 0, handle, port_id);
						note[3][1] = 1;
						entradas[0] = 0;  
					}
					if(entradas[1]){
						send_note(note_vel, base + 10, 0, handle, port_id);
						send_note(note_vel, base + 14, 0, handle, port_id);
						note[3][1] = 2;
						entradas[1] = 0;  
					}
				}else{
					send_note(0, note[3][0], 0, handle, port_id);
					//Combos
					if(note[3][1] & 1){
						send_note(0, note[3][0] + 4, 0, handle, port_id);
						send_note(0, note[3][0] + 7, 0, handle, port_id);
						note[3][1] = 0;
					}
					if(note[3][1] & 2){
						send_note(0, note[3][0] + 3, 0, handle, port_id);
						send_note(0, note[3][0] + 7, 0, handle, port_id);
						note[3][1] = 0;
					}
				}
			break;
			case 4:
				//L2
				if(msg->value){
					send_note(note_vel, base, 0, handle, port_id);
					note[4][0] = base;
					//Combos
					if(entradas[0]){
						send_note(note_vel, base + 4, 0, handle, port_id);
						send_note(note_vel, base + 7, 0, handle, port_id);
						note[4][1] = 1;
						entradas[0] = 0;  
					}
					if(entradas[1]){
						send_note(note_vel, base + 3, 0, handle, port_id);
						send_note(note_vel, base + 7, 0, handle, port_id);
						note[4][1] = 2;
						entradas[1] = 0;  
					}
				}else{
					send_note(0, note[4][0], 0, handle, port_id);
					//Combos
					if(note[4][1] & 1){
						send_note(0, note[4][0] + 4, 0, handle, port_id);
						send_note(0, note[4][0] + 7, 0, handle, port_id);
						note[4][1] = 0;
					}
					if(note[4][1] & 2){
						send_note(0, note[4][0] + 3, 0, handle, port_id);
						send_note(0, note[4][0] + 7, 0, handle, port_id);
						note[4][1] = 0;
					}
				}
			break;
			case 5:
				//R2
				if(msg->value){
					send_note(note_vel, base + 2, 0, handle, port_id);
					note[5][0] = base + 2;
					//Combos
					if(entradas[0]){
						send_note(note_vel, base + 6, 0, handle, port_id);
						send_note(note_vel, base + 9, 0, handle, port_id);
						note[5][1] = 1;
						entradas[0] = 0;  
					}
					if(entradas[1]){
						send_note(note_vel, base + 5, 0, handle, port_id);
						send_note(note_vel, base + 9, 0, handle, port_id);
						note[5][1] = 2;
						entradas[1] = 0;  
					}
				}else{
					send_note(0, note[5][0], 0, handle, port_id);
					//Combos
					if(note[5][1] & 1){
						send_note(0, note[5][0] + 4, 0, handle, port_id);
						send_note(0, note[5][0] + 7, 0, handle, port_id);
						note[5][1] = 0;
					}
					if(note[5][1] & 2){
						send_note(0, note[5][0] + 3, 0, handle, port_id);
						send_note(0, note[5][0] + 7, 0, handle, port_id);
						note[5][1] = 0;
					}
				}
			break;
			case 6:
				//L1
				if(msg->value){
					send_note(note_vel, base + 4, 0, handle, port_id);
					note[6][0] = base + 4;
					//Combos
					if(entradas[0]){
						send_note(note_vel, base + 8, 0, handle, port_id);
						send_note(note_vel, base + 11, 0, handle, port_id);
						note[6][1] = 1;
						entradas[0] = 0;  
					}
					if(entradas[1]){
						send_note(note_vel, base + 7, 0, handle, port_id);
						send_note(note_vel, base + 11, 0, handle, port_id);
						note[6][1] = 2;
						entradas[1] = 0;  
					}
				}else{
					send_note(0, note[6][0], 0, handle, port_id);
					//Combos
					if(note[6][1] & 1){
						send_note(0, note[6][0] + 4, 0, handle, port_id);
						send_note(0, note[6][0] + 7, 0, handle, port_id);
						note[6][1] = 0;
					}
					if(note[6][1] & 2){
						send_note(0, note[6][0] + 3, 0, handle, port_id);
						send_note(0, note[6][0] + 7, 0, handle, port_id);
						note[6][1] = 0;
					}
				}
			break;
			case 7:
				//R1
				if(msg->value){
					send_note(note_vel, base + 5, 0, handle, port_id);
					note[7][0] = base + 5;
					//Combos
					if(entradas[0]){
						send_note(note_vel, base + 9, 0, handle, port_id);
						send_note(note_vel, base + 12, 0, handle, port_id);
						note[7][1] = 1;
						entradas[0] = 0;  
					}
					if(entradas[1]){
						send_note(note_vel, base + 8, 0, handle, port_id);
						send_note(note_vel, base + 12, 0, handle, port_id);
						note[7][1] = 2;
						entradas[1] = 0;  
					}
				}else{
					send_note(0, note[7][0], 0, handle, port_id);
					//Combos
					if(note[7][1] & 1){
						send_note(0, note[7][0] + 4, 0, handle, port_id);
						send_note(0, note[7][0] + 7, 0, handle, port_id);
						note[7][1] = 0;
					}
					if(note[7][1] & 2){
						send_note(0, note[7][0] + 3, 0, handle, port_id);
						send_note(0, note[7][0] + 7, 0, handle, port_id);
						note[7][1] = 0;
					}
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
					base -= 12;
					printf("Base atual = %d\n", base);
				}else{

				}
			break;
			case 11:
				//R3
				if(msg->value == 1){
					base += 12;
					printf("Base atual = %d\n", base);
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
				//Este eixo envia uma mensagem MIDI de controle
				aux = msg->value;
				aux += 32767;
				aux = (((double)aux / 65534.0) * 127);
				send_control(1, (int)aux, 0, handle, port_id);
				//______________________________________________
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
				//Este exio envia uma mensagem MIDI de Pitchbend
				aux = msg->value;
				aux = (((double)aux / 32767) * 8191);
				send_pitchbend(3, (int)aux, 0, handle, port_id);
				//_______________________________________________
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
}

int main(){
	//Definição dos Combos e das Entradas	
	int i;
	base = 60;
	note_vel = 80;
	note = (char**)malloc(8 * sizeof(char*));
	for(i = 0; i < 8; i++){
		note[i] = (char*)malloc(2 * sizeof(char));
		note[i][0] = 0;
		note[i][1] = -1;
	}
	entradas = (int*)malloc(2 * sizeof(int));
	combos = (char**)malloc(2 * sizeof(char*));
	for(i = 0; i < 2; i++){
		combos[i] = (char*)malloc(10 * sizeof(char));
		entradas[i] = 0; 
	}
	combos[0][0] = 2;
	combos[0][1] = 3;
	combos[0][2] = 6;
	combos[1][0] = 6;
	combos[1][1] = 3;
	combos[1][2] = 2;
	//Inicialização da Manete	
	joystick_inicialize("/dev/input/js0", &joystick_callback, NULL);
	while(1) usleep(10000);
	return 0;
}
