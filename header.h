#include <LPC21xx.H>

#include <stdio.h>

#include <string.h>


#include <stdint.h>

#include <stdbool.h>

#include <math.h>


typedef unsigned int u32;

typedef  int s32;

typedef unsigned char u8;

typedef char s8;


#define MY_BUFFER_SIZE 100

#define WARNING_LED    (1<<17)

#define BUZZER         (1<<20)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////// WARNING ////////

void warning (void);




// **** algorithum.h *************


#ifndef ALGORITHM_BY_RF_H_

#define ALGORITHM_BY_RF_H_


/*

 * Settable parameters 

 * Leave these alone if your circuit and hardware setup match the defaults 

 * described in this code's Instructable. Typically, different sampling rate

 * and/or sample length would require these paramteres to be adjusted.

 */

#define ST 1 //4      // Sampling time in s. WARNING: if you change ST, then you MUST recalcuate the sum_X2 parameter below!

#define FS 100 //25     // Sampling frequency in Hz. WARNING: if you change FS, then you MUST recalcuate the sum_X2 parameter below!

// Sum of squares of ST*FS numbers from -mean_X (see below) to +mean_X incremented be one. For example, given ST=4 and FS=25,

// the sum consists of 100 terms: (-49.5)^2 + (-48.5)^2 + (-47.5)^2 + ... + (47.5)^2 + (48.5)^2 + (49.5)^2

// The sum is symmetrc, so you can evaluate it by multiplying its positive half by 2. It is precalcuated here for enhanced 

// performance.


//const float sum_X2 = 83325; // WARNING: you MUST recalculate this sum if you changed either ST or FS above!

//// WARNING: The two parameters below are CRUCIAL! Proper HR evaluation depends on these.

//#define MAX_HR 180  // Maximal heart rate. To eliminate erroneous signals, calculated HR should never be greater than this number.

//#define MIN_HR 40   // Minimal heart rate. To eliminate erroneous signals, calculated HR should never be lower than this number.

//// Minimal ratio of two autocorrelation sequence elements: one at a considered lag to the one at lag 0.

//// Good quality signals must have such ratio greater than this minimum.

//const float min_autocorrelation_ratio = 0.5;

//// Pearson correlation between red and IR signals.

//// Good quality signals must have their correlation coefficient greater than this minimum.

//const float min_pearson_correlation = 0.8;


////Gemini ***


#define sum_X2 83325.0f


#define MAX_HR 180

#define MIN_HR 40


#define min_autocorrelation_ratio 0.5f

#define min_pearson_correlation 0.8f


#define BUFFER_SIZE (FS*ST)

#define FS60 (FS*60)

#define LOWEST_PERIOD (FS60/MAX_HR)

#define HIGHEST_PERIOD (FS60/MIN_HR)

#define mean_X ((float)(BUFFER_SIZE-1)/2.0f)


/////////////////////////////////////////////////////////////////////////


/*

 * Derived parameters 

 * Do not touch these! 

 * 

 */

//const s32 BUFFER_SIZE = FS*ST; // Number of smaples in a single batch

//const s32 FS60 = FS*60;  // Conversion factor for heart rate from bps to bpm

//const s32 LOWEST_PERIOD = FS60/MAX_HR; // Minimal distance between peaks

//const s32 HIGHEST_PERIOD = FS60/MIN_HR; // Maximal distance between peaks

//const float mean_X = (float)(BUFFER_SIZE-1)/2.0; // Mean value of the set of integers from 0 to BUFFER_SIZE-1. For ST=4 and FS=25 it's equal to 49.5.


void rf_heart_rate_and_oxygen_saturation(u32 *pun_ir_buffer, s32 n_ir_buffer_length, u32 *pun_red_buffer, float *pn_spo2, s8 *pch_spo2_valid, s32 *pn_heart_rate, 

                                        s8 *pch_hr_valid, float *ratio, float *correl);

float rf_linear_regression_beta(float *pn_x, float xmean, float sum_x2);

float rf_autocorrelation(float *pn_x, s32 n_size, s32 n_lag);

float rf_rms(float *pn_x, s32 n_size, float *sumsq);

float rf_Pcorrelation(float *pn_x, float *pn_y, s32 n_size);

void rf_initialize_periodicity_search(float *pn_x, s32 n_size, s32 *p_last_periodicity, s32 n_max_distance, float min_aut_ratio, float aut_lag0);

void rf_signal_periodicity(float *pn_x, s32 n_size, s32 *p_last_periodicity, s32 n_min_distance, s32 n_max_distance, float min_aut_ratio, float aut_lag0, float *ratio);


#endif /* ALGORITHM_BY_RF_H_ */




////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////**** ADC ****

float lm35_read(void);

unsigned int adc_read(int );

void adc_init(void);





// **** Wifi Driver ****


void uart1_intr_init(void);

u8 wifi_start(void);

u8 send_to_Thinkspeak(s32 , float  , s32 );





// **** I2C ****

void i2c_init(void);

void i2c_write(u32 , u32  , u8 );

u8 i2c_read(u32 , u32 );

u8 read_FIFO_WR_PTR(void);

u8 read_OVR_COUNTER(void);

u8 read_FIFO_RD_PTR(void);

void MAX_init(void);

void i2c_burst_read(u32 , u32 );

 u8 temp(void);


u8 sp02_sample(void);

u8 sp02_sample_temp(void);

u32 average(u32 * );





// lcd & delay

extern void delay_sec(unsigned int );

extern void delay_ms(unsigned int );

extern void lcd_data( unsigned char );

extern void lcd_cmd( unsigned char );

extern void lcd_init(void);

extern void lcd_str(s8 *);

extern int str_len(char *);

extern void lcd_integer(int );

void lcd_float(float );


// ****** UART0 ********

extern void uart0_tx(unsigned char );

extern void uart0_init(unsigned int );

extern void uart0_tx_string(char *);

extern unsigned char  uart0_rx(void);

extern void uart0_rx_string(unsigned char *);

void uart0_adc_12_Bit(u32 );

void uart0_float(float );

void uart0_intiger(s32 );


void uart0_rx_buffer_ESPwifi(u8 *);


int cal(int *,int*,unsigned char*,unsigned char*);

void uart0_int_char(int );

void uart0_hex(u8 );


// ****** UART1 ********

extern void uart1_tx(unsigned char );

extern void uart1_init(unsigned int );

extern void uart1_tx_string(char *);

extern unsigned char  uart1_rx(void);

extern void uart1_rx_string(unsigned char *);

void uart1_adc_12_Bit(u32 );

void uart1_float(float );

void uart1_intiger(s32 );


void uart1_rx_buffer_ESPwifi(u8 *);


int cal(int *,int*,unsigned char*,unsigned char*);

void uart1_int_char(int );

void uart1_hex(u8 );


// Internal ADC 

u32 adc(u8 );

void adc_init(void);




//External intterupt

void EXT_init(void);

//void EXT_isr(void);

void ultrasonic_init(void);
unsigned int distance(void);
void DHT11_init(void);
unsigned char DHT11_ReadTemperature(void);
void uart0_tx_string(char *p);

#define RED (1<<17)
#define GREEN (1<<19)
#define DIST_THRESHOLD 15

