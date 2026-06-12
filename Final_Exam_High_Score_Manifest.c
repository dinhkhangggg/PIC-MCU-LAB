#include <18F6722.h>
#device adc=10
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_CLOCK           PIN_D6
#define EXP_OUT_DO              PIN_D4
#define G_ALL                   PIN_C5

#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6
#define NUMBER_OF_74595_MA7DOAN 8

#define EXP_OUT_ENABLE_buzzer  PIN_F4
#define NUMBER_OF_74595_buzzer 2

#define NUMBER_OF_74595_32led 4
#define EXP_OUT_ENABLE_32led  PIN_b3

#define btn1 PIN_F1

#include <74595_MA7DOAN.c>
#include <74595_buzzer.c>
#include <74595_32led.c>

const int8 ma7doan[10]={0x3F,0x06,0x5B,0x4F,0x66,
                        0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 varCode[8];
unsigned int8 varcode1[2];
unsigned int8 varcode2[4];
unsigned int32 adc_val=0;
unsigned int8 tempC=0;
int1 mode=0;

void main() 
{
   output_low(G_ALL);
   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_16,249,1);
     output_low(G_ALL);
   varcode1[0]=varcode1[1]=0x00;
   varcode2[0]=varcode2[1]=varcode2[2]=varcode2[3]=0x00;

   while(TRUE) {
      set_adc_channel(0);
      delay_us(20);
      adc_val = read_adc();
      delay_ms(5);
      tempC = (adc_val * 500) / 1023;

      if(tempC <= 25) 
         set_pwm1_duty(1000);
      else if(tempC <= 30) 
         set_pwm1_duty(800);
      else if(tempC <= 35) 
         set_pwm1_duty(400);
      else set_pwm1_duty(100);

      if(!input(btn1)) 
      {
         delay_ms(50);
         mode = !mode;
         while(!input(btn1));
      }

      if(mode == 0) 
      {
         varCode[2] = ma7doan[tempC/10];
         varCode[1] = ma7doan[tempC%10];
      } 
      else
      {
         // Hi?n th? t?c ?? (1–9 theo nhi?t ??)
         unsigned int8 speed;
         if(tempC < 20) 
               speed = 0;
         else if(tempC > 40) 
            speed = 9;
         else 
            speed = (tempC - 20) / 2; // t? 0–10 ? 0–9 m?c

         varCode[2] = ma7doan[speed/10];
         varCode[1] = ma7doan[speed%10];
      }

      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_buzzer(varcode1);
      write_expanded_outputs_32led(varcode2);
      
      delay_ms(300);
   }
}
