//#include<lpc21xx.h>
//#include"header.h"
//#define TRIG_PIN (1<<20)
//#define ECHO_PIN (1<<23)
//int trig;
//void ultrasonic_init(void)
//{
//IODIR0|=TRIG_PIN;
//IODIR0&=~(ECHO_PIN);
//T0PR=59;
//T0TCR=0;
//}
//
//unsigned int distance(void)
//{
//unsigned int time=0;
//unsigned int dis=0;
//unsigned int val=0;
//IOSET0=TRIG_PIN;
//delay_us(10);
//IOCLR0=TRIG_PIN;
////val=(IOPIN0&(ECHO_PIN));
////uart0_str("\r\nval:"); 
////uart0_int(val);
//while ((IOPIN0 & ECHO_PIN) == 0)
//{
//    if (T0TC > 30000) return 0;   // Timeout (30 ms)
//}
//T0TC=0;
//T0TCR=0X01;
//while ((IOPIN0 & ECHO_PIN) == 0)
//{
//    if (T0TC > 30000) return 0;   // Timeout (30 ms)
//}
//T0TCR=0;
//time=T0TC;
//dis=time/58;
////dis=((c*2.0)/58)/30.48;
//return dis;
//}

 #include<lpc21xx.h>
#include"header.h"
#define TRIG_PIN (1<<20)
#define ECHO_PIN (1<<23)
int trig;
void ultrasonic_init(void)
{
IODIR0|=TRIG_PIN;
IODIR0&=~(ECHO_PIN);
T0TCR=2;
T0PR=59;
T0TCR=0;
}

unsigned int distance(void)
{
unsigned int time=0;
unsigned int dis=0;
unsigned int val=0;
IOSET0=TRIG_PIN;
delay_us(10);
IOCLR0=TRIG_PIN;
uart0_str("abcd \r\n ");
while ((IOPIN0 & (ECHO_PIN))==0);
uart0_str("fghi\r\n ");
T0TCR=0X02;
T0TC=0;
T0TCR=0X01;
uart0_str("pqrs \r\n ");
val=(IOPIN0&(ECHO_PIN));
uart0_str("\r\nval:"); 
uart0_int(val);
while (!(IOPIN0 & (ECHO_PIN)));
uart0_str("mnop \r\n ");
T0TCR=0X00;
time=T0TC;
dis=time/58;
//dis=((c*2.0)/58)/30.48;
return dis;
}
