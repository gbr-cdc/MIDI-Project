//Possui as funções MIDI
#include "MIDI_functions.c"
//Para o usleep
#include <unistd.h>
//Para o open()
#include <fcntl.h>   
//Contem a biblioteca da manete
#include <linux/joystick.h>
//Para utilizar os threads, compilar com -pthread
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Constante da velocidade das notas
#define NOTEVEL 80

//Tem uma boa explicação sobre a API da manete no link abaixo
//https://www.kernel.org/doc/Documentation/input/joystick-api.txt

//As variáveis abaixo guardam informações sobre os botões da manete
//button[x] == 0 <--> o botão x não está pressionado
int button[12];
//axis[x] = valor da posição do eixo x (esse eixo pode ser horizontal ou vertical)
int axis[6];
//note[x] = Última nota tocada pelo botão x
int note[4];

//step e base são usados para definir os caminhos entre as notas do controle
int step = 1;
int base = 60;

//Parâmetros de entrada:
//msg: Struct contendo informações sobre o evento de entrada da manete
//handle: Handle do sequenciador MIDI
//port_id: ID da porta de saída
void joystick_callback(struct js_event msg, snd_seq_t* handle, int port_id){
	if(msg.type == JS_EVENT_BUTTON){
//msg.number identifica o botão, msg.value diz se ele foi pressinado (1) ou solto (0)		
		button[msg.number] = msg.value;
//Este switch mapeia os botões da manete (escluindo o D-Pad)
//Esse mapeamento foi feito para a manete B-MAX (os valores variam de uma para a outra)		
		switch(msg.number){
			case 0:
				//Triângulo
				//Se o botão foi pressinado				
				if(msg.value){
					//Envio um NOTEON e guardo a nota que eu enviei
					send_note(NOTEVEL, base, 0, handle, port_id);
					note[0] = base;
				//Se não
				}else{
					//Envio um NOTEOFF para a nota guardada					
					send_note(0, note[0], 0, handle, port_id);
				}
			break;
			case 1:
				//Bolinha
				if(msg.value){
					send_note(NOTEVEL, base + step, 0, handle, port_id);
					note[1] = base + step;
				}else{
					send_note(0, note[1], 0, handle, port_id);
				}
			break;
			case 2:
				//Xis
				if(msg.value){
					send_note(NOTEVEL, base + 2*step, 0, handle, port_id);
					note[2] = base + 2*step;
				}else{
					send_note(0, note[2], 0, handle, port_id);
				}
			break;
			case 3:
				//Quadrado
				if(msg.value){
					send_note(NOTEVEL, base + 3*step, 0, handle, port_id);
					note[3] = base + 3*step;
				}else{
					send_note(0, note[3], 0, handle, port_id);
				}
			break;
			case 4:
				//L2
				if(msg.value == 1){
					printf("L2 PRESSED\n");
					base -= 4;
				}else{
					printf("L2 RELEASED\n");
				}
			break;
			case 5:
				//R2
				if(msg.value == 1){
					printf("R2 PRESSED\n");
					base += 4;
				}else{
					printf("R2 RELEASED\n");
				}
			break;
			case 6:
				//L1
				if(msg.value == 1){
					printf("L1 PRESSED\n");
					base -= 1;
				}else{
					printf("L1 RELEASED\n");
				}
			break;
			case 7:
				//R1
				if(msg.value == 1){
					printf("R1 PRESSED\n");
					base += 1;
				}else{
					printf("R1 RELEASED\n");
				}
			break;
			case 8:
				//Select
				//PANIC BUTTON				
				if(msg.value == 1){
					printf("SELECT PRESSED\n");
					int c;
					for(c=0; c<128; c++){
						send_note(0, c, 0, handle, port_id);
					}
				}else{
					printf("SELECT RELEASED\n");
				}
			break;
			case 9:
				//Start
				if(msg.value == 1){
					printf("START PRESSED\n");
				}else{
					printf("START RELEASED\n");
				}
			break;
			case 10:
				//L3
				if(msg.value == 1){
					printf("L3 PRESSED\n");
				}else{
					printf("L3 RELEASED\n");
				}
			break;
			case 11:
				//R3
				if(msg.value == 1){
					printf("R3 PRESSED\n");
				}else{
					printf("R3 RELEASED\n");
				}
			break;
		}
	}else if(msg.type == JS_EVENT_AXIS){
//Nesse caso, msg.value guarda a posição do eixo		
		axis[msg.number] == msg.value;
//Este switch mapeia as alavancas e o D-PAD
		switch(msg.number){
		case 0:
//Para eixos horizontais, os valores crescem da esquerda para a direita, com o 0 com centro			
			//L3 Horizontal	
			if(msg.value > 0){
				printf("AXIS 0 RIGHT\n");
				step = 3;
			}else if(msg.value < 0){
				printf("AXIS 0 LEFT\n");
				step = 5;
			}else{
				printf("AXIS 0 NEUTRAL\n");
				step = 1;
			}
		break;
		case 1:
//Para eixoos verticais, os valores crescem de cima para baixo, com o 0 como centro
			//L3 Vertical
			if(msg.value > 0){
				printf("AXIS 1 DOWN\n");
				step = 4;
			}else if(msg.value < 0){
				printf("AXIS 1 UP\n");
				step = 6;
			}else{
				printf("AXIS 1 NEUTRAL\n");
				step = 1;
				
			}
		break;
		case 2:
			//R3 Horizontal
			if(msg.value > 0){
				printf("AXIS 2 RIGHT\n");
			}else if(msg.value < 0){
				printf("AXIS 2 LEFT\n");
			}else{
				printf("AXIS 2 NEUTRAL\n");
			}
		break;
		case 3:
			//R3 Vertical
			if(msg.value > 0){
				printf("AXIS 3 DOWN\n");
			}else if(msg.value < 0){
				printf("AXIS 3 UP\n");
			}else{
				printf("AXIS 3 NEUTRAL\n");
			}
		break;
		case 4:
			//D-Pad Horizontal
			if(msg.value > 0){
				printf("AXIS 4 RIGHT\n");
			}else if(msg.value < 0){
				printf("AXIS 4 LEFT\n");
			}else{
				printf("AXIS 4 NEUTRAL\n");
			}
		break;
		case 5:
			//D-pad Vertical
			if(msg.value > 0){
				printf("AXIS 5 DOWN\n");
			}else if(msg.value < 0){
				printf("AXIS 5 UP\n");
			}else{
				printf("AXIS 5 NEUTRAL\n");
			}
		break;
		}
	}
}

void *thread(void *vargp){
	struct js_event msg;
	//Abre a conexão com a manete
	char* device = "/dev/input/js0";
	int fd = open(device, O_RDONLY);
	//Recebe o nome do dispositivo
	char name[128];
	if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0)
		strncpy(name, "Unknown", sizeof(name));
	//Recebe o número de eixos do dispositivo
	char number_of_axes;
	ioctl(fd, JSIOCGAXES, &number_of_axes);
	//Recebe o número de botões do dispositivo
	char number_of_buttons;
	ioctl(fd, JSIOCGBUTTONS, &number_of_buttons);
	//Abre um cliente com o sequenciador MIDI
	snd_seq_t* handle;
	int in_id, out_id;
	open_client(&handle, &in_id, &out_id);

	while(1){
		//Faz a leitura de um evento da manete, encerra a execução se não conseguir		
		if(read(fd, &msg, sizeof(struct js_event)) != sizeof(struct js_event)) {
			exit(1);
		}else{
			//Esse tipo de mensagem serve apenas para a inicialixação da manete e pode ser ignorada            		
			if (msg.type == JS_EVENT_INIT){
				continue;            		
			}else{
				//Chama a função de callback				
				joystick_callback(msg, handle, out_id);
			}
			usleep(10000);
		}
	}

	pthread_exit((void *)NULL);
}

//Função de inicialização da manete
void joystick_inicialize(){
	int i;
	//inicialização dos vetores do início do código	
	for(i=0; i<12; i++){
		button[i] = 0;
		if(i<6) axis[i] = 0;
		if(i<4) note[i] = -1;
	}
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	pthread_join(tid, NULL);
	pthread_exit((void *)NULL);
}

int main(){
	joystick_inicialize();
	return 0;
}
