#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>

//As funções do ALSA costumam retornar valores negativos para indicar falhas de execução. Este #define permite encapsular as chamadas de função em uma estrutura que verifica esse retorno e
//exibe uma mensagem de erro  
#define CHK(stmt, msg) if((stmt) < 0) {puts("ERROR: "#msg); exit(1);}

//Parâmetros de entrada:
//handle: Varíavel que guarda o handle do sequenciador
//in_port_id: Variável que guarda o id da porta de entrada
//out_port_id: Variável que guarda o ida da porta de saída
//Descrição:
//Abre o ALSA Sequencer para entrada e saida e cria uma porta para cada função
int open_client(snd_seq_t** handle, int* in_port_id, int* out_port_id){
	CHK(snd_seq_open(handle, "default", SND_SEQ_OPEN_DUPLEX, 0), "Could not open sequencer");
	CHK(snd_seq_set_client_name(*handle, "Talker Client"), "Could not set client name");
	CHK(*out_port_id = snd_seq_create_simple_port(*handle, "O-port", SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, SND_SEQ_PORT_TYPE_APPLICATION), "Could not open port");
	CHK(*in_port_id = snd_seq_create_simple_port(*handle, "I-port", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION), "Could not open port");
	return 0;
}

//Parâmetros de entrada:
//seq_handle: Handle do sequenciador
//Descrição:
//Recebe o handle do sequenciador e retorna um evento mid recebido pela porta de entrada
snd_seq_event_t *midi_read(snd_seq_t* seq_handle)
{
	snd_seq_event_t* ev = NULL;
   	snd_seq_event_input(seq_handle, &ev);	
	return ev;
}

//Parâmetros de entrada:
//value: Valor de controle do evento de entrada
//vel: Velocidade do evento de saída
//note: Nota do evento de saida
//handle: Handle do sequenciador
//Descrição:
//Traduz um evento do tipo controlador para um evento do tipo nota de acordo com a tecla que foi apertada
void send_note(int value, unsigned char vel, unsigned char note, snd_seq_t* handle){
//Declaração do evento de saida	
	snd_seq_event_t out_ev;
//Este trecho é necessário para preparar um evento para envio 	
	snd_seq_ev_clear(&out_ev);
	snd_seq_ev_set_source(&out_ev, 0);
	snd_seq_ev_set_subs(&out_ev);
	snd_seq_ev_set_direct(&out_ev);
	snd_seq_ev_set_fixed(&out_ev);
//Quando um botão do controlador é apertado, ele envia um evento cujo valor de controle é 127
//Quando ele é solto, ele envia um segundo evento cujo o valor de controle é 0
//Assim, se o valor do controle for 0, o evento gerado pela tradução deve ser um NOTEOFF
	if(value == 0){
		out_ev.type = SND_SEQ_EVENT_NOTEOFF;
		out_ev.data.note.velocity = 0;
	}else{
		out_ev.type = SND_SEQ_EVENT_NOTEON;
		out_ev.data.note.velocity = vel;
	}
	out_ev.data.note.channel = 0;
	out_ev.data.note.note = note;
//Os dois comandos abaixo são utilizados para fazer o envio do evento criado
	snd_seq_event_output(handle, &out_ev);
	snd_seq_drain_output(handle);		
}

//Parâmetros de entrada:
//value: Valor de controle do evento de entrada
//vel: Velocidade da nota do evento de saída
//current_note: Nota que se encontra ativa
//handle: Handle do sequenciador
//Descrição:
//Essa é uma função similar a acima, porem adaptada para os botões de deslizar
//A ideia é que quando você mova o botão ele envie notas como se você estivesse deslizando a mão sobre teclas de piano
//A nota correspondente à posição que ele parou será mantida até que o botão seja movido de novo
//As notas só param quando o botão volta para a posição 0
int send_note_const(int value, unsigned char vel, unsigned char current_note, snd_seq_t* handle){
//Esse trecho é igual ao da função acima	
	snd_seq_event_t out_ev;
	snd_seq_ev_clear(&out_ev);
	snd_seq_ev_set_source(&out_ev, 0);
	snd_seq_ev_set_subs(&out_ev);
	snd_seq_ev_set_direct(&out_ev);
	snd_seq_ev_set_fixed(&out_ev);
//____________________________________________________________
	
//Primeiro o programa envia um NOTEOFF para a nota que está tocando atualmente	
	out_ev.type = SND_SEQ_EVENT_NOTEOFF;
	out_ev.data.note.channel = 0;
	out_ev.data.note.note = current_note;
	out_ev.data.note.velocity = 0;
	
	snd_seq_event_output(handle, &out_ev);
	snd_seq_drain_output(handle);

//Depois, se o valor de controle não for zero, ele envia um NOTEON para a nova nota que irá tocar
	if(value != 0){
		out_ev.type = SND_SEQ_EVENT_NOTEON;
		out_ev.data.note.channel = 0;
//Note que a nota é determinada pelo valor de controle do evento de entrada
//O botão de deslizar varia o valor entre 0 e 127
//As notas MIDI são determinadas por números também dentro deste intervalo
		out_ev.data.note.note = value;
		out_ev.data.note.velocity = vel;

		snd_seq_event_output(handle, &out_ev);
		snd_seq_drain_output(handle);
	}
}

int main(void){
//Declaração do handle(necessário para manipular o sequenciador) e do evento de entrada
	snd_seq_t* handle;
	snd_seq_event_t* ev;
//Essas variáveis guardam o número de identificação das portas de entrada e saida respectivamente
	int id_in, id_out;

//O vetor vel[] guarda a velocidade definida pelos controles designados à função de regulá-la
//O vetor note[] guarda a nota que está sendo mantida pelos botões de deslizar	
	unsigned char vel[8];
	unsigned char note[8];
//Inicialização dos vetores
	int i;
	for(i=0; i < 8; i++){
		vel[i] = 0;
		note[i] = 0;
	}
//Abertura do Sequenciador ALSA 
	open_client(&handle, &id_in, &id_out);
//Este switch mapeia as teclas do controlador MIDI que eu estou usando (nanoKONTROL2)
	while(1){
//Neste ponto o evento de entrada é lido		
		ev = midi_read(handle);
		if(ev->type != SND_SEQ_EVENT_CONTROLLER) continue;
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
				send_note(ev->data.control.value, vel[0], 60, handle);
			break;

			case 48:
				send_note(ev->data.control.value, vel[0], 61, handle);
			break;

			case 64:
				send_note(ev->data.control.value, vel[0], 62, handle);
			break;

			case 16:
				vel[0] = (unsigned char) ev->data.control.value;
			break;

			case 0:
				send_note_const(ev->data.control.value, vel[0], note[0], handle);
				note[0] = ev->data.control.value;
			break;

			case 33:
				send_note(ev->data.control.value, vel[1], 63, handle);
			break;

			case 49:
				send_note(ev->data.control.value, vel[1], 64, handle);
			break;

			case 65:
				send_note(ev->data.control.value, vel[1], 65, handle);
			break;

			case 17:
				vel[1] = (unsigned char) ev->data.control.value;
			break;

			case 1:
				send_note_const(ev->data.control.value, vel[1], note[1], handle);
				note[1] = ev->data.control.value;
			break;

			case 34:
				send_note(ev->data.control.value, vel[2], 66, handle);
			break;

			case 50:
				send_note(ev->data.control.value, vel[2], 67, handle);
			break;

			case 66:
				send_note(ev->data.control.value, vel[2], 68, handle);
			break;

			case 18:
				vel[2] = (unsigned char) ev->data.control.value;
			break;

			case 2:
				send_note_const(ev->data.control.value, vel[2], note[2], handle);
				note[2] = ev->data.control.value;
			break;

			case 35:
				send_note(ev->data.control.value, vel[3], 69, handle);
			break;

			case 51:
				send_note(ev->data.control.value, vel[3], 70, handle);
			break;

			case 67:
				send_note(ev->data.control.value, vel[3], 71, handle);
			break;

			case 19:
				vel[3] = (unsigned char) ev->data.control.value;
			break;

			case 3:
				send_note_const(ev->data.control.value, vel[3], note[3], handle);
				note[3] = ev->data.control.value;
			break;

			case 36:
				send_note(ev->data.control.value, vel[4], 72, handle);
			break;

			case 52:
				send_note(ev->data.control.value, vel[4], 73, handle);
			break;

			case 68:
				send_note(ev->data.control.value, vel[4], 74, handle);
			break;

			case 20:
				vel[4] = (unsigned char) ev->data.control.value;
			break;

			case 4:
				send_note_const(ev->data.control.value, vel[4], note[4], handle);
				note[4] = ev->data.control.value;
			break;

			case 37:
				send_note(ev->data.control.value, vel[5], 75, handle);
			break;

			case 53:
				send_note(ev->data.control.value, vel[5], 76, handle);
			break;

			case 69:
				send_note(ev->data.control.value, vel[5], 77, handle);
			break;

			case 21:
				vel[5] = (unsigned char) ev->data.control.value;
			break;

			case 5:
				send_note_const(ev->data.control.value, vel[5], note[5], handle);
				note[5] = ev->data.control.value;
			break;

			case 38:
				send_note(ev->data.control.value, vel[6], 78, handle);
			break;

			case 54:
				send_note(ev->data.control.value, vel[6], 79, handle);
			break;

			case 70:
				send_note(ev->data.control.value, vel[6], 80, handle);
			break;

			case 22:
				vel[6] = (unsigned char) ev->data.control.value;
			break;

			case 6:
				send_note_const(ev->data.control.value, vel[6], note[6], handle);
				note[6] = ev->data.control.value;
			break;

			case 39:
				send_note(ev->data.control.value, vel[7], 81, handle);
			break;

			case 55:
				send_note(ev->data.control.value, vel[7], 82, handle);
			break;

			case 71:
				send_note(ev->data.control.value, vel[7], 83, handle);
			break;

			case 23:
				vel[7] = (unsigned char) ev->data.control.value;
			break;

			case 7:
				send_note_const(ev->data.control.value, vel[7], note[7], handle);
				note[7] = ev->data.control.value;
			break;

			default:
				printf("Unknow controller\n");
		}
	}
	return 0;
}
