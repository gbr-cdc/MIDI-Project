#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//As funções do ALSA costumam retornar valores negativos para indicar falhas de execução. Este #define permite encapsular as chamadas de função em uma estrutura que verifica esse retorno e
//exibe uma mensagem de erro  
#define CHK(stmt, msg) if((stmt) < 0) {puts("ERROR: "#msg); exit(1);}

#define note_time 1000000

//Abre o ALSA Sequencer para entrada e saida e cria uma porta para cada função
int open_client(snd_seq_t** handle, int* in_port_id, int* out_port_id){
	CHK(snd_seq_open(handle, "default", SND_SEQ_OPEN_DUPLEX, 0), "Could not open sequencer");
	CHK(snd_seq_set_client_name(*handle, "Talker Client"), "Could not set client name");
	CHK(*out_port_id = snd_seq_create_simple_port(*handle, "O-port", SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, SND_SEQ_PORT_TYPE_APPLICATION), "Could not open port");
	CHK(*in_port_id = snd_seq_create_simple_port(*handle, "I-port", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION), "Could not open port");
	return 0;
}

//Recebe o handle do Sequencer e retorna um evento mid recebido pela porta de entrada
snd_seq_event_t *midi_read(snd_seq_t* seq_handle)
{
	snd_seq_event_t* ev = NULL;
   	snd_seq_event_input(seq_handle, &ev);	
	return ev;
}

void gen_note(snd_seq_event_t* ev, unsigned char vel, unsigned char note, snd_seq_t* handle){
	snd_seq_ev_set_noteon(ev, 0, note, vel);
	snd_seq_event_output(handle, ev);
	snd_seq_drain_output(handle);

	usleep(note_time);

	snd_seq_ev_set_noteoff(ev, 0, note, 0);
	snd_seq_event_output(handle, ev);
	snd_seq_drain_output(handle);

	//usleep(note_time/6);
	printf("Event sent\n");
}

int translate(snd_seq_event_t* ev, snd_seq_t* handle, unsigned char vel[]){ 	
	if(ev->type != SND_SEQ_EVENT_CONTROLLER){
		printf("ERROR: Event is not a controller\n");
		return 1;
	}
	snd_seq_event_t out_ev;
	snd_seq_ev_clear(&out_ev);
	snd_seq_ev_set_source(&out_ev, 0);
	snd_seq_ev_set_subs(&out_ev);
	snd_seq_ev_set_direct(&out_ev);
	
	switch(ev->data.control.param){
		case 58:
			printf("ctrl: %d\n", ev->data.control.param); 
		break;

		case 46:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 43:
			printf("ctrl: %d\n", ev->data.control.param);
		break;
		
		case 59:
			printf("ctrl: %d\n", ev->data.control.param);
		break;
		
		case 44:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 60:
			printf("ctrl: %d\n", ev->data.control.param);
		break;
		
		case 42:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 61:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 41:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 62:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 45:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 32:
			gen_note(&out_ev, vel[0], 60, handle);

		break;

		case 48:
			gen_note(&out_ev, vel[0], 61, handle);
		break;

		case 64:
			gen_note(&out_ev, vel[0], 62, handle);
		break;

		case 16:
			vel[0] = (unsigned char) ev->data.control.value;
		break;

		case 0:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 33:
			gen_note(&out_ev, vel[1], 63, handle);
		break;

		case 49:
			gen_note(&out_ev, vel[1], 64, handle);
		break;

		case 65:
			gen_note(&out_ev, vel[1], 65, handle);
		break;

		case 17:
			vel[1] = (unsigned char) ev->data.control.value;
		break;

		case 1:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 34:
			gen_note(&out_ev, vel[2], 66, handle);
		break;

		case 50:
			gen_note(&out_ev, vel[2], 67, handle);
		break;

		case 66:
			gen_note(&out_ev, vel[2], 68, handle);
		break;

		case 18:
			vel[2] = (unsigned char) ev->data.control.value;
		break;

		case 2:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 35:
			gen_note(&out_ev, vel[3], 69, handle);
		break;

		case 51:
			gen_note(&out_ev, vel[3], 70, handle);
		break;

		case 67:
			gen_note(&out_ev, vel[3], 71, handle);
		break;

		case 19:
			vel[3] = (unsigned char) ev->data.control.value;
		break;

		case 3:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 36:
			gen_note(&out_ev, vel[4], 72, handle);
		break;

		case 52:
			gen_note(&out_ev, vel[4], 73, handle);
		break;

		case 68:
			gen_note(&out_ev, vel[4], 74, handle);
		break;

		case 20:
			vel[4] = (unsigned char) ev->data.control.value;
		break;

		case 4:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 37:
			gen_note(&out_ev, vel[5], 75, handle);
		break;

		case 53:
			gen_note(&out_ev, vel[5], 76, handle);
		break;

		case 69:
			gen_note(&out_ev, vel[5], 77, handle);
		break;

		case 21:
			vel[5] = (unsigned char) ev->data.control.value;
		break;

		case 5:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 38:
			gen_note(&out_ev, vel[6], 78, handle);
		break;

		case 54:
			gen_note(&out_ev, vel[6], 79, handle);
		break;

		case 70:
			gen_note(&out_ev, vel[6], 80, handle);
		break;

		case 22:
			vel[6] = (unsigned char) ev->data.control.value;
		break;

		case 6:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		case 39:
			gen_note(&out_ev, vel[7], 81, handle);
		break;

		case 55:
			gen_note(&out_ev, vel[7], 82, handle);
		break;

		case 71:
			gen_note(&out_ev, vel[7], 83, handle);
		break;

		case 23:
			vel[7] = (unsigned char) ev->data.control.value;
		break;

		case 7:
			printf("ctrl: %d\n", ev->data.control.param);
		break;

		default:
			printf("Unknow controller\n");
	}		
	return 0;
}

int main(void){
	snd_seq_t* handle;
	int id_in, id_out;
	unsigned char vel[8];
	int i;
	for(i=0; i < 8; i++){
		vel[i] = 0;
	}
	open_client(&handle, &id_in, &id_out);
	while(1){
		translate(midi_read(handle), handle, vel); 
	}
	return 0;
}
