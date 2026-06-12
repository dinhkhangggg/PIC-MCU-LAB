#ifndef    __TV_D501_TH_H__
#define    __TV_D501_TH_H__
#include     <18f4550.h>
#device      adc=10
#fuses       nowdt,put,hs,noprotect,nolvp,cpudiv1
#use         delay(clock=20000000)
#use         rs232(baud=9600, xmit=pin_c6,rcv=pin_c7)
#include     <math.h>
#bit         TIM3IF                 = 0xFA1.1
#bit         SSDO                   = 0xF84.2
#bit         SSCK                   = 0xF84.0
#define      analog_pins            an0_to_an2
#define      lm35a_channel          0
#define      lm35b_channel          1
#define      gp2d12_channel         2
#define      photoresis_channel     3
#define      DCMOTOR_IN1            PIN_c1
#define      TOUCH_PIN              pin_a5
#define      DHT11_PIN              pin_d1
#define      HX711_DO               0       // khong co module nay , khai bao cho khoi bao loi
#define      HX711_CLK              0       // khong co module nay , khai bao cho khoi bao loi
#define      SRF04_TRIGER           PIN_E2
#define      soft_i2c_sda           pin_b0
#define      soft_i2c_scl           pin_b1
#define      bt0                    pin_b5
#define      bt1                    pin_b4
#define      bt2                    pin_b3
#define      bt3                    pin_b2
#define      on                     bt0      
#define      off                    bt1
#define      inv                    bt2
#define      up                     bt0    
#define      dw                     bt1
#define      clr                    bt2
#define      mod                    bt3
#define      stop                   bt3
#define      on1                    bt0     
#define      off1                   bt1
#define      on2                    bt2       
#define      off2                   bt3
#define      S7SEG_RCK              pin_d5
#define      LED32_RCK              pin_d4
#define      D7SEG_RCK              pin_d6
#define      ALLMODULE_G            pin_d0         //a - 4 byte
#define      LCD_RCK                pin_d2         //a - 2 byte dao
#define      RBDC_RCK               pin_d3         //b - 1 byte 
#define      MATRIX_RCK             pin_d7         //b - 3 byte dao
typedef enum 
{
      OK    = 0,
      ERROR = 1,
      BUSY  = 2
}result;
typedef struct 
{
      int1      STEP_MOTOR_ENABLE;
      int1      STEP_MOTOR_IN1;
      int1      STEP_MOTOR_IN2;
      int1      STEP_MOTOR_IN3;
      int1      STEP_MOTOR_IN4;
      int1      DC_MOTOR_ENABLE;
      int1      PWRKEY;
      int1      NC_PIN1;          
      int1      BUZZER;
      int1      TRIAC_1;
      int1      TRIAC_2;
      int1      RELAY_1;
      int1      RELAY_2 ;
      int1      NC_PIN2 ;  
      int1      NC_PIN3;   
      int1      NC_PIN4;  
}RBDC_STRUCT;
typedef union
{
      RBDC_STRUCT         bit;
      unsigned int8       xbyte[2];
}RBDC_UNION;

typedef struct 
{
    RBDC_UNION           rbdc;
    unsigned int8        lcddata; 
    unsigned int8        lcdcontrol;
    unsigned int8        glcddata;
    unsigned int8        glcdcontrol;    
}SYSTEM_VAR_STRUCT;
SYSTEM_VAR_STRUCT system_var={0,0,0,0,0};
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
  unsigned int8   led[4];  
} D7SEG_STRUCT; 
D7SEG_STRUCT    d7seg ={0xff,0xff,0xff,0xff}; 
result d7seg_display();
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 7 DOAN QUET xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct 
{ 
  unsigned int8   led[8];  
} S7SEG_STRUCT;
S7SEG_STRUCT s7seg={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
result s7seg_display();
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx TRIAC-BUZZER xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
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
void SSPI_WRITE(unsigned int8 x);
#endif
