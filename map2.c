#include"MIDI_joystick.h"
#include<string.h>

char** combos; //Matriz de combos reconheciveis
int* comandos; //String contendo
int base; //Valor base para a escala de notas na manete
int note_vel; //Valor da dinâmica da mensagem MIDI enviada
char** note; //Vetor que guarda as notas enviadas para ser efetuado um NOTEOFF depois
#define c_qtd 5
//Variaveis da alavanca direita
int R3_cmd[128];
int R3toggle;

//Conexões atuais da controle
int axis_x;
int axis_y;
int rotation;

void joystick_callback(t_mosaic_button_event *msg){
	//Declaração de variáveis	
	int i, j, k, t, aux, port_id, p_atual, p_anterior;
	snd_seq_t* handle;
	handle = msg->handle;
	port_id = msg->out_id;

	//Interpretação da String de Combo
	if(msg->combo){
		j = 0;		
		while(msg->combo[j] != '\0'){
			printf("%d ", msg->combo[j]);				
			j++;
		}
		printf("\n");
		for(i = 0; i <= c_qtd; i++){
			j = 0;
			k = 1;
			//Navega pelo combo
			while(msg->combo[j] != '\0'){				
				//Verifica o padrão
				if(msg->combo[j] == combos[i][k]){
					//Se padrão completo reconhecido
					if(k == (combos[i][0])){
						comandos[i]++;
						//Apago o trecho reconhecido para que ele não seja usado de novo
						for(t = 0; t < k; t++){
							msg->combo[j - t] = 'x';
						}
						k = 1;
					}else{
						k++;
					}
				}
				j++;
			}
		}
		j = 0;
		while(msg->combo[j] != '\0'){
			if(!comandos[c_qtd + 2]){
				if(msg->combo[j] == 4 && msg->combo[j+1] == 6){
					comandos[c_qtd + 1]++;
					msg->combo[j] = 'x';
					msg->combo[j+1] = 'x';
				}
			}
			if(!comandos[c_qtd + 1]){
				if(msg->combo[j] == 6 && msg->combo[j+1] == 4){
					comandos[c_qtd + 2]++;
					msg->combo[j] = 'x';
					msg->combo[j+1] = 'x';
				}
			}
			if(!comandos[c_qtd + 4]){
				if(msg->combo[j] == 2 && msg->combo[j+1] == 8){
					comandos[c_qtd + 3]++;
					msg->combo[j] = 'x';
					msg->combo[j+1] = 'x';
				}
			}
			if(!comandos[c_qtd + 3]){
				if(msg->combo[j] == 8 && msg->combo[j+1] == 2){
					comandos[c_qtd + 4]++;
					msg->combo[j] = 'x';
					msg->combo[j+1] = 'x';
				}
			}
			j++;
		}
		j = 0;
		while(msg->combo[j] != '\0'){
			if(msg->combo[j] == 6) comandos[c_qtd + 5]++;
			if(msg->combo[j] == 2) comandos[c_qtd + 6]++;
			if(msg->combo[j] == 4) comandos[c_qtd + 7]++;
			if(msg->combo[j] == 8) comandos[c_qtd + 8]++;
			j++;
		}
		for(i = 0; i < 20; i++){
			printf("%d ", comandos[i]);
		}
		printf("\n");
	}
	//Interpretação do evento
	if(msg->type == JS_EVENT_BUTTON){						
		//Mudança de oitava		
		while(comandos[c_qtd + 3] > 0){
			base += 12;
			comandos[c_qtd + 3]--;
		}
		while(comandos[c_qtd + 4] > 0){
			base -= 12;
			comandos[c_qtd + 4]--;
		}
		//Sustenido e Bemol		
		for(i = 0; i < comandos[c_qtd + 8]; i++){
			base++;
		}
		for(i = 0; i < comandos[c_qtd + 6]; i++){
			base--;
		}
		//Este switch identifica o botão pressionado
		switch(msg->id){
			case 0:
				//Triângulo				
				if(msg->value){
					//Envia uma nota MIDI caso pressionado					
					send_note(note_vel, base + 9, 0, handle, port_id);
					note[0][0] = base + 9;
				}else{
					//Envia um NOTEOFF para a ultima nota MIDI acionada
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
					//Ativa/desativa a variável R3toggle cadavez que é pressionado
					if(R3toggle == 0) R3toggle = 1;
					else R3toggle = 0;
				}else{

				}
			break;
		}
		for(i = 0; i < comandos[c_qtd + 8]; i++){
		base--;
		}
		for(i = 0; i < comandos[c_qtd + 6]; i++){
			base++;
		}
	}else if(msg->type == JS_EVENT_AXIS){
		aux = -1;
		if(comandos[c_qtd + 5]){
			while(comandos[c_qtd + 5]){
				aux = aux + 1;
				comandos[c_qtd + 5]--;
			}
		}else if(comandos[c_qtd + 6] && !comandos[2] && !comandos[3]){
			aux = 15;			
			while(comandos[c_qtd + 6]){
				aux = aux + 1;
				comandos[c_qtd + 6]--;
			}
		}else if(comandos[c_qtd + 7]){
			aux = 31;
			while(comandos[c_qtd + 7]){
				aux = aux + 1;
				comandos[c_qtd + 7]--;
			}
		}else if(comandos[c_qtd + 8] && !comandos[4] && !comandos[5]){
			aux = 47;
			while(comandos[c_qtd + 8]){
				aux = aux + 1;
				comandos[c_qtd + 8]--;
			}
		}else if(comandos[2]){
			aux = 63;
			while(comandos[c_qtd + 6]){
				aux = aux + 1;
				comandos[c_qtd + 6]--;
			}
		}else if(comandos[3]){
			aux = 78;
			while(comandos[c_qtd + 6]){
				aux = aux + 1;
				comandos[c_qtd + 6]--;
			}
		}else if(comandos[4]){
			aux = 94;
			while(comandos[c_qtd + 8]){
				aux = aux + 1;
				comandos[c_qtd + 8]--;
			}
		}else if(comandos[5]){
			aux = 110;
			while(comandos[c_qtd + 8]){
				aux = aux + 1;
				comandos[c_qtd + 8]--;
			}
		}
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
					if(aux > -1){
						axis_y = aux;
						printf("Horizontal controlando %d\n", axis_y);
					}
					//______________________________________________
					//Este eixo envia uma mensagem MIDI de controle
					aux = msg->value;
					aux += 32767;
					aux = (((double)aux / 65534.0) * 127);
					send_control(axis_y, (int)aux, 0, handle, port_id);
					//______________________________________________
				}else{
					//Seleção de controlador para a rotação					
					if(aux > -1){
						rotation = aux;
						printf("Rotação controlando %d\n", rotation);
					}
					//___________________________________________
					if(msg->axis[3] == 32767){
						p_atual = msg->value + 32767;
						p_atual = (((double)p_atual / 65534.0) * 4);
						p_anterior = msg->axis[2] + 32767;
						p_anterior = (((double)p_anterior / 65534.0) * 4);
						if(p_atual > p_anterior) R3_cmd[rotation]--;
						if(p_atual < p_anterior) R3_cmd[rotation]++;
					}
					if(msg->axis[3] == -32767){
						p_atual = msg->value + 32767;
						p_atual = (((double)p_atual / 65534.0) * 4);
						p_anterior = msg->axis[2] + 32767;
						p_anterior = (((double)p_anterior / 65534.0) * 4);
						if(p_atual > p_anterior) R3_cmd[rotation]++;
						if(p_atual < p_anterior) R3_cmd[rotation]--;
					}
					if(R3_cmd[rotation] > 127) R3_cmd[rotation] = 127;
					if(R3_cmd[rotation] < 0) R3_cmd[rotation] = 0;
					send_control(rotation, R3_cmd[rotation], 0, handle, port_id);
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
					if(aux > -1){
						axis_x = aux;
						printf("Vertical controlando %d\n", axis_x);
					}
					//Este exio envia uma mensagem MIDI de Pitchbend
					aux = msg->value;
					aux += 32767;
					aux = (((double)aux / 65534.0) * 127);
					send_control(axis_x, (int)aux, 0, handle, port_id);
					//_______________________________________________
				}else{
					if(aux > -1){
						rotation = aux;
						printf("Rotação controlando %d\n", rotation);
					}
					if(msg->axis[2] == 32767){
						p_atual = msg->value + 32767;
						p_atual = (((double)p_atual / 65534.0) * 4);
						p_anterior = msg->axis[3] + 32767;
						p_anterior = (((double)p_anterior / 65534.0) * 4);
						if(p_atual > p_anterior) R3_cmd[rotation]++;
						if(p_atual < p_anterior) R3_cmd[rotation]--;
					}
					if(msg->axis[2] == -32767){
						p_atual = msg->value + 32767;
						p_atual = (((double)p_atual / 65534.0) * 4);
						p_anterior = msg->axis[3] + 32767;
						p_anterior = (((double)p_anterior / 65534.0) * 4);
						if(p_atual > p_anterior) R3_cmd[rotation]--;
						if(p_atual < p_anterior) R3_cmd[rotation]++;
					}
					if(R3_cmd[rotation] > 127) R3_cmd[rotation] = 127;
					if(R3_cmd[rotation] < 0) R3_cmd[rotation] = 0;
					send_control(rotation, R3_cmd[rotation], 0, handle, port_id);
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
	for(i = 0; i < 20; i++){
		comandos[i] = 0;
	}
}

int main(){
	int i;
	//Inicialização de valores globais
	base = 60;
	note_vel = 80;
	R3toggle = 0;
	axis_x = 0;
	axis_y = 1;
	rotation = 0;
	//Inicialização do vetor de notas disparadas
	//Este vetor é usado para dar NOTEOFF nas notas corretas
	note = (char**)malloc(8 * sizeof(char*));
	for(i = 0; i < 8; i++){
		note[i] = (char*)malloc(2 * sizeof(char));
		note[i][0] = 0;
		note[i][1] = -1;
	}
	//Inicialização do vetor de comandos reconhecidos e do vetor de combos reconhecíveis
	comandos = (int*)malloc(20 * sizeof(int));
	combos = (char**)malloc(20 * sizeof(char*));
	for(i = 0; i < 20; i++){
		combos[i] = (char*)malloc(10 * sizeof(char));
		comandos[i] = 0; 
	}
	for(i = 0; i < 128; i++){
		R3_cmd[i] = 0;
	}
	//Lista de combos reconhecíveis
	//41236
	combos[0][0] = 5;	
	combos[0][1] = 4;
	combos[0][2] = 1;
	combos[0][3] = 2;
	combos[0][4] = 3;
	combos[0][5] = 6;	
	//63214
	combos[1][0] = 5;	
	combos[1][1] = 6;
	combos[1][2] = 3;
	combos[1][3] = 2;
	combos[1][4] = 1;
	combos[1][5] = 4;
	//236
	combos[2][0] = 3;
	combos[2][1] = 2;
	combos[2][2] = 3;
	combos[2][3] = 6;
	//214
	combos[3][0] = 3;
	combos[3][1] = 2;
	combos[3][2] = 1;
	combos[3][3] = 4;
	//896
	combos[4][0] = 3;
	combos[4][1] = 8;
	combos[4][2] = 9;
	combos[4][3] = 6;
	//874
	combos[5][0] = 3;
	combos[5][1] = 8;
	combos[5][2] = 7;
	combos[5][3] = 4;
	
	//Inicialização da Manete	
	joystick_inicialize("/dev/input/js0", &joystick_callback, NULL);
	while(1) usleep(10000);
	return 0;
}
