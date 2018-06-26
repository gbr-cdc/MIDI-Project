#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>

//As funções do ALSA costumam retornar valores negativos para indicar falhas de execução. Este #define permite encapsular as chamadas de função em uma estrutura que verifica esse retorno e
//exibe uma mensagem de erro  
#define CHK(stmt, msg) if((stmt) < 0) {puts("ERROR: "#msg); exit(1);}
#define vel 60

//Parãmetros de entrada:
//handle: variável que guarda o handle do sequenciador (endereço de um ponteiro)
//port_id: variável que guarda o id da porta de saída (endereço de um inteiro)
//Descrição:
//Abre um cliente com o sequenciador ALSA e cria uma porta de saída para ele
int open_client(snd_seq_t** handle, int* port_id){
	CHK(snd_seq_open(handle, "default", SND_SEQ_OPEN_OUTPUT, 0), "Could not open sequencer");
	CHK(snd_seq_set_client_name(*handle, "Chords Client"), "Could not set client name");
	CHK(*port_id = snd_seq_create_simple_port(*handle, "Out", SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, SND_SEQ_PORT_TYPE_APPLICATION), "Could not open port");
}

//Parâmetros de entrada:
//handle: handle do sequenciador ALSA
//id: número de identificação da porta de saída
//note: nota que nomeia o acorde
//type: indica se deverá ser enviado um noteon ou um noteoff
//Descrição:
//Envia três eventos MIDI do tipo nota que formam um acorde maior da nota base(a nota mas a terceira e a quinta de sua escala)
void snd_chord(snd_seq_t* handle, int id, int note, int type){
	snd_seq_event_t event;
	snd_seq_ev_set_subs(&event);
	snd_seq_ev_set_direct(&event);
	snd_seq_ev_set_source(&event, id);
	if(type > 0){
		snd_seq_ev_set_noteon(&event, 0, note, vel);
		snd_seq_event_output(handle, &event);
		
		snd_seq_ev_set_noteon(&event, 0, note + 2, vel);
		snd_seq_event_output(handle, &event);
		
		snd_seq_ev_set_noteon(&event, 0, note + 4, vel);
		snd_seq_event_output(handle, &event);
	}else{
		snd_seq_ev_set_noteoff(&event, 0, note, 0);
		snd_seq_event_output(handle, &event);
		
		snd_seq_ev_set_noteoff(&event, 0, note + 2, 0);
		snd_seq_event_output(handle, &event);
		
		snd_seq_ev_set_noteoff(&event, 0, note + 4, 0);
		snd_seq_event_output(handle, &event);
	}
	snd_seq_drain_output(handle);
}

int main(int argc, char** argv){
	snd_seq_t* handle;
	int port_id, i;
	char key;
//O vetor state[] guarda se um acorde está ativo ou não
//Se o valor do vetor na posição correspondente ao acorde for 1 ele está inativo e a função snd_chord deve enviar eventos do tipo NOTEON
//Se o valor for -1, ele está ativo e a função snd_chord deve enviar eventos do tipo NOTEOFF
//Os 7 espaços do vetor estão associados às 7 notas Dó, Ré, Mi, Fá, Sol, Lá, Si respectivamente 	
	int state[7];
	for(i = 0; i < 7; i++){
		state[i] = 1;
	}
	open_client(&handle, &port_id);
	while(1){
		key = getchar();
		switch(key){
			case 'c':
				snd_chord(handle, port_id, 60, state[0]);
				state[0] = state[0] * -1;
			break;
			
			case 'd':
				snd_chord(handle, port_id, 62, state[1]);
				state[1] = state[1] * -1;
			break;
			
			case 'e':
				snd_chord(handle, port_id, 64, state[2]);
				state[2] = state[2] * -1;
			break;
			
			case 'f':
				snd_chord(handle, port_id, 65, state[3]);
				state[3] = state[3] * -1;
			break;
			
			case 'g':
				snd_chord(handle, port_id, 67, state[4]);
				state[4] = state[4] * -1;
			break;
			
			case 'a':
				snd_chord(handle, port_id,  69, state[5]);
				state[5] = state[5] * -1;
			break;
			
			case 'b':
				snd_chord(handle, port_id, 71, state[6]);
				state[6] = state[6] * -1;
			break;
			
			default:
				printf("wrong key\n");
		}
		//Limpeza de buffer
		while ((key = getchar()) != '\n' && key != EOF) { }
	}
	return 0;
}
