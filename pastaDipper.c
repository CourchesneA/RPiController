#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

const int pwmPin = 18; //Broadcom pin
const int AIN1Pin = 23; //Output pins, plugged to controller driver inputs
const int AIN2Pin = 24;
const int STBYPin = 25;
const int pwmMaxIntensity = 1000; //Real max is 1024 but limiting to 1000 for security reasons
const int speed1 = 150;
const int speed2 = 300;
const int speed3 = 450;
const int speed4 = 600;
const int speed5 = 750;

void sig_handler(int signo);
void clean();

int main(int argc, char *argv[])
{
 //catch exit conditions
 if (signal(SIGINT, sig_handler) == SIG_ERR)
 {
     printf("\ncan't catch SIGINT\n");
 }
 /*if (signal(SIGKILL, sig_handler) == SIG_ERR)
 {
     printf("\ncan't catch SIGKILL\n");
 }
 if (signal(SIGSTOP, sig_handler) == SIG_ERR)
 {
     printf("\ncan't catch SIGSTOP\n");
 }*/


 printf ("Raspberry Pi winringPi PWM test program \n");
 wiringPiSetupGpio();   //initialize wiringPi with broadcom GPIO
 
 int waitTime, opTime, power; 
 if(argc == 1 ){
     opTime = 2000;
     waitTime = 7000;
     power = 400;
 }else{
    if (sscanf (argv[1], "%i", &waitTime)!=1) {
        printf ("error - arg 1 not an integer\n"); 
        exit(0);
    }
    if (sscanf (argv[2], "%i", &opTime)!=1) {
        printf ("error - arg 2 not an integer\n");
        exit(0);
    }
    if (sscanf (argv[3], "%i", &power)!=1) {
        printf ("error - arg 3 not an integer\n");
        exit(0);
    }
 }

 printf("Got prams:\nwaitTime: %d\nopTime: %d\npower: %d\n\n",waitTime,opTime,power);

 //Pin setup
 printf("Configuring the pins\n\n");
 pinMode (pwmPin,PWM_OUTPUT);
 pinMode (AIN1Pin,OUTPUT);
 pinMode (AIN2Pin,OUTPUT);
 pinMode (STBYPin,OUTPUT);

 pullUpDnControl(AIN1Pin,PUD_DOWN);
 pullUpDnControl(AIN2Pin,PUD_DOWN);
 pullUpDnControl(STBYPin,PUD_DOWN);

 digitalWrite(AIN1Pin,1);
 digitalWrite(AIN2Pin,0);
 digitalWrite(STBYPin,0);
 pwmWrite(pwmPin,power);

 //Running sequence
 
 //Lower the rope
 printf("Lowering the rope\n\n");
 digitalWrite(STBYPin,1);
 delay(opTime);
 digitalWrite(STBYPin,0);
 //reverse
 printf("Waiting for %d ms\n\n",waitTime);
 delay(50);
 digitalWrite(AIN1Pin,0);
 digitalWrite(AIN2Pin,1);
 delay(50);
 //wait
 delay(waitTime);
 //Bring back the pastas
 printf("Raising the rope\n\n");
 digitalWrite(STBYPin,1);
 delay(opTime);
 digitalWrite(STBYPin,0);

 
 printf("\nCleaning everything for shutdown\n");
 clean();
 printf("Execution successful.\n");
}

void clean(){
 digitalWrite(AIN1Pin,0);
 digitalWrite(AIN2Pin,0);
 digitalWrite(STBYPin,0);

 pinMode (pwmPin,INPUT);
 pinMode (AIN1Pin,INPUT);
 pinMode (AIN2Pin,INPUT);
 pinMode (STBYPin,INPUT);

 pullUpDnControl(AIN1Pin,PUD_OFF);
 pullUpDnControl(AIN2Pin,PUD_OFF);
 pullUpDnControl(STBYPin,PUD_OFF);

 exit(0);
}

void sig_handler(int signo)
{
    printf("Received signal %s, cleaning GPIO and exiting\n",signo);
    clean();
}
