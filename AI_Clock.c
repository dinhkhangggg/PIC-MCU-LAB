#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)

// --- Các ð?nh ngh?a chân (gi? nguyên) ---
#define EXP_OUT_ENABLE_BUZZER   PIN_F4      //MBI_LE0
#define EXP_OUT_CLOCK           PIN_D6      //MBI_SCK
#define EXP_OUT_DO              PIN_D4      //MBI_SDI
#define G_ALL                   PIN_C5      //MBI_OE
#define EXP_OUT_ENABLE_32LED    PIN_B3  
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6  
#define btn3                    PIN_B7      // Nút GI?M (-)
#define btn2                    PIN_B4      // Nút TÃNG (+)
#define btn1                    PIN_F1      // Nút MODE (Ch?n ch? ð?)

// --- Các ð?nh ngh?a thý vi?n (gi? nguyên) ---
#define NUMBER_OF_74595_MA7DOAN 8
#include <74595_MA7DOAN.c>

// M?ng m? LED 7 ðo?n Cathode
const int8 array[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

// --- Bi?n cho ð?ng h? ---
unsigned int8 hours, minutes, seconds;
unsigned int8 setting_mode = 0; // 0=Ch?y, 1=Ch?nh gi?, 2=Ch?nh phút
int1 blink_state = 0;          // Tr?ng thái ð? làm hi?u ?ng nh?p nháy

// --- Bi?n cho b? ð?m ng?t ---
int8 tick_counter = 0;

// --- C?u h?nh ng?t Timer1 ---
// Timer1 s? ðý?c c?u h?nh ð? t?o ra 20 ng?t m?i giây (m?i 50ms)
// Khi ð?m ð? 20 ng?t, ta có 1 giây.
#int_TIMER1
void TIMER1_ISR()
{
   set_timer1(3036); // N?p l?i giá tr? ban ð?u cho Timer1
   
   tick_counter++;
   if(tick_counter >= 20)
   {
      tick_counter = 0;
      seconds++;
      
      if(seconds >= 60)
      {
         seconds = 0;
         minutes++;
      }
      if(minutes >= 60)
      {
         minutes = 0;
         hours++;
      }
      if(hours >= 24)
      {
         hours = 0;
      }
   }
   
   // C? n?a giây ð?o tr?ng thái blink_state m?t l?n
   if(tick_counter == 10 || tick_counter == 20)
   {
      blink_state = ~blink_state;
   }
}

void main() 
{   
    // --- Kh?i t?o ban ð?u ---
    output_low(G_ALL);
    
    // --- Cài ð?t th?i gian m?c ð?nh ---
    hours = 11;
    minutes = 08;
    seconds = 00;
    
    // --- C?u h?nh Timer1 ---
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // Ngu?n xung n?i, b? chia 8
    set_timer1(3036); // N?p giá tr? ban ð?u ð? có ng?t m?i 50ms
    
    // --- Cho phép ng?t ---
    enable_interrupts(INT_TIMER1);
    enable_interrupts(GLOBAL);
    
    while(TRUE)
    {
        // --- 1. X? L? NÚT NH?N ---
        
        // Nút MODE: Chuy?n qua l?i gi?a các ch? ð?
        if(!input(btn1))
        {
            delay_ms(20);
            setting_mode++;
            if(setting_mode > 2) setting_mode = 0; // Quay v?ng 0->1->2->0
            while(!input(btn1));
        }
        
        // N?u ðang ? ch? ð? ch?nh s?a (gi? ho?c phút)
        if(setting_mode != 0)
        {
            // Nút TÃNG
            if(!input(btn2))
            {
                delay_ms(50); // Cho phép nh?n gi? ð? tãng nhanh
                if(setting_mode == 1) // Ch?nh gi?
                {
                   hours++;
                   if(hours > 23) hours = 0;
                }
                else if(setting_mode == 2) // Ch?nh phút
                {
                   minutes++;
                   if(minutes > 59) minutes = 0;
                }
                seconds = 0; // Reset giây m?i khi ch?nh
            }
            // Nút GI?M
            if(!input(btn3))
            {
                delay_ms(50);
                if(setting_mode == 1) // Ch?nh gi?
                {
                   if(hours == 0) hours = 23;
                   else hours--;
                }
                else if(setting_mode == 2) // Ch?nh phút
                {
                   if(minutes == 0) minutes = 59;
                   else minutes--;
                }
                seconds = 0;
            }
        }
        
        // --- 2. HI?N TH? LÊN LED 7 ÐO?N ---
        unsigned int8 varCode[8] = {0,0,0,0,0,0,0,0};
        
        // Hi?n th? GIÂY (v? trí 0, 1)
        varCode[1] = array[seconds / 10];
        varCode[0] = array[seconds % 10];
        
        // Hi?n th? PHÚT (v? trí 3, 4)
        if(setting_mode == 2 && blink_state) // N?u ðang ch?nh phút th? nh?p nháy
        {
            varCode[4] = 0x00;
            varCode[3] = 0x00;
        }
        else
        {
            varCode[4] = array[minutes / 10];
            varCode[3] = array[minutes % 10];
        }

        // Hi?n th? GI? (v? trí 6, 7)
        if(setting_mode == 1 && blink_state) // N?u ðang ch?nh gi? th? nh?p nháy
        {
            varCode[7] = 0x00;
            varCode[6] = 0x00;
        }
        else
        {
            varCode[7] = array[hours / 10];
            varCode[6] = array[hours % 10];
        }
        
        // Thêm d?u hai ch?m ngãn cách
        varCode[2] = 0b00110000; // D?u hai ch?m (segment e,f) - có th? thay ð?i
        varCode[5] = 0b00110000;

        write_expanded_outputs_MA7DOAN(varCode);
        delay_ms(10); // Thêm delay nh? ð? gi?m t?i cho vi ði?u khi?n
    }
         varCode0[1]=0b00000000;
     varCode0[0]=0b00000000;
              write_expanded_outputs_BUZZER(varCode0);
}
