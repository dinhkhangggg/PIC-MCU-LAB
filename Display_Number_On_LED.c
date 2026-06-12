#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)
#define EXP_OUT_ENABLE_BUZZER         PIN_F4   //MBI_LE0
#define EXP_OUT_CLOCK            PIN_D6   //MBI_SCK
#define EXP_OUT_DO               PIN_D4   //MBI_SDI
#define G_ALL                    PIN_C5   //MBI_OE
#define EXP_OUT_ENABLE_32LED     PIN_B3 
#define EXP_OUT_ENABLE_MA7DOAN         PIN_F6 
#define NUMBER_OF_74595_Buzzer 2
#define NUMBER_OF_74595_32_LEDs 4
#define NUMBER_OF_74595_MA7DOAN 8
#include <74595_Buzzer.c>
#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>
int8 array[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned int8 varCode[8];
unsigned int8 varCode0[2];
unsigned int8 varCode1[4];

void main() 
{   
   int32 i;
   int32 x = 0;
   output_low(G_ALL);
   
     varCode0[1]=0b00000000;
     varCode0[0]=0b00000000;
     //const unsigned char Khang[5] = {0x76, 0x74, 0x5F, 0x54, 0x6F};


   
   while(TRUE)
   {

     
     //x = 0;
     for(i=0;i<=32;i++)
      {  
         
         varCode[7] = array[i/10000000];
         varCode[6] = array[i/1000000];
         varCode[5] = array[i/100000];
         varCode[4] = array[i/10000];         
         varCode[3] = array[i/1000];
         varCode[2] = array[i/100];
         varCode[1] = array[i/10];
         varCode[0] = array[i%10];
         
        
         
         
         varCode1[0] = x;
         varCode1[1] = x>>8;
         varCode1[2] = x>>16;
         varCode1[3] = x>>24;
         x = i+0x01;
          
        
         write_expanded_outputs_32LED(varCode1);
          write_expanded_outputs_MA7DOAN(varCode);
         write_expanded_outputs_BUZZER(varCode0);
         delay_ms(500);
     
     /*if(i==32)
     {
      for(i=32;i>=0;i--)
      {  
         
         varCode[1] = array[i/10];
         varCode[0] = array[i%10];
   
         x = (i<<1);
         varCode1[0] = x;
         varCode1[1] = x>>8;
         varCode1[2] = x>>16;
         varCode1[3] = x>>24;
         write_expanded_outputs_32LED(varCode1);
   
         write_expanded_outputs_MA7DOAN(varCode);
         write_expanded_outputs_BUZZER(varCode0);
         delay_ms(200);
         if(i==0) break;
      }
     }*/
     
    }
     
   }     
} 
