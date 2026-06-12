#include <18F6722.h>
#fuses HS, MCLR
#use delay(clock=40MHz)

#define EXP_OUT_ENABLE_BUZZER   PIN_F4      //MBI_LE0
#define EXP_OUT_CLOCK           PIN_D6      //MBI_SCK
#define EXP_OUT_DO              PIN_D4      //MBI_SDI
#define G_ALL                   PIN_C5      //MBI_OE
#define EXP_OUT_ENABLE_32LED    PIN_B3  
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6  
#define btn3                    PIN_B7      
#define btn2                    PIN_B4      
#define STOP                    PIN_F1      
#define RUN                     PIN_B6
//#define rst                     PIN_G5
#define NUMBER_OF_74595_Buzzer 2
#define NUMBER_OF_74595_32_LEDs 4
#define NUMBER_OF_74595_MA7DOAN 8
#include <74595_Buzzer.c>
#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

const int8 array[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 varCode[8];     
unsigned int8 varCode1[4];
unsigned int8 varCode0[2];
int8 x=0x01;
int32 i;
int1 s=1;

void delay_msx(int16 i)
{
   for (int16 t=0;t<=i;t++)
   {
      
      if (!input(stop))
      {
         s=1;
         break;
         delay_ms(1);
      }
      if (!input(run))
      {
         s=0;
         break;
         delay_ms(1);
      }
      delay_ms(1);
   }
}

void main() 
{   
     varCode0[1]=0b00000000;
     varCode0[0]=0b00000000;
    output_low(G_ALL);
     
    while(TRUE)
    {
        if(!input(RUN))
        {
           delay_ms(20); 
           s=0;
           varCode[7]=0x77;
           varCode[6]=0x1C;
           varCode[5]=0x54;
           varCode1[0]=x;
           write_expanded_outputs_MA7DOAN(varCode); 
           delay_ms(1000);
           while(!input(btn3));
        }
        if (!input(STOP))
        {
            delay_ms(20);
            s=1;
            varCode[7]=0x6D;
            varcode[6]=0x78;
            varcode[5]=0x5c;
            varCode[4]=0x73;
            //varCode1[0]=x;
            write_expanded_outputs_MA7DOAN(varCode);
            
            while(!input(STOP));
            delay_ms(1000);
        }
        
       if (s==0)
       {
         for (i=0;i<300;i++)
         {
            varCode[2] = array[(i/100) % 10];    
            varCode[1] = array[(i/10) % 10];      
            varCode[0] = array[i % 10]; 
            
            if (i<100) 
            {  
               varCode[2]=0x00;
               if (i<10) 
               {
                  varCode[1]=0x00;}
            }

           write_expanded_outputs_MA7DOAN(varCode);
           write_expanded_outputs_BUZZER(varCode0); 
           delay_msx(100);
                           
         }
       }
       
       if (s==1)
       {        
            varCode[2] = array[(i/100) % 10];    
            varCode[1] = array[(i/10) % 10];      
            varCode[0] = array[i % 10]; 
            
            if (i<100) 
            {  
               varCode[2]=0x00;
               if (i<10) 
               {
                  varCode[1]=0x00;}
            }
           
           write_expanded_outputs_MA7DOAN(varCode);
           write_expanded_outputs_BUZZER(varCode0);
                           
         }
       }
   }

