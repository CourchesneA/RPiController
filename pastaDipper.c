#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>

const int pwmPin = 18; //Broadcom pin
const int AIN1Pin = 23; //Output pins, plugged to controller driver inputs
const int AIN2Pin = 24;
const int STBYPin = 25;
const int pwmMaxIntensity = 1000; //Real max is 1024 but limiting to 1000 for security reasons
int main(void)
{
 //catch exit conditions
 if (signal(SIGINT, sig_handler) == SIG_ERR)
 {
     printf("\ncan't catch SIGINT\n");
 }
 if (signal(SIGKILL, sig_handler)) == SIG_ERR)
 {
     printf("\ncan't catch SIGKILL\n");
 }
 if (signal(SIGSTROP, sig_handler)) == SIG_ERR)
 {
     printf("\ncan't catch SIGSTOP\n")'
 }


 printf ("Raspberry Pi winringPi PWM test program \n");
 wiringPiSetupGpio();   //initialize wiringPi with broadcom GPIO
 //Pin setup
 pinMode (pwmPin,PWM_OUTPUT);
 pinMode (AIN1Pin,OUTPUT);
 pinMode (AIN2Pin,OUTPUT);
 pinMode (STBYPin,OUTPUT);

 pullUpDnControl(AIN1Pin,PUD_DOWN);
 pullUpDnControl(AIN2Pin,PUD_DOWN);
 pullUpDnControl(STBYPin,PUD_DOWN);

 digitalWrite(AIN1Pin,1);
 digitalWrite(AIN2Pin,0);
 digitalWrite(STBYPin,1);
 pwmWrite(pwmPin,0.3*pwmMaxIntensity);
 delay(2000);
 digitalWrite(STBYPin,0);
 delay(2000);
 digitalWrite(STBYPin,1);
 delay(2000); 

}

void clean(){
 pinMode (pwmPin,INPUT);
 pinMode (AIN1Pin,INPUT);
 pinMode (AIN2Pin,INPUT);
 pinMode (STBYPin,INPUT);

 pullUpDnControl(AIN1Pin,PUD_OFF);
 pullUpDnControl(AIN2Pin,PUD_OFF);
 pullUpDnControl(STBYPin,PUD_DOWN);

 exit(0);
}

void sig_handler(int signo)
{
    printf("Received signal %s, cleaning GPIO and exiting\n",signo);
    clean();
}
