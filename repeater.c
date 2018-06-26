#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//As funções do ALSA costumam retornar valores negativos para indicar falhas de execução. Este #define permite encapsular as chamadas de função em uma estrutura que verifica esse retorno e
//exibe uma mensagem de erro  
#define CHK(stmt, msg) if((stmt) < 0) {puts("ERROR: "#msg); exit(1);}

int open_client(snd_seq_t** handle, int* port_id){
	CHK(snd_seq_open(handle, "default", SND_SEQ_OPEN_OUTPUT, 0), "Could not open sequencer");
	CHK(snd_seq_set_client_name(*handle, "Repetidor"), "Could not set client name");
	CHK(*port_id = snd_seq_create_simple_port(*handle, "Out", SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, SND_SEQ_PORT_TYPE_APPLICATION), "Could not open port");
}

int main(int argc, char** argv){
	snd_seq_t* handle;
	int port_id, c, note;
	open_client(&handle, &port_id);
	snd_seq_event_t event;
	snd_seq_ev_set_source(&event, port_id);
	snd_seq_ev_set_subs(&event);
	snd_seq_ev_set_direct(&event);
	c = 1;
	note = 36;
	while(1){
		snd_seq_ev_set_noteon(&event, 0, note, 60);
		snd_seq_event_output(handle, &event);
		snd_seq_drain_output(handle);
		
		usleep(50000);
		
		snd_seq_ev_set_noteoff(&event, 0, note, 0);
		snd_seq_event_output(handle, &event);
		snd_seq_drain_output(handle);
		
		if(note == 96) c = -1;
		else if(note == 32) c = 1;
		note = note + c; 
	}
}
