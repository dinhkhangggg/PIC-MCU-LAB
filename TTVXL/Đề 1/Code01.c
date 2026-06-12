#include <18F4550.h>
#fuses HS
#use delay(clock=8MHz)
#define EXP_OUT_ENABLE_8L7D  PIN_D5
#define EXP_OUT_ENABLE7D  PIN_D6
#define EXP_OUT_ENABLE_RLB  PIN_D3
#define EXP_OUT_ENABLE_LCD  PIN_D2  
#define EXP_OUT_ENABLE_MTL  PIN_D7 
#define EXP_OUT_ENABLE_32  PIN_D4   //RCK_32LED
#define EXP_OUT_CLOCK   PIN_E0   //SSCK
#define EXP_OUT_DO      PIN_E2   //SSDO

#define G_ALL           PIN_D0   //G_ALL

#define BT0               PIN_B5 //run
#define BT1               PIN_B4 //stop
#define BT2               PIN_B3 //pause



#define NUMBER_OF_74595_MTL 6
#define NUMBER_OF_74595_8L7D 2
#define NUMBER_OF_74595_7D 4
#define NUMBER_OF_74595_32 4
#define NUMBER_OF_74595_RLB 1
#define NUMBER_OF_74595_LCD 2
#include <74595_32leddon.c>
#include <74595_RLB.c>
#include <74595_8L7D.c>
#include <74595.c>
#include <74595_LCD.c>
#include <74595_MTL.c>

const unsigned int8 MA7DOAN[17]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0X88,0X83,0XC6,0XA1,0X86,0X8E,0XFF};
const char aryCode7Seg[17]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
//                           'A'  'b'  'C'  'd'  'E'  'F'
                            0x88,0x83,0xC6,0xA1,0x86,0x8E,0xff};

unsigned int8 MTL0,MTL1,MTL2,MTL3,MTL4,MTL5;
unsigned int8 LCD0,LCD1;                           
unsigned int8 varCount;
unsigned int16 varCode,varCode1,varCode2;                          
unsigned int8 X8LED0,X8LED1,X8LED2,X8LED3;
UNSIGNED INT8 LED0,LED1,LED2,LED3;
unsigned int8 DATA_RLB;                         

void TAT_32_LED()
{
  X8LED0=0x0;
  X8LED1=0x0;
  X8LED2=0x0;
  X8LED3=0X0;
  
  write_expanded_outputs_32LED(&x8LED0);  
}
void TAT_RTL()
{
    DATA_RLB=0x0;
    write_expanded_outputs_RLB(&DATA_RLB); 
    DATA_RLB=0x0;
    write_expanded_outputs_RLB(&DATA_RLB);  
}
void  TAT_4_LED_SEG()
{
   LED0=0xff;
   LED1=0xff;
   LED2=0xff;
   LED3=0xff;
   write_expanded_outputs_8L7D(&LED0);
}
void TAT_8LED_QUET()
{
varCount=2;
varCode1=(~aryCode7Seg[varCount])&0x00FF;
varCode2=~0b0000000001111111;
varCode=varCode2<<8|varCode1;
write_expanded_outputs_8L7D(&varCode);
varCount=3;
varCode1=(~aryCode7Seg[varCount])&0x00FF;  
varCode2=~0b0000000010111111;
varCode=varCode2<<8|varCode1;
write_expanded_outputs_8L7D(&varCode);
varCount=16;
varCode1=(~aryCode7Seg[varCount])&0x00FF;
varCode2=~0b0000000011011111;
varCode=varCode2<<8|varCode1;
write_expanded_outputs_8L7D(&varCode);
varCount=16;
varCode1=(~aryCode7Seg[varCount])&0x00FF;  
varCode2=~0b0000000011101111;
varCode=varCode2<<8|varCode1;
write_expanded_outputs_8L7D(&varCode);
varCount=16;
varCode1=(~aryCode7Seg[varCount])&0x00FF;
varCode2=~0b0000000011110111;
varCode=varCode2<<8|varCode1;
write_expanded_outputs_8L7D(&varCode);
varCount=16;
varCode1=(~aryCode7Seg[varCount])&0x00FF;  
varCode2=~0b0000000011111011;
varCode=varCode2<<8|varCode1;
write_expanded_outputs_8L7D(&varCode);
varCount=16;
varCode1=(~aryCode7Seg[varCount])&0x00FF;
varCode2=~0b00000000111111101;
varCode=varCode2<<8|varCode1;
write_expanded_outputs_8L7D(&varCode);
varCount=16;
varCode1=(~aryCode7Seg[varCount])&0x00FF;  
varCode2=~0b00000000111111110;
varCode=varCode2<<8|varCode1;
write_expanded_outputs_8L7D(&varCode);
}
void TAT_LCD()
{
   LCD0=0x0;
   LCD1=0x0;
   write_expanded_outputs_LCD(&LCD0);
   
}
void TAT_MTL()
{
  MTL0=0x0;
  MTL1=0x0;
  MTL2=0x0;
  MTL3=0x0;
  MTL4=0x0;
  MTL5=0x0;
  write_expanded_outputs_MTL(&MTL0);
}
void TAT_ALL()
{
  TAT_32_LED();
  TAT_RTL();
  TAT_4_LED_SEG();
  TAT_8LED_QUET();
  TAT_LCD();
  TAT_MTL();
}

unsigned int8 j,dem;
unsigned int32 k,save;
unsigned int8 run;
unsigned int1 CHAY,STOP,PAUSE;





#int_timer1
VOID KTRA_NUTNHAN()
{
 IF(INPUT(BT0)==0)// RUN
    {
      DELAY_MS(10);
      IF(INPUT(BT0)==0)
       {
        WHILE(INPUT(BT0)==0){}
        STOP=0;
        CHAY=1;
        PAUSE=0;
        }
        }
    IF(INPUT(BT1)==0) //STOP
    {
      DELAY_MS(10);
      IF(INPUT(BT1)==0)
       {
        WHILE(INPUT(BT1)==0){} 
        STOP=1;
        CHAY=0;
        PAUSE=0;
        }
    }
    IF(INPUT(BT2)==0) //PASUE
    {
      DELAY_MS(10);
      IF(INPUT(BT2)==0)
       {
        WHILE(INPUT(BT2)==0){} 
        STOP=0;
        CHAY=~CHAY;
        PAUSE=~PAUSE;
  
       }
    }
    set_timer1(-5000);
}

void hienthi_Led_seg(int8 dem)
{
   LED0=0xff;
   LED1=0xff;
   LED2=0xff;
   LED3=MA7DOAN[dem];
   write_expanded_outputs_8L7D(&LED0);
}


void main()
{
 save=0;
 output_low(G_ALL);
 TAT_ALL();
  /////////////////////////////////////
  X8LED0 = read_eeprom(0x44);
  dem = read_eeprom(0x40);
    CHAY = read_eeprom(0x10);
      STOP = read_eeprom(0x30);
      PAUSE = read_eeprom(0x20);
    
  enable_interrupts(global);
 enable_interrupts(int_timer1);
 setup_timer_1(t1_internal|t1_div_by_8);
 set_timer1(-5000);
 //////////////////////////////////////////
   while(true)
   {
   lap:
   if(STOP==1)
   {
       k=0;
       dem=0;
       x8LED0=k; 
       write_expanded_outputs_32LED(&x8LED0);
       hienthi_Led_seg(dem);
       if(CHAY==1) goto lap;
       run=0x00000001;
        
   }
   
   if(CHAY==1 || PAUSE==1)
   {
     for(j=0;j<8;j++)
     {
      if(CHAY==0) goto lap;
       dem=dem+1;
       if(dem==9) dem=1;
       k=save|run;
    
       x8LED0=k;
      
       write_expanded_outputs_32LED(&x8LED0);
       
        
       run=(run<<1);
       if(run==0) run=0x1;
       hienthi_Led_seg(dem);
        
         
        if(CHAY==0) goto lap;
      delay_ms(1000);
       if(CHAY==0) goto lap;
       
       
       
        write_eeprom(0x44,X8LED0);
    write_eeprom(0x40,dem);
    write_eeprom(0x10,CHAY);
      write_eeprom(0x30,STOP);
      write_eeprom(0x20,PAUSE);

       }
     } 

   }
    write_eeprom(0x44,X8LED0);
    write_eeprom(0x40,dem);
    write_eeprom(0x10,CHAY);
      write_eeprom(0x30,STOP);
      write_eeprom(0x20,PAUSE);
}

