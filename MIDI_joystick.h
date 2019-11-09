#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "MIDI_functions.h"

//Estrutura que encapsula o evento gerado por acionar a manete
typedef struct{
	int chain;
	char* combo; //String de como caso esxista uma, NULL caso contrário
	int* axis; //Vetor dos estados atuais dos eixos da manete
	int* buttons; //Vetor dos estados atuais dos botões da manete
	unsigned int type; //JS_EVENT_BUTTON, JS_EVENT_AXIS ou JS_EVENT_INIT
	unsigned int id; //Identifica qual botão/eixo foi acionado
	int value; //1(pressionado) ou 0(solto) para botões, [-32767, +32767] para eixos
	unsigned int time; //tempo em que o botão foi pressionado em milisegundos
	snd_seq_t* handle; //Handle do sequenciador MIDI alsa
	int in_id; //ID da porta de entrada do sequenciador
	int out_id; //ID da porta de saida do sequenciador
}t_mosaic_button_event;

//Definição da assinatura da função callback de evento
typedef void (t_mosaic_joystick_event_callback_function)(
                t_mosaic_button_event *event
                );

//Deefinição da assinatura da função callback de registro
typedef void (t_mosaic_joystick_register_callback_function)(
                char * device,
                char * name,
                char number_of_axes,
                char number_of_buttons,
                int driver_version
                );

//Estrutura de cada device aberto, incluindo o caminho e as funções de callback
typedef struct {
    t_mosaic_joystick_event_callback_function * event_callback_function;
    t_mosaic_joystick_register_callback_function * register_callback_function;
    char * device;
} t_mosaic_device_data;

//Thread que abre e gerencia as informações e entradas de cada manete
void *joystick_thread(void *data){
	int buttons_pressed;
	int chain;	
	//vetor que monitorará o estado atual dos eixos	
	int* axis;
	//vetor que monitorará o estado atual dos butões
	int* buttons;	
	//Variável auxiliar	
	int i;
	//Se uma função de callback não foi especificada a thread se encerra logo no início
	if (((t_mosaic_device_data *)data)->event_callback_function == NULL){
		pthread_exit((void *)NULL);
	}
	//Abertura do arquivo de entrada da manete
	int fd = open(((t_mosaic_device_data *)data)->device, O_RDONLY);
	//A chamada abaixo informa o nome do dispositivo
	//Se o nome não puder ser recebido ele é definido como "Unknown"	
	char name[128];
	if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0)
		strncpy(name, "Unknown", sizeof(name));
	//A chamada abaixo informa o número de eixos da manete
	char number_of_axes;
	ioctl(fd, JSIOCGAXES, &number_of_axes);
	//Inicialização do vetor de eixos	
	axis = (int*)malloc((int)number_of_axes * sizeof(int));
	for(i = 0; i < (int)number_of_axes; i++) axis[i] = 0;
	//A chamada abaixo informa o número de botões da manete
	char number_of_buttons;
	ioctl(fd, JSIOCGBUTTONS, &number_of_buttons);
	//Inicialização do vetor de botões
	buttons = (int*)malloc((int)number_of_buttons * sizeof(int));
	for(i = 0; i < number_of_buttons; i++) buttons[i] = 0;
	//A chamada abaixo informa a versão do driver do dispositivo
	int driver_version;
	ioctl(fd,JSIOCGVERSION,&driver_version);
	//Se houver uma função de callback para registro das informações, ela é chamada aqui
	if (((t_mosaic_device_data *)data)->register_callback_function != NULL)
		((t_mosaic_device_data *)data)->register_callback_function(
			((t_mosaic_device_data *)data)->device,
			name,
			number_of_axes,
			number_of_buttons,
			driver_version);
		
	//Abre um cliente com o sequenciador MIDI
	snd_seq_t* handle;
	int in_id, out_id;
	open_client(&handle, &in_id, &out_id, "ControlTunes");
	//A partir daqui os eventos das entradas da manete começam a serem lidos
	t_mosaic_button_event *btn_event = (t_mosaic_button_event *) malloc(sizeof(btn_event));
	struct js_event msg;
	//Primeiro os eventos de inicialização da manete são recebidos e ignorados
	do{
		//Nesse e no laço abaixo, se ocorreu um erro na hora de ler o evento a thread se encerrra		
		if(read(fd, &msg, sizeof(struct js_event)) != sizeof(struct js_event)){
			printf("JOYSTICK INITIALAZING ERROR\n");			
			pthread_exit((void *)NULL);
		}
	}while(msg.type == JS_EVENT_INIT);
	//Combo Buffer
	char* cbuffer;
	int ccount;
	unsigned int time; 
	cbuffer = (char*)malloc(128 * sizeof(char));
	ccount = 0;
	time = 0;
	buttons_pressed = 0;
	//O laço abaixo faz o tratamento dos eventos verdadeiros	
	while(1) {
		if(read(fd, &msg, sizeof(struct js_event)) != sizeof(struct js_event)){
			printf("EVENT READING ERROR\n");			
			pthread_exit((void *)NULL);
		}
		//Esse trecho conta a quantidade de botões que estão em estado pressionado
		if(msg.type == JS_EVENT_BUTTON){
			if(msg.value) buttons_pressed++;
			else buttons_pressed--;
		}	
		//Tratamento de Combo
		if((msg.type == JS_EVENT_AXIS) && ((msg.number == 0) || (msg.number == 1) || (msg.number == 4) || (msg.number == 5))){
			if(ccount == 128) ccount = 0;
			if((msg.time - time) > 500){
				ccount = 0;
			}
			if((msg.number % 2) != 0){
				if(msg.value == -32767){
					if(axis[msg.number - 1] == -32767) cbuffer[ccount++] = 7; 
					if(axis[msg.number - 1] == 0) cbuffer[ccount++] = 8;
					if(axis[msg.number - 1] == 32767) cbuffer[ccount++] = 9;
				}
				if(msg.value == 32767){
					if(axis[msg.number - 1] == -32767) cbuffer[ccount++] = 1;
					if(axis[msg.number - 1] == 0) cbuffer[ccount++] = 2;
					if(axis[msg.number - 1] == 32767) cbuffer[ccount++] = 3;
				}
				if(msg.value == 0){
					if(axis[msg.number - 1] == -32767) cbuffer[ccount++] = 4;
					//if(axis[msg.number - 1] == 0) ccount--;
					if(axis[msg.number - 1] == 32767) cbuffer[ccount++] = 6;
				}
			}else{
				if(msg.value == -32767){
					if(axis[msg.number + 1] == -32767) cbuffer[ccount++] =  7; 
					if(axis[msg.number + 1] == 0) cbuffer[ccount++] = 4;
					if(axis[msg.number + 1] == 32767) cbuffer[ccount++] = 1;
				}
				if(msg.value == 32767){
					if(axis[msg.number + 1] == -32767) cbuffer[ccount++] = 9;
					if(axis[msg.number + 1] == 0) cbuffer[ccount++] = 6;
					if(axis[msg.number + 1] == 32767) cbuffer[ccount++] = 3;
				}
				if(msg.value == 0){
					if(axis[msg.number + 1] == -32767) cbuffer[ccount++] = 8;
					//if(axis[msg.number + 1] == 0) ccount--;
					if(axis[msg.number + 1] == 32767) cbuffer[ccount++] = 2;
				}
			}			
		}
		btn_event->combo = NULL;
		//Se a entrada for um botão
		if(msg.type == JS_EVENT_BUTTON){			
			//Ele passará a string de combo caso ouver algum no buffer			
			if(ccount > 0){
				cbuffer[ccount] = '\0';
				btn_event->combo = cbuffer;
				ccount = 0;
			}else{
				//Se o botão for pressionado logo após outro e houver um combo desencadeado, ele também receberá o combo
				//if(((msg.time - time) < 200) && chain && (msg.value == 1)) btn_event->combo = cbuffer;
				//else chain = 0;
			}
			if(((msg.time - time) < 500)) btn_event->chain = 1;
			else btn_event->chain = 0;
		}
		if(msg.type == JS_EVENT_AXIS){
			if(msg.number == 2 || msg.number == 3){
				if(ccount > 0){
					cbuffer[ccount] = '\0';
					btn_event->combo = cbuffer;
					ccount = 0;
				}	
			}
		}
		//Definição do btn_event
		btn_event->type = msg.type;
		btn_event->id = msg.number;
		btn_event->value = msg.value;
		btn_event->time = msg.time;
		btn_event->handle = handle;
		btn_event->in_id = in_id;
		btn_event->out_id = out_id;
		btn_event->axis = axis;
		btn_event->buttons = buttons; 
		//A chamada da função de callback para os eventos se encontra aqui
		((t_mosaic_device_data *)data)->event_callback_function(btn_event);
		//Atualização do estado do joystick
		if(msg.type == JS_EVENT_AXIS) axis[msg.number] = msg.value;
		if(msg.type == JS_EVENT_BUTTON) buttons[msg.number] = msg.value;
		time = msg.time;
	}
}

//Parâmetros de entrada:
//device: string contendo o caminho para o arquivo de entrada
//event_callback_function: ponteiro para a função de callback para os eventos
//register_callback_function: ponteiro para a função de callback para o registro de informações
void joystick_inicialize(const char * device,
                t_mosaic_joystick_event_callback_function * event_callback_function,
                t_mosaic_joystick_register_callback_function * register_callback_function){
	t_mosaic_device_data * data = (t_mosaic_device_data *) malloc(sizeof(t_mosaic_device_data));
	data->device = (char *) malloc(strlen(device));
	strcpy(data->device, device);
	data->event_callback_function = event_callback_function;
	data->register_callback_function = register_callback_function;
	pthread_t tid;
	pthread_create(&tid, NULL, joystick_thread, data);
}
