#ifndef    __TV_MOPHONG_H__
#define    __TV_MOPHONG_H__
#include    <18f6722.h>
#device     adc =10
#fuses      hs,PUT, NOWDT
#use        delay(clock=20M)
#USE        SPI(SPI2,STREAM = SPI_2,baud=5000000)
#include    <math.h>
#bit     TIM3IF           = 0xFA1.1
#define  analog_pins        an0_to_an3
#define  lm35a_channel      0
#define  lm35b_channel      1
#define  gp2d12_channel     2
#define  photoresis_channel 3
#define  TOUCH_PIN        pin_c6
#define  DHT11_PIN        pin_c5
#define  HX711_DO         pin_d2
#define  HX711_CLK        pin_d3
#define  DCMOTOR_IN1      PIN_G0
#define  STEPMOTOR_IN0    PIN_G1
#define  STEPMOTOR_IN1    PIN_G2
#define  STEPMOTOR_IN2    PIN_G3
#define  STEPMOTOR_IN3    PIN_G4
#define  soft_i2c_sda     pin_c4
#define  soft_i2c_scl     pin_c3 
#define  SRF04_TRIGER     PIN_C7
#define  LCD_RS           PIN_F0
#define  LCD_E            PIN_F5
#define  OUTPUT_LCD       OUTPUT_E
#define  LED32_RCK        pin_d5
#define  D7SEG_RCK        pin_d7
#define  S7SEG_RCK        pin_a5
#define  TRIAC_PIN        Pin_f6
#define  BUZZER_PIN       pin_f7
#define  GLCD_CSA         PIN_F1   
#define  GLCD_CSB         PIN_F2   
#define  GLCD_CSC         PIN_F3                       
#define  GLCD_RS          PIN_F0
#define  GLCD_E           PIN_F4
#define  output_glcd      output_e

//***** SRF04 using CCP1 (RC2) pin as ECHO pin *******************
#DEFINE  BT0             PIN_b4
#DEFINE  BT1             PIN_b5
#DEFINE  BT2             PIN_b6
#DEFINE  BT3             PIN_b7
#DEFINE  ON              BT0 
#DEFINE  UP              BT0  
#DEFINE  ON1             BT0 
#DEFINE  OFF             BT1
#DEFINE  DW              BT1
#DEFINE  OFF1            BT1
#DEFINE  INV             BT2
#DEFINE  ON2             BT2
#DEFINE  CLR             BT2
#DEFINE  MOD             BT3
#DEFINE  STOP            BT3    
#DEFINE  OFF2            BT3
typedef enum 
{
      OK    = 0,
      ERROR = 1,
      BUSY  = 2
}result;
typedef struct
{
      unsigned int32 timeline;
}DELAY_STRUCT;
DELAY_STRUCT  delay=0;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 32 LED don xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef union 
{ 
  int1           ledx1[32];
  unsigned int8  ledx8[4];
  unsigned int16 ledx16[2];
  unsigned int32 ledx32; 
} LED32_UNION;
LED32_UNION led32;
result led32_display(void);
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 7 DOAN TT xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
const unsigned int8 m7d[16]= {0XC0,0XF9,0xA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0x88, 0x83, 0xC6, 0xA1, 0x86, 0x7f};
typedef struct 
{ 
  unsigned int8   led[8];  
} D7SEG_STRUCT; 
D7SEG_STRUCT    d7seg ={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; 
result d7seg_display();
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 7 DOAN QUET xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct 
{ 
  unsigned int8   led[8];  
} S7SEG_STRUCT; 
S7SEG_STRUCT    s7seg ={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; 
result s7seg_display();
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx TRIAC-BUZZER xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result buzzer_on(void);
result buzzer_off(void);
result triac_2_on(void);
result triac_2_off(void);
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx BASE FUNCTIONS xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result system_init(void);
result CHECK_MODULE();
#endif
