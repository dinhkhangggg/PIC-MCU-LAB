#include <18F6722.h>
#fuses HS,NOMCLR
#use delay(clock=40MHz)

#define EXP_OUT_ENABLE_BUZZER   PIN_F4      //MBI_LE0
#define EXP_OUT_CLOCK           PIN_D6      //MBI_SCK
#define EXP_OUT_DO              PIN_D4      //MBI_SDI
#define G_ALL                   PIN_C5      //MBI_OE
#define EXP_OUT_ENABLE_32LED    PIN_B3  
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6  
#define btn3                    PIN_B7      
#define btn2                    PIN_B4      
#define btn1                    PIN_F1      
#define btn0                    PIN_B6
#define rst                     PIN_G5
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
int8 s;       


void main() 
{   

    output_low(G_ALL);
    varCode0[1]=0b00000000;
    varCode0[0]=0b00000000;
     
    /*varcode1[0]=0x01;
    varCode1[1] = 0;
    varCode1[2] = 0;
    varCode1[3] = 0;
    write_expanded_outputs_32LED(varCode1);*/
   
    while(TRUE)
    {
        if(!input(btn3))
        {
            delay_ms(20); 
            s=1;
            while(!input(btn3));
            //delay_ms(50);
        }
        if (!input(rst))
        {
            delay_ms(20);
            s=2;
            while(!input(rst));
            //delay_ms(50);
        }
        
        
        if(s == 1)
        {
           varCode1[0]=0b00000001;
           write_expanded_outputs_32LED(varCode1);
           delay_ms(500);
           varCode1[0]=0b00000000;
           write_expanded_outputs_32LED(varCode1);  
           delay_ms(500);
        }
        else if (s==0)
        {
         varCode1[0]=0b00000001; 
         write_expanded_outputs_32LED(varCode1);  
        }
        else if (s==2)
        {
         varCode1[0]=0b00000000;
         write_expanded_outputs_32LED(varCode1);
        }
        else;
    
    write_expanded_outputs_BUZZER(varCode0);

    }
}
