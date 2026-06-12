#include <18f4550.h>
#fuses hs, nomclr
#use delay (clock=20M)

#include <74595_32leddon.c>
#incLude <74595_4led7doan.c>
#include <dk_rtb.c>

#define level pin_b4
#define FR  pin_b5
#define stop pin_b3 

signed int16 captocdo;
int1 en=0;
int1 forward =0;
unsigned int32 led1,led2,led3,led4,led;
const unsigned int8 mabaydoan[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};
void xulyled7doan();
void xulyled7doan()
{
   led1=0xff;
   led2=0xff;
   led3=0xff;
   led4=mabaydoan[captocdo];
   led=led1|led2<<8|led3<<16|led4<<24;
   xuat4led7doan(&led);
}

void main()
{
   output_low(G_ALL);
   control_rtb(dc_motor);
   setup_timer_2(T2_div_by_16,249,1);
   if(read_eeprom(1) != 0x55) 
   {
     
      CAPTOCDO = 0; 
      write_eeprom(0, CAPTOCDO); 
      write_eeprom(1, 0x55); 
   }
   else
   {
      CAPTOCDO = read_eeprom(0);
      if(CAPTOCDO > 9) CAPTOCDO = 0;
   }
   xulyled7doan();
   while(true)
   {
   // kiem tra nut FR /////////////////// pin_b5
      if(input(FR)==0)
      {
         delay_ms(5);
         if(input(FR)==0)
         {
         forward=~forward;
         en=1;
         if(forward==1) 
            {
               setup_ccp1(ccp_pwm);
               set_pwm1_duty(captocdo*100);
               setup_ccp2(ccp_off);
               output_low(pin_c1);
            }
         else if(forward==0)
            {
               setup_ccp2(ccp_pwm);
               set_pwm2_duty(captocdo*100);
               setup_ccp1(ccp_off);
               output_low(pin_c2);
            }
         while(input(FR)==0) {}
         }
      }
      // kiem tra nut level /////////////////// pin_b4
      if(input(level)==0)
      {
         delay_ms(5);
         if(input(level)==0)
         {
         if(forward==1)
         {
         if(en==1)
         captocdo++; if(captocdo>9) captocdo=0;
         write_eeprom(0x00,captocdo);
         set_pwm1_duty(captocdo*100);
         xulyled7doan(); 
         }
         else if(forward==0)
         {
         if(en==1)
         captocdo++; if(captocdo>9) captocdo=0;
         write_eeprom(0x00,captocdo);
         set_pwm2_duty(captocdo*100);
         xulyled7doan();
         }
         while(input(level)==0) {}
         }
      }
      //kiem tra nut stop pin-b3
      if(input(stop)==0)
      {
         delay_ms(5);
         if(input(stop)==0)
         {
         en=0;
         setup_ccp1(ccp_off);
         setup_ccp1(ccp_off);
         output_low(pin_c1);
         output_low(pin_c2);
         while(input(stop)==0) {}
         }
      }
   }
}

