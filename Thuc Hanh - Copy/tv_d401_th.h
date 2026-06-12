#ifndef    __TV_D401_TH_H__
#define    __TV_D401_TH_H__
#include    <18f6722.h>
#device     adc=10
#fuses      nowdt,put,noprotect,hs
#use        delay(clock=20M)
#USE        SPI(SPI2,STREAM = SPI_2,baud=5000000)
#include    <math.h>
#define      bt0              pin_b6
#define      bt1              pin_f1
#define      bt2              pin_b4
#define      bt3              pin_b7
#DEFINE      ON               BT0 
#DEFINE      UP               BT0  
#DEFINE      ON1              BT0 
#DEFINE      OFF              BT1
#DEFINE      DW               BT1
#DEFINE      OFF1             BT1
#DEFINE      INV              BT2
#DEFINE      ON2              BT2
#DEFINE      CLR              BT2
#DEFINE      MOD              BT3
#DEFINE      STOP             BT3    
#DEFINE      OFF2             BT3
#bit         TIM3IF           = 0xFA1.1
#define      analog_pins        an0_to_an4
#define      lm35a_channel      0
#define      lm35b_channel      4
#define      gp2d12_channel     3
#define      photoresis_channel 1
#define      DCMOTOR_IN1      PIN_c1
#define      KEYPAD_SCL       pin_f3
#define      KEYPAD_SDA       pin_f2
#define      DHT11_PIN        pin_g3
#define      TOUCH_PIN        PIN_G0
#define      SOFT_I2C_SDA     pin_c4
#define      SOFT_I2C_SCL     pin_c3
#define      HX711_DO         pin_d0
#define      HX711_CLK        pin_d1
#define      SRF04_TRIGER     pin_e7
#define      SRF04_ECHO       pin_e6
#define      ALLMODULE_G      pin_c5   //pin_e1 
#define      LED32_RCK        pin_b3   //pin_d0
#define      D7SEG_RCK        pin_f6
#define      MATRIX_RCK       pin_f5
#define      LCD_RCK          pin_g4
#define      GLCD_RCK         pin_f7
#define      RBDC_RCK         pin_f4
typedef enum 
{
      OK    = 0,
      ERROR = 1,
      BUSY  = 2
}result;
typedef struct 
{
      int1 step_motor_enable ;
      int1 ms1 ;
      int1 ms2 ;
      int1 ms3 ;
      int1 step;
      int1 dir ;
      int1 servo1 ;
      int1 step_motor_in1;
      int1 step_motor_in2;
      int1 step_motor_in3;
      int1 step_motor_in4 ;
      int1 triac_1;
      int1 triac_2 ;
      int1 buzzer;
      int1 relay_1 ;
      int1 relay_2;
}RBDC_STRUCT;
typedef union
{
      RBDC_STRUCT            bit;
      unsigned int8          xbyte[2];
}RBDC_UNION;
typedef struct 
{
    RBDC_UNION            rbdc;
    unsigned int8         lcddata; 
    unsigned int8         lcdcontrol;
    unsigned int8         glcddata;
    unsigned int8         glcdcontrol; 
}SYSTEM_VAR_STRUCT;
SYSTEM_VAR_STRUCT system_var={0,0,0,0,0,0};
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
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx TRIAC-BUZZER- DC xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result buzzer_on();
result buzzer_off();
result relay_1_on();
result relay_1_off();
result relay_2_on();
result relay_2_off();
result triac_1_on();
result triac_1_off();
result triac_2_on();
result triac_2_off();
result DCMOTOR_ENABLE();
result STEPMOTOR_ENABLE();
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx BASE FUNCTIONS xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result system_init(void);
#endif
