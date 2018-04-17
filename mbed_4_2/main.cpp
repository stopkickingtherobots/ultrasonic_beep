/*
Name:       Benn Hamer
Task:       4.2
Objective:  PWM Buzzer and I2C Ultrasonic

Terminal Setup for Serial Monitor

Copy pasta this into terminal to get usb port n#
ls /dev/tty.usbmodem*

It will return the end digits for the following command

Then copy pasta:
screen /dev/tty.usbmodem1422
*/

#include "mbed.h"
#include "ultrasonic.h"
 
DigitalOut trigger(p6);
DigitalOut myled(LED1); //monitor trigger
DigitalOut myled2(LED2); //monitor echo
DigitalIn  echo(p7);
int distance = 0;
int correction = 0;
int far = 45;
int close = 30;
int closest = 15;
int i;
Timer sonar;
PwmOut speaker(p21);
 
int main()
{
    sonar.reset();
// measure actual software polling timer delays
// delay used later in time correction
// start timer
    sonar.start();
// min software polling delay to read echo pin
    while (echo==2) {};
    myled2 = 0;
// stop timer
    sonar.stop();
// read timer
    correction = sonar.read_us();
    printf("Approximate software overhead timer delay is %d uS\n\r",correction);
 
//Loop to read Sonar distance values, scale, and print
    while(1) {
// trigger sonar to send a ping
        trigger = 1;
        myled = 1;
        myled2 = 0;
        sonar.reset();
        wait_us(10.0);
        trigger = 0;
        myled = 0;

//wait for echo high
        while (echo==0) {};
        myled2=echo;

//echo high, so start timer
        sonar.start();
//wait for echo low
        while (echo==1) {};

//stop timer and read value
        sonar.stop();

//subtract software overhead timer delay and scale to cm
        distance = (sonar.read_us()-correction)/58.0;
        myled2 = 0;
        
        if(distance <= far && distance > close)
        {
            printf(" Far \n\r");
            printf(" %d cm \n\r",distance);
            
            speaker.period(1.0/250.0); // 250hz period
            speaker =0.5; //50% duty cycle - max volume
            wait(3);
            speaker=0.0; // turn off audio
            wait(2);
        }
        else if(distance <= close && distance > closest)
        {
            printf(" Close \n\r");
            printf(" %d cm \n\r",distance);
            
            speaker.period(1.0/500.0); // 500hz period
            speaker =0.5; //50% duty cycle - max volume
            wait(3);
            speaker=0.0; // turn off audio
            wait(1);  
        }
        else if(distance <= closest)
        {
            printf(" Closest \n\r");
            printf(" %d cm \n\r",distance);
            
            speaker.period(1.0/750.0); // 750hz period
            speaker =0.5; //50% duty cycle - max volume
            wait(2);
            speaker=0.0; // turn off audio
        }
        else if(distance > far)
        {
            printf(" No object \n\r");
            printf(" %d cm \n\r",distance);
            
            wait(2);
        }
        else
        {
            printf(" Error reading distance :( \n\r");
            wait(2);
        }
        
//wait so that any echo(s) return before sending another ping
        wait(1);
    }
}