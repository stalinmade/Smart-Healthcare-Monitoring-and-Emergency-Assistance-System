#include"header.h"


void uart0_init(unsigned int baud)

{

	unsigned int pclk,d;

	if(VPBDIV==0)

		pclk=15000000;

	else if(VPBDIV==1)

		pclk=60000000;

	else if(VPBDIV==2)

		pclk=30000000;

	

	PINSEL0 |=0x05;

	d=pclk/(16*baud);

	U0LCR=0x83;

	U0DLL=d&0xff;

	U0DLM=(d>>8)&0xff;

	U0LCR=0x03;

}




void uart0_rx_buffer_ESPwifi(u8 *buf)

{

    int i=0;


    while(1)

    {

        buf[i]=uart0_rx();


        if(i>3)

        {

            if(buf[i]=='\n' &&  buf[i-1]=='\r')

			{

                break;

			}

        }


        i++;

    }


    buf[i]=0;

}





void uart0_tx(unsigned char data)

{

	U0THR=data;

	while(((U0LSR>>5)&1)==0);

}


unsigned char uart0_rx(void)

{

	while(((U0LSR>>0)&1)==0);

	return U0RBR;

}


void uart0_tx_string(char *p)

{

	while(*p)

	{

		uart0_tx(*p);

		//while((U0LSR>>5&1)==0);

		p++;

	}

}


void uart0_rx_string(unsigned char * s)

{

	int i=0;

	unsigned char t ;

	while((t=uart0_rx())!='\r')

	{

		uart0_tx(t);

		s[i]=t;

		i++;

	}

	s[i]=0;

}


//void uart0_adc_12_Bit(u32 bit)

//{

//	if(bit==0)

//	{

//		uart0_tx('0');

//		return;

//	}

//	

//	u8 a[4];

//	s32 i=0;

//	while(bit)

//	{

//		a[i]= bit%10;

//		i++;

//		bit/=10;

//	}

//	

//	while(--i >=0)

//	{

//		uart0_tx(a[i]+48);

//	}

//}


void uart0_float(float num)

{

	u32 intnum,decinum;

	s32 i=0;

	u8 a[7];

	if(num<0)

	{

		//uart0_tx_string("\r\n Num is -ve\r\n");

		return;

	}

	if(num==0)

	{

		uart0_tx('0');

		return;

	}

	intnum=num;

	decinum=((num-intnum)*10);


	if(intnum==0)

	{

		uart0_tx('0');

	}

	else {

		while(intnum)

	{

		a[i]=intnum%10;

		intnum/=10;

		i++;

	}

	while(i>0)

	{

		i--;

		uart0_tx(a[i]+48);

	}

}

	uart0_tx('.');


	if(decinum==0)

	{

		uart0_tx('0');

		return;

	}

	else {

	

	while(decinum)

	{

		a[i]=decinum%10;

		decinum/=10;

			i++;

	}


	while(i>0)

	{

		i--;

		uart0_tx(a[i]+48);

	}

}

	

}

		

void uart0_integer(s32 num)

{

	u32 a[10];

	s32 i=0;

	if(num < 0 || num == 0)

	{

		if( num == 0)

		{

			uart0_tx('0');

			return;

		}

			

	//	uart0_tx_string("\r\n Negative number \r\n");

		return;

	}

	while(num)

	{

		a[i]=num%10;

		num/=10;

		i++;

	}

	while(i)

	{

		--i;

		uart0_tx(a[i]+48);

	}

}


u32 uart0_rx_string_decimal(void)

{

		s32 i=0;

	  u32 d=0;

	u8 t,s[32];

	while((t=uart0_rx())!='\r')

	{

		uart0_tx(t);

		s[i]=t;

		i++;

	}

	s[i]=0;

	i=0;

	while(s[i])

	{

		d=(d*10)+(s[i]-48);

		i++;

	}

	return d;	

}


void uart0_hex(u8 num) //for 1 byte

{

u8 t;

t = (num>>4&0x0f);


   if(t<10)

   {

     uart0_tx(t+48);

   }

   else

   {

   uart0_tx(t-10+65);

   }

 t = (num&0x0f);


   if(t<10)

   {

     uart0_tx(t+48);

   }

   else

   {

   uart0_tx(t-10+65);

   }

  }
