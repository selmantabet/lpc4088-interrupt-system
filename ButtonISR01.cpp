#include "mbed.h"

// Create DigitalOut objects to control LED1-LED4:
DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

// Create an InterruptIn object for the pushbutton:
InterruptIn mypushpubutton(p23);

void pushbutton_isr(void){
        myled1 = 0; //turn on 
        wait(0.2);
        myled1 = 1;
        myled2 = 0; // turn on
        wait(0.2);
        myled2 = 1;
        myled4 = 1;
        wait(0.2);
        myled4 = 0;
        myled3 = 1;
        wait(0.2);
        myled3 = 0;
        wait(0.2);
}

int main(){
    myled1 = 1; // LED1 is active low, turn it off
    myled2 = 1; // LED2 is active low, turn it off
    myled3 = 0; // LED3 is active high, turn it off
    myled4 = 0; // LED4 is active high, turn it off
    mypushpubutton.mode(PullUp); // Setup a PullUp Resister
    mypushpubutton.rise(&pushbutton_isr); // Register an ISR on the rise edge
    while(1) {
		myled1 = 0; //turn on 
        wait(1);
        myled1 = 1;
        myled2 = 0; // turn on
        wait(1);
        myled2 = 1;
        myled4 = 1;
        wait(1);
        myled4 = 0;
        myled3 = 1;
        wait(1);
        myled3 = 0;
        wait(1);      
    }
}


