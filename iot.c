#include"header.h"


// MAX30102

volatile u8 sample_buf[6];




extern u32 ir_buffer[MY_BUFFER_SIZE], red_buffer[MY_BUFFER_SIZE];

static u32 index=0;




/////// WARNING ////////


void warning (void)

{

  IOCLR0 =  WARNING_LED;

	IOCLR0 = BUZZER;

	lcd_cmd(0x01);

	lcd_cmd(0x80);

	lcd_str("WARNING");

	//can use an switch interrupt here to reset from this while(sw==0); inside isr flag=1;

}

	


void MAX_init(void)

{

uart0_tx_string("\r\n IN MAX_init...\r\n");





  // mode configartion 0x09

  i2c_write(0xAE,0x09,0x40);

  do

  {

   uart0_tx_string("\r\n Checking power ready \r\n");

   uart0_tx_string("\r\n Power ready = ");

   uart0_hex(i2c_read(0xae,0x00));

   }while(i2c_read(0xae,0x09) & 0x40);


   // FIFO configaration(0X08)

 //i2c_write(0xAE,0x08,0xB0);   //Sample averaging = 32 samples, FIFO rollover enabled

  i2c_write(0xAE,0x08,0x00);   //Sample averaging = 0 samples, FIFO rollover disabled




  // spo2 configaration 0x0A

   i2c_write(0xAE,0x0A,0x27);


  // LED pulse amplitude ( 0x0C - 0X0D)

   i2c_write(0xAE,0x0C,0x3F);

   i2c_write(0xAE,0x0D,0x24);


 i2c_write(0xAE,0x09,0x03);


  // clearing the FIFO_WR_PTR, OVF_COUNTER, and FIFO_RD_PTR

i2c_write(0xAE,0x04,0x00);

//uart0_tx_string("\r\n read_FIFO_WR_PTR = ");

//uart0_hex(read_FIFO_WR_PTR());

//

//uart0_tx_string("\r\n read_OVR_COUNTER = ");

i2c_write(0xAE,0x05,0x00);

//uart0_hex(read_OVR_COUNTER());


i2c_write(0xAE,0x06,0x00);

//uart0_tx_string("\r\n read_FIFO_RD_PTR = ");

//uart0_hex(read_FIFO_RD_PTR());

//

//uart0_tx_string("\r\n Power ready = ");

//uart0_hex(i2c_read(0xae,0x00));




// Interrupt ******


 i2c_write(0xAE,0x02,0xE0);

 i2c_write(0xAE,0x03,0x02);




 uart0_tx_string("\r\nMAX_init completed...\r\n");

 }




u8 sp02_sample_temp(void)

{

    //u8 wr_ptr,rd_ptr , no_of_available_samples,full=0;

    u32 red_raw, IR_raw ;

        u8 i=0;


//uart0_tx_string("\r\n read_FIFO_WR_PTR = ");

//uart0_intiger(read_FIFO_WR_PTR());

//

//uart0_tx_string("\r\n read_OVR_COUNTER = ");

//uart0_intiger(read_OVR_COUNTER());

//

//uart0_tx_string("\r\n read_FIFO_RD_PTR = ");

//uart0_intiger(read_FIFO_RD_PTR());




   while(i<32)

   {

   i2c_burst_read(0xAE,0x07);

   red_raw = (sample_buf[0] & 0x03)<<16 | (sample_buf[1]<<8) | sample_buf[2] ;

   IR_raw = (sample_buf[3] & 0x03)<<16 | (sample_buf[4]<<8) | sample_buf[5] ;

//   uart0_tx_string("\r\n red_raw : ");

//   uart0_intiger(red_raw);

//   uart0_tx_string("  | IR_raw : ");

//   uart0_intiger(IR_raw);


         ir_buffer[index]=IR_raw;

   red_buffer[index]=red_raw;


index++;

if(index==(MY_BUFFER_SIZE))

{

 //uart0_tx_string("\r\n buffer =400 \r\n ");

 index=0;

 return 1;

}

i++;

   }

//uart0_tx_string("\r\n index : ");

//uart0_intiger(index);

         return 0;

}

u8 sp02_sample(void)

{

    u8 wr_ptr,rd_ptr , no_of_available_samples,full=0;

    u32 red_raw, IR_raw ;


uart0_tx_string("\r\n ******** in sample... ************\r\n");


    wr_ptr = read_FIFO_WR_PTR();

    rd_ptr = read_FIFO_RD_PTR();


    if( wr_ptr > rd_ptr)

    {

                uart0_tx_string("\r\n Inside if \r\n ");

      no_of_available_samples =  wr_ptr - rd_ptr;

    }

    else if ( wr_ptr < rd_ptr )

    {

                uart0_tx_string("\r\n Inside else if \r\n ");

       no_of_available_samples =  32 + wr_ptr - rd_ptr;

    }

    else

    {

                uart0_tx_string("\r\nINT1 = ");

uart0_hex(i2c_read(0xAE, 0x00));


uart0_tx_string("\r\nWR = ");

uart0_integer(read_FIFO_WR_PTR());


uart0_tx_string("\r\nRD = ");

uart0_integer(read_FIFO_RD_PTR());

                uart0_tx_string("\r\n Inside else \r\n ");

    return 0;

    }


         uart0_tx_string("\r\n wr_ptr = ");

         uart0_integer(wr_ptr);

                 uart0_tx_string("\r\n rd_ptr = ");

         uart0_integer(rd_ptr);

                 uart0_tx_string("\r\n no. of samples = ");

         uart0_integer(no_of_available_samples);





   while(no_of_available_samples)

   {


   i2c_burst_read(0xAE,0x07);

   red_raw = (sample_buf[0] & 0x03)<<16 | (sample_buf[1]<<8) | sample_buf[2] ;

   IR_raw = (sample_buf[3] & 0x03)<<16 | (sample_buf[4]<<8) | sample_buf[5] ;

//   uart0_tx_string("\r\n red_raw : ");

//   uart0_intiger(red_raw);

//   uart0_tx_string("  | IR_raw : ");

//   uart0_intiger(IR_raw);


         ir_buffer[index]=IR_raw;

   red_buffer[index]=red_raw;


index++;

if(index>=(MY_BUFFER_SIZE))

{

 uart0_tx_string("\r\n buffer =600 \r\n ");

 index=0;

 full =1;

}


   no_of_available_samples -- ;

   }

uart0_tx_string("\r\n index : ");

uart0_integer(index);

         return full;

}




 u8 temp(void)

 {

 u8 temp_fraction;

   i2c_write(0xAE,0x21,0x01);

   while( i2c_read(0xAE,0x21) &1);

   temp_fraction = i2c_read(0xAE,0x20);

   return ( i2c_read(0xAE,0x1F) + ( temp_fraction & 0x0F) );

}




u8 read_FIFO_WR_PTR(void)

{

u8 res;

 res = i2c_read(0xAE,0x04);

 return   res;// & 0x0f;


}


 u8 read_OVR_COUNTER(void)

{

  u8 res;

 res = i2c_read(0xAE,0x05);

 return   res ;//& 0x0f;


}


u8 read_FIFO_RD_PTR(void)

{

  u8 res;

 res = i2c_read(0xAE,0x06);

 return   res ;//& 0x0f;


}




u32 average(u32 * b)

{

  u32 av=0,i;


  for( i=0;i<MY_BUFFER_SIZE;i++)

        {

                 av=av+b[i];

        }

        return av/MY_BUFFER_SIZE;

}
