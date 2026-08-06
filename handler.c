#include"header.h"
 extern u32 flag;
void temp_handler(u32 temperature)
{ 
lcd_cmd(0x01);
        lcd_str("HIGH TEMPERATURE ALERT");
		IOSET0=GREEN;
		IOCLR0=RED;
	    switch(temperature)
			   {

			   case 39: //pwm_duty(75);	
						uart0_str("\n\rPatient temperature going high: ");
					   	uart0_float(temperature);
						break;
			   case 40: //pwm_duty(100);	
						uart0_str("\n\rPatient temperature going high: ");
					  	uart0_float(temperature);
						break;
			   case 36: //pwm_duty(50);
			   			uart0_str("\n\rPatient temperature going low: ");
						uart0_float(temperature);
						break;
			   case 35: //pwm_duty(100);	
						uart0_str("\n\rPatient temperature going low: ");
					  	uart0_float(temperature);
						break;
			   default: //pwm_duty(0);
			           uart0_str("\n\r--------------Emergence-------------");
					   uart0_float(temperature); break;
			   }
}

void SpO2_handler(u32 SpO2)
{
IOSET0=GREEN;
IOCLR0=RED;
lcd_cmd(0x01);
lcd_str("OXYGEN LEVEL ALERT");
uart0_str("\n\rOXYGEN LEVEL ALERT\n\r");
if(SpO2 >110 && SpO2 < 120){ 
								           		
											uart0_str("\n\rPatient Oxygen going high: ");
										   	uart0_float(SpO2);
											}
		       else if(SpO2 >120 && SpO2 < 140){ 
								          	
											uart0_str("\n\rPatient Oxygen going very high: ");
										   	uart0_float(SpO2);
											}
			   else if(SpO2 >80 && SpO2 < 95){
								           	
											uart0_str("\n\rPatient Oxygen going low: ");
										   	uart0_float(SpO2);
										}
			  else if(SpO2 >70 && SpO2 < 80){ 
								          	
											uart0_str("\n\rPatient Oxygen going very low: ");
										   	uart0_float(SpO2);
										}

//			   else {
//			   uart0_str("\n\r--------------Emergence-------------");
//			
//					   uart0_float(SpO2); 
//			   }
}
  void emergence(void)
{
 int i=0;
 //lcd_cmd(0x01);
// circular_scroll_left("EMERGENCY ASSISTANCE REQUIRED",0xc0);
 uart0_str("\n\rPatient  is in emergence" );
 uart0_str("emergence button pressed");
// IOSET0=GREEN;
  /*for(i=0;i<100;i++)
  {
    IOCLR0=RED;
	delay_ms(250);
	IOSET0=GREEN;
  }	*/
 flag=0;
}

void BPM_handler(u32 bpm)
{
IOSET0=GREEN;
IOCLR0=RED;
lcd_cmd(0x01);
lcd_str("ABNORMAL HEARTBEAT DETECTED");
uart0_str("\n\rABNORMAL HEARTBEAT DETECTED\n\r");
if(bpm >100 && bpm < 110){ 
								           		
											uart0_str("\n\rPatient BPM going high: ");
										   	uart0_float(bpm);
											}
		       else if(bpm >120 && bpm < 140){ 
								          	
											uart0_str("\n\rPatient BPM going very high: ");
										   	uart0_float(bpm);
											}
			   else if(bpm >80 && bpm < 95){
								           	
											uart0_str("\n\rPatient BPM` going low: ");
										   	uart0_float(bpm);
										}
			  else if(bpm >70 && bpm < 80){ 
								          	
											uart0_str("\n\rPatient BPM going very low: ");
										   	uart0_float(bpm);
										}

//			   else {
//			   uart0_str("\n\r--------------Emergence-------------");
//			
//					   uart0_float(bpm); 
//			   }
}

void saline_handler(u32 dis)
{
	if(dis<=DIST_THRESHOLD)
	{
		IOSET0=RED|BUZZER;
		IOCLR0=GREEN;
		lcd_cmd(0x01);
		delay_ms(2);
		lcd_cmd(0x80);
		lcd_str("Patient Normal");
	}
	else if(dis>DIST_THRESHOLD)
	{
		IOSET0=GREEN|RED;
		IOCLR0=BUZZER;
		delay_sec(2);
		circular_scroll_left("Warning!! Low Saline Level",0xc0);
	}
}


void normal()
{
    lcd_str("PATIENT NORMAL");
	IOCLR0=GREEN;

}

