#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)

// --- Cc d?nh nghia chn (gi? nguyn) ---
#define EXP_OUT_ENABLE_BUZZER   PIN_F4      //MBI_LE0
#define EXP_OUT_CLOCK           PIN_D6      //MBI_SCK
#define EXP_OUT_DO              PIN_D4      //MBI_SDI
#define G_ALL                   PIN_C5      //MBI_OE
#define EXP_OUT_ENABLE_32LED    PIN_B3  
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6  
#define btn3                    PIN_B7      // Nt gi?m
#define btn2                    PIN_B4      // Nt tang
#define btn1                    PIN_F1      // Nt CH?Y/D?NG
#define btn0                    PIN_
// --- Cc d?nh nghia thu vi?n (gi? nguyn) ---
#define NUMBER_OF_74595_Buzzer 2
#define NUMBER_OF_74595_32_LEDs 4
#define NUMBER_OF_74595_MA7DOAN 8
#include <74595_Buzzer.c>
#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

const int8 array[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 varCode[8];     
unsigned int8 varCode1[4];     
int1 state_is_running = 0;     
unsigned int8 setting_value;    
unsigned int8 count_value;      
unsigned int32 led_effect_var;  


void display_number(unsigned int8 number, int8 position)
{

   varCode[position+1] = array[number / 10];
   varCode[position] = array[number % 10];
}

void main() 
{   

    output_low(G_ALL);
    setting_value = 0; 
    
    while(TRUE)
    {

        if(!input(btn1))
        {
            delay_ms(20); 
            state_is_running = ~state_is_running; 
            while(!input(btn1)); 
        }
        
        if(state_is_running == 0)
        {
          
            if(!input(btn2))
            {
                delay_ms(20);
                setting_value++; 
                if(setting_value > 32) 
                  setting_value = 0;
                while(!input(btn2));
            }
            if(!input(btn3))
            {
                delay_ms(20);
                if(setting_value == 0) setting_value = 32;
                else setting_value--;
                while(!input(btn3));
            }
            display_number(setting_value, 6);
            varCode[0] = 0x00;
            varCode[1] = 0x00;
            write_expanded_outputs_MA7DOAN(varCode);
        }
        else 
        {
            led_effect_var = 0; 
            for(count_value = 0; count_value <= setting_value; count_value++)
            {

                display_number(count_value, 0);
                
                display_number(setting_value, 6);
                led_effect_var = (led_effect_var << 1) + 1;
                varCode1[0] = led_effect_var;
                varCode1[1] = led_effect_var >> 8;
                varCode1[2] = led_effect_var >> 16;
                varCode1[3] = led_effect_var >> 24;

                write_expanded_outputs_32LED(varCode1);
                write_expanded_outputs_MA7DOAN(varCode);
                delay_ms(200);
                
                if(!input(btn1))
                {
                    delay_ms(20);
                    state_is_running = 0; 
                    while(!input(btn1));
                    break;
                }
            }
            state_is_running = 0; 
        }
    }
}
