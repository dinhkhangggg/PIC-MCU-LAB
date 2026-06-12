#include <16f887.h>
#device adc=10
#fuses intrc_io
#use   delay(clock=8M)

#define LCD_ENABLE_PIN  PIN_d0       
#define LCD_RS_PIN      PIN_d1 
#define LCD_RW_PIN      PIN_d2  
#define LCD_DATA4       PIN_D4  
#define LCD_DATA5       PIN_D5  
#define LCD_DATA6       PIN_D6   
#define LCD_DATA7       PIN_D7  
#include <lcd.c>
unsigned int8 temp , box ;
unsigned int1 TT;
unsigned int8 N,S ;
const unsigned maled[10] ={0xc0, 0xf9, 0xa4,0xb0,0x99,0x92,0x82, 0xf8, 0x80, 0x90};

void main() {
   set_tris_A(0x00);
   set_tris_B(0x00);
   set_tris_D(0x00);
   set_tris_e(0xff);
   set_tris_c(0xff);
   lcd_init();
   
   setup_adc(adc_clock_internal);
   setup_adc_ports(san5|san6|vss_vdd);
   
   setup_timer_1(T1_external|t1_div_by_1);
   set_timer1(0);
   
   delay_ms(2);
   while(true) 
   {
 
      set_adc_channel(5);
      delay_ms(50);
      temp=read_adc()/2.046;
      
      set_adc_channel(6);
      delay_ms(50);
      N= read_adc()/10.23;
      
      if(N>=50) 
            N=50;
         else if (N<=10) 
            N=10;
         else;
         
        IF (TEMP>50)
         { 
         TT = 1;
         LCD_SEND_BYTE(0,0X01);
         setup_timer_1(t1_disabled);
         }
         
      
      ELSE IF (TEMP <= 50);
      {
         TT=0;
         LCD_SEND_BYTE(0,0X01);
         setup_timer_1(t1_external|t1_div_by_1);
      }
      
      ELSE;
      
      
      if(TT==1)
      {
     LCD_SEND_BYTE(0,0X01);
      }
      
      else IF (TT == 0)
      {
        S=get_timer1();
         if(S>=N)
         {
            box++;
            S=1;
            set_timer1(1);
            
         }
         
         else;
      
      }
      
      ELSE;
      
      lcd_gotoxy(1,1);
      printf(lcd_putc,"SP=%02U",S);
      lcd_gotoxy(13,1);
      printf(lcd_putc,"N=%02U",N);
      lcd_gotoxy(1,2);
      printf(lcd_putc,"BOX=%04u %02u",box, temp);
      output_B(maled[N%10]);
      output_a(maled[N/10]);
      
   }
   
}


