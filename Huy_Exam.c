#include <18F6722.h>
#device adc=10
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_CLOCK           PIN_D6      //MBI_SCK
#define EXP_OUT_DO              PIN_D4      //MBI_SDI
#define G_ALL                   PIN_C5      //MBI_OE
#define EXP_OUT_ENABLE_32LED    PIN_B3  
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6 
#define EXP_OUT_ENABLE_BUZZER   PIN_F4

#define NUMBER_OF_74595_32_LEDs   4
#define NUMBER_OF_74595_MA7DOAN 8
#define NUMBER_OF_74595_Buzzer  2
#define btn0 pin_b6
#define btn1 pin_f1

#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>
#include <74595_Buzzer.c>

const int8 ma7doan[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned int8 varCode[8];

unsigned int32 lm35a=0;
unsigned int8 hta=0;
unsigned int32 i=0;
unsigned int8 buzzer[2];
int1 mode=0;
unsigned int8 cd;

void delay_msx(unsigned int32 t)
{
   for (i=0; i<t; i++)
   {
      delay_ms(1);
   }
}

void main ()
{
   output_low(G_ALL);
   
   buzzer[0] = 0x00;
   buzzer[1] = 0x00;
   write_expanded_outputs_BUZZER(buzzer);
   
   setup_adc_ports(an0, vss_vdd);
   setup_adc(adc_clock_internal);
   setup_ccp1(CCP_PWM);

   while (true)
   {
      
      set_adc_channel(0);
      delay_us(20);
      lm35a = read_adc();
      delay_msx(1);
      hta = (lm35a*500)/1023;   
      setup_ccp1(CCP_PWM);

      if(!input(btn0))
      {
         mode = ~mode;
      }
      
      if (!input(btn1))
      {  
      delay_msx(20);
      if (cd>9)   cd=1;
      else cd++;
      while (!input(btn1));
      }
      
      if (mode == 0)
      {
      if (hta <= 30)
      {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(900);

         varCode[0] = 0x00;             
         varCode[1] = 0x00;         
         varCode[2] = ma7doan[hta%10];   
         varCode[3] = ma7doan[hta/10];   
         varCode[7] = 0x00;              
         varCode[6] = 0x00;              
         varCode[5] = 0x00;             
         varCode[4] = ma7doan[cd];              
         write_expanded_outputs_MA7DOAN(varCode);
      }
      else if (hta > 30 && hta <= 35)
      {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(500);

         varCode[7]=0x00;
         varCode[6]=0x00;
         varCode[5]=0x00;
         varCode[4]=ma7doan[mode+1];
         varCode[0]=0x00; 
         varCode[1]=0x00;
         varCode[2]=ma7doan[hta%10];
         varCode[3]=ma7doan[hta/10];
         write_expanded_outputs_MA7DOAN(varCode);
      }
      else
      {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(100);

         varCode[2]=ma7doan[hta%10];
         varCode[3]=ma7doan[hta/10];
         varCode[7]=0x00;   
         varCode[6]=0x00;   
         varCode[5]=0x00;   
         varCode[4]=ma7doan[cd]; 
         write_expanded_outputs_MA7DOAN(varCode);
         delay_msx(300);

         varCode[7]=0x00;
         varCode[6]=0x00;
         varCode[5]=0x00;
         varCode[2]=0x00;
         varCode[3]=0x00;
         write_expanded_outputs_MA7DOAN(varCode);
         delay_msx(300);
      }

      delay_msx(300);
   }
  
  else
  {
   if (cd==1)
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(100);
  }
   
   else if (cd==2)
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(200);
   }
   
      else if (cd==3)
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(300);
   }
   
   else if (cd==4)
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(400);
   }
   
   else if (cd==5)
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(500);
   } 
  
   else if (cd==6)
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(600);
   }  
   else if (cd==7)
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(700);
   }
   else if (cd==8)
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(800);
   }
   else if (cd==9)
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(900);
   }
   else 
   {
         setup_timer_2(T2_DIV_BY_16,249,1);
         set_pwm1_duty(0);
   }
   varCode[7] = 0x00;
   varCode[6] = 0x00;
   varCode[5] = 0x00;
   varCode[4] = ma7doan[mode+1];
   varCode[3] = 0x00;
   varCode[2] = 0x00;
   varCode[1] = 0x00;
   varCode[0] = 0x00;
   write_expanded_outputs_MA7DOAN(varCode);
   delay_msx(300);
  } 
}

