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
unsigned int8 varCode1[8];
//unsigned int32 dv, chuc, tram, ngan, cn, tn, tr, ctr;

void main() 
{   
   int32 i;
   int32 x = 0;
   output_low(G_ALL);
   
     varCode0[1]=0b00000000;
     varCode0[0]=0b00000000;

   
   while(TRUE)
   {

     for(i=0;i<=1000000;i++)
      {        
         
         
         varCode[7] = array[(i/10000000) % 10]; 
         varCode[6] = array[(i/1000000) % 10]; 
         varCode[5] = array[(i/100000) % 10];   
         varCode[4] = array[(i/10000) % 10];   
         varCode[3] = array[(i/1000) % 10];    
         varCode[2] = array[(i/100) % 10];    
         varCode[1] = array[(i/10) % 10];      
         varCode[0] = array[i % 10];  
         
         if (i<10000000) 
         {
            varCode[7]=0x00;
            if (i<1000000) 
            {
               varCode[6]=0x00;
                if (i<100000) 
                {
                  varCode[5]=0x00;
                  if (i<10000) 
                  {
                     varCode[4]=0x00;
                     if (i<10000) 
                     {
                        varCode[4]=0x00;
                        if (i<1000) 
                           {
                              varCode[3]=0x00;
                              if (i<100) 
                                 {
                                    varCode[2]=0x00;
                                    if (i<10) varCode[1]=0x00;}
                           }}}}}}
         
         varCode1[0] = x;
         varCode1[1] = x>>8;
         varCode1[2] = x>>16;
         varCode1[3] = x>>24;
         varCode1[4] = x>>32;
         varCode1[5] = x>>40;
         varCode1[6] = x>>44;
         varCode1[7] = x>>52;
         
         x = i+0x01;
           
        
         write_expanded_outputs_32LED(varCode1);
         write_expanded_outputs_MA7DOAN(varCode);
         write_expanded_outputs_BUZZER(varCode0);
         delay_us(500);
     
  }
 }
}

