#include <18F6722.h>
#include <stdlib.h> // Thm th vi?n ny ? dng hm ng?u nhin rand()
#fuses HS,MCLR
#use delay(clock=40MHz)

// --- Cc ?nh ngh?a chn ---
#define EXP_OUT_ENABLE_BUZZER   PIN_F4      //MBI_LE0
#define EXP_OUT_CLOCK           PIN_D6      //MBI_SCK
#define EXP_OUT_DO              PIN_D4      //MBI_SDI
#define G_ALL                   PIN_C5      //MBI_OE
#define EXP_OUT_ENABLE_32LED    PIN_B3  
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6  
#define btn3                    PIN_B7      // Nt gi?m
#define btn2                    PIN_B4      // Nt tng
#define btn1                    PIN_F1      // Nt CH?Y/D?NG
#define btn0                    PIN_F0      // Nt MODE <-- B?N H?Y CH?N CHN PH H?P

// --- Cc ?nh ngh?a th vi?n ---
#define NUMBER_OF_74595_Buzzer 2
#define NUMBER_OF_74595_32_LEDs 4
#define NUMBER_OF_74595_MA7DOAN 8
#include <74595_Buzzer.c>
#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

// M?ng m? LED 7 o?n Cathode
const int8 array[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

// --- Khai bo bi?n ---
unsigned int8 varCode[8];       
unsigned int8 varCode1[4];      
int1 state_is_running = 0;      // 0 = Ci ?t, 1 = Ch?y
unsigned int8 mode = 1;         // Bi?n lu ch? ?, m?c ?nh l 1
unsigned int8 setting_value;    
unsigned int8 count_value;      
unsigned int32 led_effect_var;
signed int8 i; // Dng cho v?ng l?p ?m ng?c

// Hm hi?n th? s? ln 2 LED 7 o?n
// ? s?a l?i th? t? ? hi?n th? ng: [hng ch?c][hng n v?]
void display_number(unsigned int8 number, int8 position)
{
   varCode[position+1] = array[number / 10]; // Hng ch?c ? v? tr cao hn
   varCode[position]   = array[number % 10]; // Hng n v? ? v? tr th?p hn
}

void main() 
{   
    output_low(G_ALL);
    setting_value = 0; 
    srand(1); // Kh?i t?o b? sinh s? ng?u nhin
    
    while(TRUE)
    {
        // --- 1. KI?M TRA NT CHUY?N TR?NG THI (CH?Y/D?NG) ---
        if(!input(btn1))
        {
            delay_ms(20); 
            state_is_running = ~state_is_running; 
            while(!input(btn1)); 
        }
        
        // --- 2. X? L? THEO TR?NG THI ---
        if(state_is_running == 0) // TR?NG THI CI ?T
        {
            // Ki?m tra nt TNG (btn2)
            if(!input(btn2))
            {
                delay_ms(20);
                setting_value++; 
                if(setting_value > 32) setting_value = 0;
                while(!input(btn2));
            }
            // Ki?m tra nt GI?M (btn3)
            if(!input(btn3))
            {
                delay_ms(20);
                if(setting_value == 0) setting_value = 32;
                else setting_value--;
                while(!input(btn3));
            }
            // KI?M TRA NT CHUY?N MODE (btn0)
            if(!input(btn0))
            {
                delay_ms(20);
                mode++;
                if(mode > 3) mode = 1; // Quay v?ng 3 mode 1 -> 2 -> 3 -> 1
                while(!input(btn0));
            }
            
            // Hi?n th? gi tr? ang ci ?t ln 2 LED bn ph?i
            display_number(setting_value, 6);
            // Hi?n th? ch? ? ang ch?n ? LED gi?a
            varCode[4] = array[mode];
            // T?t cc LED c?n l?i
            varCode[0] = 0x00; varCode[1] = 0x00; varCode[2] = 0x00;
            varCode[3] = 0x00; varCode[5] = 0x00;

            write_expanded_outputs_MA7DOAN(varCode);
        }
        else // TR?NG THI CH?Y
        {
            // S? d?ng switch-case ? ch?n ch? ? ch?y
            switch(mode)
            {
                // -- CASE 1: ?M XUI (0 -> setting_value) --
                case 1:
                    led_effect_var = 0; 
                    for(count_value = 0; count_value <= setting_value; count_value++)
                    {
                        if(!input(btn1)) break; // Ki?m tra d?ng gi?a ch?ng
                        display_number(count_value, 0);
                        display_number(setting_value, 6);
                        
                        led_effect_var = (led_effect_var << 1) + 1;
                        varCode1[0] = led_effect_var; varCode1[1] = led_effect_var >> 8;
                        varCode1[2] = led_effect_var >> 16; varCode1[3] = led_effect_var >> 24;
                        
                        write_expanded_outputs_32LED(varCode1);
                        write_expanded_outputs_MA7DOAN(varCode);
                        delay_ms(200);
                    }
                    break;
                    
                // -- CASE 2: ?M NG?C (setting_value -> 0) --
                case 2:
                    led_effect_var = ((unsigned int32)1 << (setting_value + 1)) - 1;
                    for(i = setting_value; i >= 0; i--)
                    {
                        if(!input(btn1)) break;
                        count_value = i; // Gn gi tr? ?m
                        display_number(count_value, 0);
                        display_number(setting_value, 6);

                        led_effect_var = led_effect_var >> 1; // Hi?u ?ng LED t?t d?n
                        varCode1[0] = led_effect_var; varCode1[1] = led_effect_var >> 8;
                        varCode1[2] = led_effect_var >> 16; varCode1[3] = led_effect_var >> 24;

                        write_expanded_outputs_32LED(varCode1);
                        write_expanded_outputs_MA7DOAN(varCode);
                        delay_ms(200);
                    }
                    break;
                
                // -- CASE 3: HI?N TH? S? NG?U NHIN --
                case 3:
                    // V?ng l?p ny s? ch?y m?i cho ?n khi nh?n btn1
                    while(input(btn1))
                    {
                        // T?o s? ng?u nhin t? 0 ?n setting_value
                        count_value = rand() % (setting_value + 1);
                        display_number(count_value, 0);
                        display_number(setting_value, 6);

                        // Hi?u ?ng 1 LED sng t?i v? tr ng?u nhin
                        led_effect_var = (unsigned int32)1 << count_value;
                        varCode1[0] = led_effect_var; varCode1[1] = led_effect_var >> 8;
                        varCode1[2] = led_effect_var >> 16; varCode1[3] = led_effect_var >> 24;

                        write_expanded_outputs_32LED(varCode1);
                        write_expanded_outputs_MA7DOAN(varCode);
                        delay_ms(300); // Tng delay cho d? quan st
                    }
                    break;
            }
            
            // Sau khi ch?y xong ho?c nh?n d?ng, t? ?ng quay v? tr?ng thi ci ?t
            delay_ms(20);
            while(!input(btn1)); // ?m b?o nt d?ng ? ?c nh? ra
            state_is_running = 0; 
        }
    }
}
