#include "mbed.h"

// Create DigitalOut objects to control LED1-LED4:
DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

// Create an InterruptIn object for the pushbutton:
InterruptIn mypushpubutton(p23);

float speed = 1.0 ; // Initial value to speed
bool  flag = false;

void pushbutton_isr(void){
	if (flag == true){
		speed = 0.2;
    }
    else{
		speed = 1.0;
    }
    flag = !flag; //Invert flag
    wait(5);
}

int main() {
    myled1 = 1; // LED1 is active low, turn it off
    myled2 = 1; // LED2 is active low, turn it off
    myled3 = 0; // LED3 is active high, turn it off
    myled4 = 0; // LED4 is active high, turn it off
	mypushpubutton.mode(PullUp); // Setup a PullUp Resister
    mypushpubutton.rise(&pushbutton_isr); // Register an ISR on the rise edge
    while(1){
		myled1 = 0; //turn on 
        wait(speed);
        myled1 = 1; //turn off
        myled2 = 0; //turn on
        wait(speed);
        myled2 = 1;
        myled4 = 1; //turn on
        wait(speed);
        myled4 = 0;
        myled3 = 1; //turn on
        wait(speed);
        myled3 = 0;
        wait(speed);      
	}
}


