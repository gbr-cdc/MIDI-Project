#include "joystick.h"
#include "MIDI_functions.c"


void joystick_callback(t_mosaic_button_event *event){

}

void register_callback(char * device,
                char * name,
                char number_of_axes,
                char number_of_buttons,
                int driver_version){

}


int main(int argc, char *argv[]) {
    joystick_inicialize("/dev/input/js0", &joystick_callback, &register_callback);
    while(1) usleep(10000);
    return 0;
}
