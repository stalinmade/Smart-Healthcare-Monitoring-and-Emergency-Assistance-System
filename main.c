#include"header.h"


volatile u8 timer_flag;
//u8  sample_buf[6];

#define LED 1<<3


//u32 ir_buffer[MY_BUFFER_SIZE], red_buffer[MY_BUFFER_SIZE];


u32 ir_buffer[100] = {

90000, 90040, 90080, 90120, 90160, 90200, 90240, 90280, 90320, 90360,

90400, 90440, 90480, 90520, 90560, 90600, 90640, 90680, 90720, 90760,

90800, 90840, 90880, 90920, 90960, 91000, 90960, 90920, 90880, 90840,

90800, 90760, 90720, 90680, 90640, 90600, 90560, 90520, 90480, 90440,

90400, 90360, 90320, 90280, 90240, 90200, 90160, 90120, 90080, 90040,

90000, 90040, 90080, 90120, 90160, 90200, 90240, 90280, 90320, 90360,

90400, 90440, 90480, 90520, 90560, 90600, 90640, 90680, 90720, 90760,

90800, 90840, 90880, 90920, 90960, 91000, 90960, 90920, 90880, 90840,

90800, 90760, 90720, 90680, 90640, 90600, 90560, 90520, 90480, 90440,

90400, 90360, 90320, 90280, 90240, 90200, 90160, 90120, 90080, 90040

};


u32 red_buffer[100] = {

70000, 70020, 70040, 70060, 70080, 70100, 70120, 70140, 70160, 70180,

70200, 70220, 70240, 70260, 70280, 70300, 70320, 70340, 70360, 70380,

70400, 70420, 70440, 70460, 70480, 70500, 70480, 70460, 70440, 70420,

70400, 70380, 70360, 70340, 70320, 70300, 70280, 70260, 70240, 70220,

70200, 70180, 70160, 70140, 70120, 70100, 70080, 70060, 70040, 70020,

70000, 70020, 70040, 70060, 70080, 70100, 70120, 70140, 70160, 70180,

70200, 70220, 70240, 70260, 70280, 70300, 70320, 70340, 70360, 70380,

70400, 70420, 70440, 70460, 70480, 70500, 70480, 70460, 70440, 70420,

70400, 70380, 70360, 70340, 70320, 70300, 70280, 70260, 70240, 70220,

70200, 70180, 70160, 70140, 70120, 70100, 70080, 70060, 70040, 70020

};


int main()

{

//u32 i;


float spo2;

float ratio;

float correlation;

s32 heart_rate;

s8 hr_valid, spo2_valid ;
s32 dis;
	u8 temp;


	lcd_init();
	lcd_cmd(0x01);
//	lcd_str("started");					      

	IODIR0 = WARNING_LED;

	uart0_init(9600);

i2c_init();							   

//MAX_init();	
	ultrasonic_init();
	DHT11_init();
uart0_tx_string("\r\n Started \r\n "	);

	

	   lcd_cmd(0x01);

		 lcd_cmd(0x80);

		 lcd_str("Starting....");

	

////**** Set wifi return status , 

	

/////////////////////////////////////////////////////////////////////////////////////////


	while(1)

	{

	   //dis=distance();
	   uart0_tx('b');
	   temp=DHT11_ReadTemperature();
	   uart0_tx('a');
	   if(sp02_sample_temp())
	   {

		       if (ir_buffer[0] < 50000) 

            {

                uart0_tx_string("\r\n Sensor not in use. Please place your finger.");

							  lcd_cmd(0xC0);

							  lcd_str("NO SENSOR");

            }

			else

       	{

    rf_heart_rate_and_oxygen_saturation( ir_buffer, BUFFER_SIZE, red_buffer, &spo2, &spo2_valid, &heart_rate, &hr_valid, &ratio, &correlation );	

			 

			 /// Heart rate 

						if(hr_valid)

						{

							uart0_tx_string("\r\n HR =  "	);

							uart0_integer(heart_rate); //printf("HR = %d", heart_rate);

							lcd_cmd(0x80);

							lcd_str("HR : ");

							lcd_integer(heart_rate);

								

						}

        				/// spo2

						if(spo2_valid)
						{
							  uart0_tx_string("  SPO2 =  "	);
							uart0_float(spo2); //printf("SpO2 = %.1f", spo2);
							lcd_cmd(0xC0);
							lcd_str("SPO2 : ");
							lcd_float(spo2);
			           	}	
						if(dis)
						{
						    uart0_tx_string("   saline =  "	);
							uart0_integer(dis); //printf("SpO2 = %.1f", spo2);
							uart0_tx_string("ml");
							lcd_cmd(0xC0);
							lcd_str("saline: ");
							lcd_integer(dis);
						}
						if(temp)
						{
						    uart0_tx_string("  temp =  "	);
							uart0_integer(temp); //printf("SpO2 = %.1f", spo2);
							uart0_tx_string(".c");
							lcd_cmd(0xC0);
							lcd_str("temp: ");
							lcd_integer(temp);
						}

					}
	 }

	 delay_ms(500);

	}

}

	

