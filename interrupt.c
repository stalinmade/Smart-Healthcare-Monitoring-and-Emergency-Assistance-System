#include"header.h"
//extern u32 flag;
void eint0_handler()__irq
{
//emergence();
EXTINT=1;
//flag=1;	
VICVectAddr=0;
}

void config_eint0()
{
PINSEL1|=0x1;
EXTMODE=1;
EXTPOLAR=0;
VICIntSelect=0;
VICVectCntl0=14|(1<<5);
VICVectAddr0=(u32)eint0_handler;
VICIntEnable=1<<14;
}

