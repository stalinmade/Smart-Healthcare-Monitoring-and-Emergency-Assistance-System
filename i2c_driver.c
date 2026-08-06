#include"header.h"


#define SI ((I2CONSET>>3)&1)

extern volatile u8 sample_buf[6];
 
void i2c_init(void)

{

        // *** I2C Bit freq, setting ***

        // Bit freq. = FCLK/(I2SCLL + I2SCLH)

        //(I2SCLL + I2SCLH) = 15000000/100000; setting as 100kBps


        PINSEL0 |= (1<<4) | (1<<6); //SCL = P0.2 & SDA = P0.3

        I2SCLL = (15000000/100000)/2;

        I2SCLH = (15000000/100000)/2;

        I2CONSET = 1<<6 ;

        I2CONCLR = 1<<2;

}




void i2c_burst_read(u32 slave_addr, u32 memory_addr)

{

       //u8 result;

       s8 i,t=0;


        // Generate start condition

        I2CONSET = (1<<5); // STA =1;

		I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x08) // checking for start condition

        {

                uart0_tx_string("\r\n ERROR : START condition \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        I2CONCLR = (1<<5); // STA =0;




        // Sending SA+W

        I2DAT = slave_addr  ; //SA+W

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x18) // checking for SA+W,ACK

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , I2STAT != 0x18 \r\n");

                goto exit; //need to change to goto exit or somthing

        }

        if(I2STAT == 0x20) // checking for SA+W,NOT ACK

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

                goto exit; //need to change to goto exit or somthing

        }




        // Sending memory addr

        I2DAT = memory_addr ;

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

		if(I2STAT != 0x28)   //  Data/Memory_addr  has been transmitted expected status

        {

                uart0_tx_string("\r\n ERROR : memory_addr \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        if(I2STAT == 0x30)   //  Data/Memory_addr  has been transmitted; NOT ACK has been received

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

                goto exit; //need to change to goto exit or somthing

        }




        // Sending Restart condition      

        I2CONSET = (1<<5); // STA =1;

				I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x10)   //  Reapeted Start condition failed transmitted

        {

                uart0_tx_string("\r\n ERROR : Restart condition \r\n");

                goto exit; //need to change to goto exit or somthing

        }

        I2CONCLR = (1<<5); // STA =0;




        // Sending SA+R

        I2DAT = slave_addr^1  ; //SA+R

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x40) // Expected status

        {

                uart0_tx_string("\r\n ERROR : SA + W failed, I2STAT != 0x40 \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        if(I2STAT == 0x48) // checking for SA+W,NOT ACK

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

 for(i=0;i<5;i++)

 {


  // Geting data  & sending ACK 

   I2CONSET=(1<<2); // seting AA=1

   I2CONCLR = (1<<3); // clearing SI flag

   while(SI==0);

        if(I2STAT == 0x58) // Data byte has been received; NOT ACK has been returned

        {

                uart0_tx_string("\r\n ERROR : Data byte has been received; NOT ACK has been returned \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        if(I2STAT != 0x50) // Data byte has been received; ACK has been returned

        {

                uart0_tx_string("\r\n ERROR : ACK \r\n");

                goto exit ; //need to change to goto exit or somthing

        }


//        uart0_tx_string("\r\n llll Byte = ");

//        uart0_hex( I2DAT);

        sample_buf[t++]= I2DAT;




  }


        //getting last data byte

        I2CONCLR = 1<<2;

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x58) // Data byte has been received; NOT ACK has been returned

        {

                uart0_tx_string("\r\n ERROR : DATA reception \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

//        uart0_tx_string("\r\n last Byte = ");

//        uart0_hex( I2DAT);

        sample_buf[t++]= I2DAT;

        sample_buf[t]= 0;

				exit:

        I2CONSET = (1<<4);

        I2CONCLR = (1<<3);

        while((I2CONSET>>4&1) == 1);

				//uart0_tx_string("\r\n $$$$$$  READ COMPLETE $$$$$$...\r\n");




}


void i2c_write(u32 slave_addr, u32 memory_addr , u8 data)

{

        // Generate start condition

		

        I2CONSET = (1<<5); // STA =1;

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x08) // checking for start condition

        {

                uart0_tx_string("\r\n ERROR : START condition \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        I2CONCLR = (1<<5); // STA =0;




         // sending SA+W

        I2DAT = slave_addr ; //SA+W

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

				

//				uart0_tx_string("\r\n I2STAT =  ");

//				uart0_intiger(I2STAT);

//				uart0_tx_string("\r\n");

				

         if(I2STAT != 0x18) // checking for SA+W,ACK

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , I2STAT != 0x18 \r\n");

                goto exit ; //need to change to goto exit or somthing

        }


        if(I2STAT == 0x20) // checking for SA+W,NOT ACK

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

                goto exit ; //need to change to goto exit or somthing

        }


        // Sending memory addr


        I2DAT = memory_addr ;

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x28)   //  Data/Memory_addr  has been transmitted expected status

        {

                uart0_tx_string("\r\n ERROR : memory_addr \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        if(I2STAT == 0x30)   //  Data/Memory_addr  has been transmitted; NOT ACK has been received

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

                goto exit ; //need to change to goto exit or somthing

        }


        //Send data

        I2DAT = data ;

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x28)   //  Expected status

        {

                uart0_tx_string("\r\n ERROR : Data , I2STAT != 0x28 \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        if(I2STAT == 0x30)   //  Data/Memory_addr  has been transmitted; NOT ACK has been received

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

				exit:

        I2CONSET = (1<<4);

        I2CONCLR = (1<<3);

        while((I2CONSET>>4&1) == 1);

				//uart0_tx_string("\r\n !!!!!!  Write COMPLETE !!!!!...\r\n");

}







u8 i2c_read(u32 slave_addr, u32 memory_addr)

{

        u8 result;


        // Generate start condition

        I2CONSET = (1<<5); // STA =1;

		I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        I2CONCLR = (1<<5); // STA =0;




        // Sending SA+W

        I2DAT = slave_addr  ; //SA+W

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x18) // checking for SA+W,ACK

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , I2STAT != 0x18 \r\n");

                goto exit; //need to change to goto exit or somthing

        }

        if(I2STAT == 0x20) // checking for SA+W,NOT ACK

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

                goto exit; //need to change to goto exit or somthing

        }




        // Sending memory addr

        I2DAT = memory_addr ;

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

		if(I2STAT != 0x28)   //  Data/Memory_addr  has been transmitted expected status

        {

                uart0_tx_string("\r\n ERROR : memory_addr \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        if(I2STAT == 0x30)   //  Data/Memory_addr  has been transmitted; NOT ACK has been received

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

                goto exit; //need to change to goto exit or somthing

        }




        // Sending Restart condition      

        I2CONSET = (1<<5); // STA =1;

				I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x10)   //  Reapeted Start condition failed transmitted

        {

                uart0_tx_string("\r\n ERROR : Restart condition \r\n");

                goto exit; //need to change to goto exit or somthing

        }

        I2CONCLR = (1<<5); // STA =0;




        // Sending SA+R

        I2DAT = slave_addr^1  ; //SA+R

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x40) // Expected status

        {

                uart0_tx_string("\r\n ERROR : SA + W failed, I2STAT != 0x40 \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        if(I2STAT == 0x48) // checking for SA+W,NOT ACK

        {

                uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

                goto exit ; //need to change to goto exit or somthing

        }


  // Geting data

    I2CONCLR = 1<<2;

        I2CONCLR = (1<<3); // clearing SI flag

        while(SI==0);

        if(I2STAT != 0x58) // checking for 0x58 status

        {

                uart0_tx_string("\r\n ERROR : DATA reception \r\n");

                goto exit ; //need to change to goto exit or somthing

        }

        result = I2DAT;

				exit:

//        I2CONSET = (1<<4);

//        I2CONCLR = (1<<3);

//        delay_ms(100);

        I2CONSET = (1<<4);

        I2CONCLR = (1<<3);

        while((I2CONSET>>4&1) == 1);

				//uart0_tx_string("\r\n $$$$$$  READ COMPLETE $$$$$$...\r\n");

        return result;


}



















//#include"header.h"


//#define SI ((I2CONSET>>3)&1)




//void i2c_init(void)

//{

//	// *** I2C Bit freq, setting ***

//	// Bit freq. = FCLK/(I2SCLL + I2SCLH)

//	//(I2SCLL + I2SCLH) = 15000000/100000; setting as 100kBps

//	

//	I2SCLL = (15000000/100000)/2;

//	I2SCLH = (15000000/100000)/2;

//	

//	I2CONSET = 1<<6 ;

//	I2CONCLR = 1<<2;

//	

//	PINSEL0 |= (1<<4) | (1<<6); //SCL = P0.2 & SDA = P0.3

//}


//void i2c_write(u32 slave_addr, u32 memory_addr , u8 data)

//{ 

//	// Generate start condition 

//	uart0_tx_string("\r\n Write start...\r\n");

//	

//	

//	I2CONSET = (1<<5); // STA =1;

//	I2CONCLR = (1<<3); // clearing SI flag

//	while(SI==0);

//	if(I2STAT != 0x08) // checking for start condition

//	{

//		uart0_tx_string("\r\n ERROR : START condition \r\n");

//		goto exit ; //need to change to goto exit or somthing

//	}

//	I2CONCLR = (1<<5); // STA =0;


//	

//	

//	I2DAT = slave_addr ; //SA+W

//	I2CONCLR = (1<<3); // clearing SI flag

//	while(SI==0);

//	

//	if(I2STAT == 0x20) // checking for SA+W,NOT ACK

//	{

//		uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

//		goto exit ; //need to change to goto exit or somthing

//	}

//		uart0_tx_string("\r\n Write SA...\r\n");

//	

//	// Sending memory addr

//	

//	I2DAT = memory_addr ;

//	I2CONCLR = (1<<3); // clearing SI flag

//	while(SI==0);

//	if(I2STAT == 0x30)   //  Data/Memory_addr  has been transmitted; NOT ACK has been received

//	{

//		uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

//		goto exit ; //need to change to goto exit or somthing

//	}

//	uart0_tx_string("\r\n Write mr...\r\n");

//	

//	//Send data 

//	I2DAT = data ;

//	I2CONCLR = (1<<3); // clearing SI flag

//	while(SI==0);

//	if(I2STAT == 0x30)   //  Data/Memory_addr  has been transmitted; NOT ACK has been received

//	{

//		uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

//		goto exit ; //need to change to goto exit or somthing

//	}

//	uart0_tx_string("\r\n Write data...\r\n");

//	

//	I2CONSET = (1<<4);

//	I2CONCLR = (1<<3);

//	delay_ms(100);

//}




//u8 i2c_read(u32 slave_addr, u32 memory_addr)

//{ 

//	u8 result;

//	

//	uart0_tx_string("\r\n Read start...\r\n");

//	

//	// Generate start condition 

//	I2CONCLR = (1<<3); // clearing SI flag

//	I2CONSET = (1<<5); // STA =1;

//	while(SI==0);

//	I2CONCLR = (1<<5); // STA =0;

//	

//		uart0_tx_string("\r\n Read start complete...\r\n");

//	

//	// Sending SA+W

//	I2DAT = slave_addr  ; //SA+W

//	I2CONCLR = (1<<3); // clearing SI flag

//	while(SI==0);

//	if(I2STAT == 0x20) // checking for SA+W,NOT ACK

//	{

//		uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

//		goto exit; //need to change to goto exit or somthing

//	}

//	

//		uart0_tx_string("\r\n Read SA w...\r\n");

//	

//	

//	// Sending memory addr

//	I2DAT = memory_addr ;

//	I2CONCLR = (1<<3); // clearing SI flag

//	while(SI==0);

//	if(I2STAT == 0x30)   //  Data/Memory_addr  has been transmitted; NOT ACK has been received

//	{

//		uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

//		goto exit; //need to change to goto exit or somthing

//	}

//	

//		uart0_tx_string("\r\n Read mr...\r\n");

//	// Sending Restart condition

//	I2CONCLR = (1<<3); // clearing SI flag

//	I2CONSET = (1<<5); // STA =1;

//	while(SI==0);

//		if(I2STAT != 0x10)   //  Reapeted Start condition has been transmitted

//	{

//		uart0_tx_string("\r\n ERROR : Restart condition \r\n");

//		goto exit; //need to change to goto exit or somthing

//	}

//	I2CONCLR = (1<<5); // STA =0;

//	

//		uart0_tx_string("\r\n Read restart...\r\n");

//	uart0_intiger(I2DAT);

//	uart0_tx_string("\r\n Read restart...\r\n");

//	// Sending SA+R

//	I2DAT = slave_addr^1  ; //SA+R

//	I2CONCLR = (1<<3); // clearing SI flag

//	while(SI==0);

//	if(I2STAT == 0x48) // checking for SA+W,NOT ACK

//	{

//		uart0_tx_string("\r\n ERROR : SA + W failed , NOT ACK \r\n");

//		goto exit ; //need to change to goto exit or somthing

//	}

//	uart0_tx_string("\r\n Read SA+r...\r\n");

//	

//  // Geting data 

//	I2CONCLR = (1<<3); // clearing SI flag

//	while(SI==0);

//	result = I2DAT;

//	uart0_tx_string("\r\n Read done...\r\n");

//	I2CONSET = (1<<4);

//	I2CONCLR = (1<<3);

//	delay_ms(100);

//	goto exitresult;

//	

//}
