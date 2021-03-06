/* Selman Tabet (@selmantabet - https://selman.io/) - UIN 724009859
Assignment 3 - Question 3

This function turns the LEDs in the sequence 1-2-4-3. When the user presses
the pushbutton, the LED sequence would run at a faster rate until the user
releases the button. Upon button release, all LEDs would turn off for
three seconds prior to resuming the main sequence.

ISR calls here cannot stack up as nested ISRs do, meaning that when an ISR
is running, all further interrupt signals are ignored until ISR exit.
In this case, pressing the button again while the LEDs are off from the last
registered button release would not cause anything.

Special functions that protect the integrity of the LED states prior to
main process interruption were designed.

Developed using the Mbed IDE. Tested on an EA LPC4088 QuickStart Board. */

#include "mbed.h"
#include <assert.h>

double interval = 1.0 ; //Time between LED state switches

//Create DigitalOut objects to control LED1-LED4:
DigitalOut my_led1(LED1); //Active Low
DigitalOut my_led2(LED2); //Active Low
DigitalOut my_led3(LED3); //Active High
DigitalOut my_led4(LED4); //Active High

//Create an InterruptIn object for the pushbutton:
InterruptIn Button(p23);

/*The following two functions work on saving the LED states prior to 
jumping into an ISR, then loaded back before RETI (Return from Interrupt),
this was implemented so that the LED states remain intact following each ISR.
The ISRs modify the LED states, so it is imperative to maintain
their integrity in the principal routine for a more graceful execution.*/

int encode_state(DigitalOut led1, DigitalOut led2, 
                DigitalOut led3, DigitalOut led4)
                {
                /*The function saves the current LED states for 
                future restoration. The function is operating-mode-agnostic
                i.e. it does not matter if the LEDs operated in
                Active-Low or Active-High modes*/
                    int state_seq = 0b0000; //Bit sequence led1,led2,led3,led4
                    if (led1 != 0) state_seq |= 0b1000;
                    if (led2 != 0) state_seq |= 0b0100;
                    if (led3 != 0) state_seq |= 0b0010;
                    if (led4 != 0) state_seq |= 0b0001;
                    return state_seq;
                }
                
void decode_state(int state_id){
    /*This function decodes and restores the LED states back to their
    original form.
    Assumed that the LEDs are declared externally. !!Non-modular function!! */
    my_led1 = (state_id & 0b1000) >> 3;
    my_led2 = (state_id & 0b0100) >> 2;
    my_led3 = (state_id & 0b0010) >> 1;
    my_led4 = (state_id & 0b0001);
    assert(encode_state(my_led1, my_led2, my_led3, my_led4) == state_id);
    //Ensure the integrity of the LED values
    }

void pushbutton_isr_press(void){
    int save_state = encode_state(my_led1, my_led2, my_led3, my_led4);
    interval = 0.25;
    decode_state(save_state); //Load pre-ISR state
}

void pushbutton_isr_release(void){
    int save_state = encode_state(my_led1, my_led2, my_led3, my_led4);
    my_led1 = 1; my_led2 = 1; my_led3 = 0; my_led4 = 0; wait(3); //All OFF.
    interval = 1.0;
    decode_state(save_state); //Load pre-ISR state
}

int main(){
    my_led1 = 1; my_led2 = 1; my_led3 = 0; my_led4 = 0; //All OFF.
    Button.mode(PullUp); //Setup a PullUp Resistor
    Button.fall(&pushbutton_isr_press); //Falling edge = Button Press ISR
    Button.rise(&pushbutton_isr_release); //Rising edge = Button Release ISR
    
    while(1) { //LED1-LED2-LED4-LED3 sequence.
        my_led1 = 0; wait(interval); my_led1 = 1;
        my_led2 = 0; wait(interval); my_led2 = 1;
        my_led4 = 1; wait(interval); my_led4 = 0;
        my_led3 = 1; wait(interval); my_led3 = 0;
    }
}
