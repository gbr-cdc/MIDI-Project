#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//As funções do ALSA costumam retornar valores negativos para indicar falhas de execução. Este #define permite encapsular as chamadas de função em uma estrutura que verifica esse retorno e
//exibe uma mensagem de erro  
#define CHK(stmt, msg) if((stmt) < 0) {puts("ERROR: "#msg); exit(1);}

//Abre o ALSA Sequencer para entrada e saida e cria uma porta para cada função
int open_client(snd_seq_t** handle, int* port_id){
	CHK(snd_seq_open(handle, "default", SND_SEQ_OPEN_DUPLEX, 0), "Could not open sequencer");
	CHK(snd_seq_set_client_name(*handle, "Talker Client"), "Could not set client name");
	CHK(*port_id = snd_seq_create_simple_port(*handle, "O-port", SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, SND_SEQ_PORT_TYPE_APPLICATION), "Could not open port");
	CHK(*port_id = snd_seq_create_simple_port(*handle, "I-port", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION), "Could not open port");
	return 0;
}

//Recebe o handle do Sequencer e retorna um evento mid recebido pela porta de entrada
snd_seq_event_t *midi_read(snd_seq_t* seq_handle)
{
	snd_seq_event_t* ev = NULL;
   	snd_seq_event_input(seq_handle, &ev);	
	return ev;
}

int translate(snd_seq_event_t* ev, snd_seq_t* handle){
	if(ev->type != SND_SEQ_EVENT_CONTROLLER){
		printf("ERROR: Event is not a controller\n");
		return 1;
	}
	//snd_seq_event_t* out_ev;
	switch(ev->data.control.param){
		case (char)58:
			printf("ctrl: %d\n", ev->data.control.param); 
		break;

		case (char)46:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)43:
			printf("ctrl: %d\n", ev->data.control.param);
		break;
		
		case (char)59:
			printf("ctrl: %d\n", ev->data.control.param);
		break;
		
		case (char)44:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)60:
			printf("ctrl: %d\n", ev->data.control.param);
		break;
		
		case (char)42:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)61:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)41:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)62:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)45:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)32:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)48:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)64:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)16:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)0:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)33:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)49:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)65:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)17:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)1:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)34:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)50:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)66:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)18:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)2:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)35:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)51:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)67:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)19:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)3:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)36:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)52:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)68:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)20:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)4:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)37:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)53:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)69:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)21:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)5:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)38:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)54:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)70:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)22:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)6:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)39:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)55:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)71:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)23:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case (char)7:
			printf("ctrl: %d\n", ev->data.control.param);
		break;
	}		
	return 0;
}

int main(void){
	snd_seq_t* handle;
	int id;
	open_client(&handle, &id);
	while(1){
		translate(midi_read(handle), handle); 
	}
	return 0;
}
