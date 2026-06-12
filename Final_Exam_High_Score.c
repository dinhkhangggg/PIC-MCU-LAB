#include <18F6722.h>
#device adc=10
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_CLOCK           PIN_D6
#define EXP_OUT_DO              PIN_D4
#define G_ALL                   PIN_C5
#define EXP_OUT_ENABLE_32LED    PIN_B3  
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6 
#define EXP_OUT_ENABLE_BUZZER   PIN_F4

#define NUMBER_OF_74595_32_LEDs   4
#define NUMBER_OF_74595_MA7DOAN 8
#define NUMBER_OF_74595_Buzzer  2

#define btn1 PIN_F1
#define btn3 PIN_B7

#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>
#include <74595_Buzzer.c>

const int8 ma7doan[10]={0x3F,0x06,0x5B,0x4F,0x66,
                        0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 led32[4];
unsigned int8 varCode[8];
unsigned int8 buzzer[2];
unsigned int32 lm35a=0;
unsigned int8 hta=0;
int1 mode=0;
unsigned int16 cd=100;
int1 blink=0;
int1 special_mode=0;


void main()
{
   output_low(G_ALL);
   buzzer[0]=0x00; buzzer[1]=0x00;
   led32[0]=led32[1]=led32[2]=led32[3]=0x00;
   write_expanded_outputs_BUZZER(buzzer);
   
   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_16,249,1);

   while(TRUE)
   {
      set_adc_channel(0);
      delay_us(20);
      lm35a = read_adc();
      delay_ms(5);
      hta = (lm35a * 500) / 1023;

      if(!input(btn1) && !input(btn3))
         special_mode = 1;
      else
         special_mode = 0;

      if(special_mode)
      {
         set_pwm1_duty(100);
         varCode[4] = 2+4+8+16+32;
         varCode[3] = ma7doan[hta/10];
         varCode[2] = ma7doan[hta%10];
         write_expanded_outputs_MA7DOAN(varCode);
         delay_ms(300);
      }
      else
      {
         if(!input(btn1)) 
         {
            delay_ms(20);
            mode = !mode;
            while(!input(btn1));
         }

         if(!input(btn3))
         {
            delay_ms(20);
            if(mode==1) 
            {
               if(cd==100) cd=300;
               else if(cd==300) cd=600;
               else if(cd==600) cd=900;
               else cd=100;
            }
            while(!input(btn3));
         }

         if(mode==0) 
         {
            if(hta <= 30)       set_pwm1_duty(900);
            else if(hta <= 35)  set_pwm1_duty(500);
            else                set_pwm1_duty(100);
         } 
         else 
         {
            set_pwm1_duty(cd);
         }

         varCode[4] = ma7doan[mode];

         if(hta > 35) 
         {
            if(blink == 0) 
            {
               varCode[3] = ma7doan[hta/10];
               varCode[2] = ma7doan[hta%10];
            } 
            else 
            {
               varCode[3] = 0x00;
               varCode[2] = 0x00;
            }
            blink = !blink;
         } 
         else 
         {
            varCode[3] = ma7doan[hta/10];
            varCode[2] = ma7doan[hta%10];
         }

         write_expanded_outputs_MA7DOAN(varCode);
         write_expanded_outputs_32LED(led32);
         delay_ms(300);
      }
   }
}

